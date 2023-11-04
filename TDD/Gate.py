from functools import reduce
import numpy as np
from TN import Index,Tensor,TensorNetwork
from TDD_Q import reshape
from TDD import TDD, Node,cont,add

class Gate :
    def __init__(self,matrix:np.array) -> None: 
        if np.shape(matrix)[0] == np.shape(matrix)[1] and np.shape(matrix)[0] & np.shape(matrix)[0]-1 == 0 :
                self.n = np.shape(matrix)[0]
                self.mat = matrix
        else :
            raise ValueError(f"gate operator must be square, now it is a {np.shape(matrix)} matrix")

    def E(self,rho_tdd:TDD,epsilon=0.0) :
        input_indexes_key = get_tdd_indexes_key(rho_tdd=rho_tdd)

        E_tdd = self.generate_operation(input_indexes_key=input_indexes_key,epsilon=epsilon)
        rho_tdd = cont(E_tdd,rho_tdd)

        return change_index(rho_tdd,old="y",new="x")

    def generate_operation(self,input_indexes_key,epsilon=0.0):
    
        if not 0 <= epsilon <= 1 :
            raise ValueError("epsilon out of range")

        E_star_mat = self.__noise(np.conj(self.mat.T),epsilon)
        E_mat = self.__noise(self.mat,epsilon)

        index_E = self.__distribute_index(input_indexes_key)
            
        E_star_tdd = Tensor(E_star_mat,index_E[0])
        E_tdd = Tensor(E_mat,index_E[1])

        return TensorNetwork([E_star_tdd,E_tdd]).cont()

    def __noise(self,mat:np.array,epsilon:float)->np.array:
        """
        generate operation E and E_star in reshape matrix form
        """
        res = list()
        noise_E = np.eye(np.shape(mat)[0])
        res.append(reshape(np.sqrt(epsilon)*noise_E))
        res.append(reshape(np.sqrt(1-epsilon)*mat))

        return np.array(res)

    def __distribute_index(self,input_range:list)-> list :
        """
        distribute indexes to get indexes of E_star_tdd and E_tdd
        """
        out_range= self.__generate_index_str(input_range)
        indexes_E_star = ["i"]
        indexes_E = ["i"]

        indexes_E_star.extend(cross_generate_list(out_range[::2],input_range[::2]))
        indexes_E.extend(cross_generate_list(input_range[1::2],out_range[1::2]))

        return ([Index(i) for i in indexes_E_star],[Index(i) for i in indexes_E])

    def __generate_index_str(self,range_num:list)->list[str] :
        range_list = [int(i[1:]) for i in range_num]
        return [f"y{i}" for i in range_list]

def change_index(tdd:TDD,old:str,new:str)->TDD:
    tdd = tdd.self_copy()
    for index in tdd.index_set: 
        index.key = index.key.replace(old,new)

    for key in tdd.key_2_index:
        try:
            tdd.key_2_index[key] = tdd.key_2_index[key].replace(old,new)
        except:
            continue

    tdd.index_2_key= dict([val,key]for key,val in tdd.key_2_index.items())

    return tdd

def change_index_list(tdd,olds,news)->TDD:
    tdd = tdd.self_copy()
    for i in range(len(olds)):
        tdd = change_index(tdd,olds[i],news[i])
    return tdd

def tdd_product(tdd1:TDD,tdd2:TDD)->TDD:
    """
    tdd1 and tdd2 have different index set
    """
    index_1 = get_tdd_indexes_key(tdd1)
    index_2 = get_tdd_indexes_key(tdd2)

    if index_1 == index_2 :
        raise ValueError("the index of tdd1 and tdd2 should not be equal")
    if len(index_1) != len(index_2) :
        raise ValueError("the number of tdd1 and tdd2 must be equal")

    tdd2 = change_index_list(tdd2,index_2[::2],index_1[1::2])
    return cont(tdd1,tdd2)

def tdd_add(tdd1,tdd2):
    if not tdd1.index_set == tdd2.index_set :
        raise ValueError("the index of tdd1 and tdd2 should be equal")
    res_tdd = add(tdd1,tdd2)
    tdd_initial(res_tdd,tdd1)
    return res_tdd

def tdd_initial(res_tdd,original_tdd)->None:
    """
    it is a dangerous function, use it carefully
    res_tdd.index_set = []
    so need original_tdd
    """
    res_tdd.index_set=original_tdd.index_set
    res_tdd.key_2_index=original_tdd.key_2_index
    res_tdd.index_2_key=original_tdd.index_2_key 
    res_tdd.key_width=original_tdd.key_width
    return 

def cross_generate_list(even_index:list,odd_index:list)->list:
    if not len(even_index) == len(even_index) :
        raise ValueError("The number of even and odd indexes must be equal")
    
    res = []
    for i in range(len(even_index)) :
        res.append(even_index[i])
        res.append(odd_index[i])
    return  res

def get_tdd_indexes_key(rho_tdd:TDD)->list[str]:
    return [index.key for index in rho_tdd.index_set]

def find_basis(rho_tdd:TDD,basis = 0)->list:
    res = []
    n = len(rho_tdd.index_set)//2
    res_tdd = []
    Sum = 0
    for i in range(2**n) :
        ba = bin(i).replace("0b","").zfill(n)
        proj_tdd = generate_projector(ba,rho_tdd.index_set,basis)
        prob = measure(rho_tdd,proj_tdd)
        if  prob > 0 :
            rho_tdd = proj_to_ortho(rho_tdd,proj_tdd)
            res.append([ba,prob])
            res_tdd.append(proj_tdd)
            Sum += prob
            if Sum >= 1 :
                break
    if basis:
        for i in res:
            i[0] = i[0].replace("0","+")
            i[0] = i[0].replace("1","-")
    tdd = reduce(lambda td1,td2 : tdd_add(td1,td2) , res_tdd)
    return (res,tdd)

def cir_kron(gates:list)-> Gate:
    gates = [gate.mat for gate in gates]
    cir = reduce(lambda a,b: np.kron(a,b), gates )
    return Gate(cir)

def measure(rho_tdd:TDD,proj_tdd:TDD)->float:
    proj_tdd = change_index(proj_tdd,"x","y")
    res = trace(tdd_product(rho_tdd,proj_tdd))

    return res

def proj_to(rho_tdd:TDD,proj_tdd:TDD)->TDD:
    """
    rho_tdd and proj_tdd have the same index
    """
    proj_tdd = change_index(proj_tdd,"x","y")
    temp = tdd_product(proj_tdd,rho_tdd)
    res_tdd = tdd_product(temp, proj_tdd)
    res_tdd = change_index(res_tdd,"y","x")
    return res_tdd

def proj_to_ortho(rho_tdd:TDD,proj_tdd:TDD)->TDD:
    proj_tdd = proj_tdd.self_copy()
    identity = __generate_identity_tdd(proj_tdd.index_set)
    proj_tdd.weight *= -1
    p_dot = tdd_add(identity,proj_tdd)

    return proj_to(rho_tdd,p_dot)

def trace(rho_tdd:TDD)->float:
    index = rho_tdd.index_set
    index = cross_generate_list(index[1::2],index[::2])
    identity = __generate_identity_tdd(index)
    return cont(rho_tdd,identity).weight

def generate_projector(words:list,index_set:list,basis = 0)->TDD :
    if basis == 0 :
        basis_group = np.array([[[1,0],[0,0]],[[0,0],[0,1]]])
    else :
        basis_group = 1/2* np.array([[[1,1],[1,1]],[[1,-1],[-1,1]]])
    n = len(words)
    res_tdd = [Tensor(basis_group[int(words[i])],[index_set[2*i],index_set[2*i+1]]) for i in range(n)]
    
    return TensorNetwork(res_tdd).cont()

def find_way(node:Node,s=[])->list:
    """
    return list[[node successor i th,node.key]]
    """
    if node.key == -1: return s
    n = node.succ_num
    for i in range(n) :
        if node.out_weight[i] != 0 :
            s.append([i,node.key])
            next = find_way(node.successor[i],s)
            if next : return s
            s.pop()
    return []

def generate_projector_by_way(rho_tdd:TDD)->TDD :
    way = find_way(rho_tdd.node,[])
    tdd_index = get_tdd_indexes_key(rho_tdd)
    way_key = dict([rho_tdd.key_2_index[i[1]],i[0]] for i in way)
    words = [way_key.get(i,0) for i in tdd_index[1::2]]
    return generate_projector(words,rho_tdd.index_set)
    
def find_basis_easily(rho_tdd:TDD,s=[])->list[TDD]:
    if rho_tdd.node.key == -1: return s
    proj_tdd = generate_projector_by_way(rho_tdd)
    s.append(proj_tdd)
    return find_basis_easily(proj_to_ortho(rho_tdd,proj_tdd),s)

def __generate_identity_tdd(indexes:list[Index])->TDD:
    identity_mat = np.eye(2**(len(indexes)//2))
    return Tensor(reshape(identity_mat),indexes).tdd()

I = Gate(np.array([[1,0],[0,1]]))
X = Gate(np.array([[0,1],[1,0]]))
Y = Gate(np.array([[0,-1j],[1j,0]]))
Z = Gate(np.array([[1,0],[0,-1]]))
H = Gate(1/np.sqrt(2)*np.array([[1,1],[1,-1]]))
Cx = Gate(np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]])) 
Tf = Gate(
    np.array([
        [1,0,0,0,0,0,0,0],
        [0,1,0,0,0,0,0,0],
        [0,0,1,0,0,0,0,0],
        [0,0,0,1,0,0,0,0],
        [0,0,0,0,1,0,0,0],
        [0,0,0,0,0,1,0,0],
        [0,0,0,0,0,0,0,1],
        [0,0,0,0,0,0,1,0]
        ]
    ))
T = Gate(np.array([[1,0],[0,np.exp(np.pi*0.25j)]]))
T_ = Gate(np.array([[1,0],[0,np.exp(np.pi*-0.25j)]]))
S = Gate(np.array([[1,0],[0,1j]]))