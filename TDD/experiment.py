import numpy as np
from TDD import cont,get_identity_tdd,add,conjugate,cont
from TN import Index,Tensor,TensorNetwork
import networkx as nx
import copy
from functools import reduce
from QA import QuantumAutomata

ket_a=1/np.sqrt(2)*np.array([1,1])
ket_m=1/np.sqrt(2)*np.array([1,-1])
ket_0=np.array([1,0])
ket_1=np.array([0,1])
ket = {"+":ket_a,"-":ket_m,"0":ket_0,"1":ket_1}

def generate_ini_state(qubits_num:int,state:str):
    initial_state=TensorNetwork([])
    for k in range(qubits_num):
        ts=Tensor(ket[state[k]],[Index(f"x{k}")])
        initial_state.tensors.append(ts)
    return initial_state

def change_index(tdd,old,new):
    temp=[]
    for k in tdd.index_set:
        try:
            temp.append(Index(k.key.replace(old,new,1),k.idx))
        except:
            temp.append(k)
    tdd.index_set=temp

    for k in tdd.key_2_index:
        try:
            tdd.key_2_index[k] = tdd.key_2_index[k].replace(old,new,1)
        except:
            continue

    tdd.index_2_key= dict([val,key]for key,val in tdd.key_2_index.items())

def simulation(op,phi):
    """phi, op are the TDD representation of the input state and operator"""
    res=cont(op,phi)
#     change_index(res,'y','x')
    return res

def update_tdd_after_add(res_tdd, apply_tdd):
    res_tdd.index_set= apply_tdd.index_set
    res_tdd.key_2_index= apply_tdd.key_2_index
    res_tdd.index_2_key= apply_tdd.index_2_key
    res_tdd.key_width= apply_tdd.key_width

def generate_initial_space(initial_states):
    sp=get_identity_tdd()
    sp.weight=0
    for phi in initial_states:
        sp,_=join(sp,phi)
    return sp

def join(sp,phi):
    """return the join of a subspace sp and a quantum state s"""
    p=cont(sp,phi)

    p.weight*=-1
    phi2=add(phi,p)
    update_tdd_after_add(phi2,phi)
    phi_conj=conjugate(phi2)

    w=cont(phi2,phi_conj).weight
    try:
        phi2.weight/=np.sqrt(w)
        phi_conj.weight/=np.sqrt(w)
        change_index(phi_conj,'x','y')
        phi_proj=cont(phi2,phi_conj)
        sp=add(sp,phi_proj)
        update_tdd_after_add(sp,phi_proj)
    except:
        pass
    return sp,phi2

def image(qa):
    sp= generate_initial_space(qa.initial_states)
    max_node = 0
    for op in qa.operators:
        qa.operators[op],node = qa.operators[op].cont()
        max_node = max(max_node,node)
    res_list = []
    for op in qa.operators:
        for phi in qa.initial_states:
            max_node= max(max_node,qa.operators[op].node_number())
            res=simulation(qa.operators[op],phi)
            res_list.append(res)
            change_index(res,'y','x')
            max_node = max(max_node,res.node_number())
            sp,_=join(sp,res)
    return sp,max_node

def image_cont_par(qa,k1=0,k2=0):
    sp= generate_initial_space(qa.initial_states)
    max_node = 0
    for op in qa.operators:
        qa.operators[op],node=contraction_partition(qa.operators[op],k1,k2)
        max_node = max(node,max_node)

    res_list=[]
    for op in qa.operators:
        for phi in qa.initial_states:
            res=phi
            for p in qa.operators[op]:
                max_node = max(max_node,p.node_number())
                res=simulation(p,res)
                max_node = max(max_node,res.node_number())
            change_index(res,'y','x')
            res_list.append(res)
            sp,_=join(sp,res)
    return sp,max_node

def image_add_par(tn,qa,k=0,partial=0):
    sp= generate_initial_space(qa.initial_states)

    if not partial:
        for op in qa.operators:
            qa.operators[op]=addition_partition(tn,k)
    else:
        for op in qa.operators:
            qa.operators[op]=addition_partition_partial(tn,k)

    for op in qa.operators:
        for phi in qa.initial_states:
            res=get_identity_tdd()
            res.weight=0
            for p in qa.operators[op]:
                temp=simulation(p,phi)
                change_index(temp,'y','x')
                res=add(res,temp)
            update_tdd_after_add(res,temp)
            sp,_=join(sp,res)

    return sp

def addition_partition(tn,k=0):
    """partition a tensor network tn according to k nodes with biggest degree"""

    lin_graph=nx.Graph()

    tn.get_index_set()

    lin_graph.add_nodes_from(tn.index_set)

    for ts in tn.tensors:
        for k1 in range(len(ts.index_set)):
            for k2 in range(k1+1,len(ts.index_set)):
                if ts.index_set[k1].key!=ts.index_set[k2].key:
                    lin_graph.add_edge(ts.index_set[k1].key,ts.index_set[k2].key)


    deg=[lin_graph.degree(key) for key in lin_graph.nodes]

    nodes_with_biggest_degree=[]

    node_list=list(lin_graph.nodes)

    t=k
    while t>0:
        biggest_degree=max(deg)
        if biggest_degree<=0:
            break
        idx=deg.index(biggest_degree)
        if "_" in node_list[idx]:
            nodes_with_biggest_degree.append(node_list[idx])
        deg[idx]=0
        t-=1

    dd_list=[]
    for t in range(2**k):
        b=list(bin(t)[2:])
        b.reverse()
        temp_tn=copy.copy(tn)
        temp_tn.tensors=[]
        for ts in tn.tensors:
            temp_ts=copy.copy(ts)
            for idx in ts.index_set:
                if idx.key in nodes_with_biggest_degree:
                    if nodes_with_biggest_degree.index(idx.key)< len(b) and b[nodes_with_biggest_degree.index(idx.key)]=='1':
                        temp_ts.dd=cont(temp_ts.tdd(),Tensor(ket1,[idx]).tdd())
                    else:
                        temp_ts.dd=cont(temp_ts.tdd(),Tensor(ket0,[idx]).tdd())
            temp_tn.tensors.append(temp_ts)
        dd_list.append(temp_tn.cont())

    # A=[a.node_number() for a in dd_list]
    # print(A)

    return dd_list

def contraction_partition(tn,k1=0,k2=0):
    dd_list=[]

    cir_par=circuit_partition(tn,k1,k2)
    max_node = 0
    for ver in cir_par:
        for hor in ver:
            tdd,node = TensorNetwork(hor).cont()
            max_node = max(max_node,node)
            dd_list.append(tdd)

    return dd_list,max_node

def circuit_partition(tn,k1=0,k2=0):
    """The first partition scheme;
    cx_max is the number of CNOTs allowed to be cut
    """

    num_qubit=tn.qubits_num

    if k1==0:
        blocks_num=2
        k1=np.ceil(num_qubit/2)
    else:
        blocks_num=int(np.ceil(num_qubit/k1))
    res=[[[] for _ in range(blocks_num)]]

    if k2==0:
        cx_max=num_qubit//2
    else:
        cx_max=k2

    cx_num=0
    level=0

    for ts in tn.tensors:
        q = ts.qubits
        mi=int(np.floor(min(q)/k1))
        ma=int(np.floor(max(q)/k1))
        if mi==ma:
            res[level][mi].append(ts)
        else:
            cx_num+=1
            if cx_num<=cx_max:
                res[level][int(np.floor(q[-1]/k1))].append(ts)
            else:
                level+=1
                res.append([])
                for k in range(blocks_num):
                    res[level].append([])
                res[level][int(np.floor(q[-1]/k1))].append(ts)
                cx_num=1

    return res

def addition_partition_partial(tn,k=0):
    """partition a tensor network tn according to k nodes with biggest degree"""

    lin_graph=nx.Graph()

    tn.get_index_set()

    lin_graph.add_nodes_from(tn.index_set)

    for ts in tn.tensors:
        for k1 in range(len(ts.index_set)):
            for k2 in range(k1+1,len(ts.index_set)):
                if ts.index_set[k1].key!=ts.index_set[k2].key:
                    lin_graph.add_edge(ts.index_set[k1].key,ts.index_set[k2].key)

    deg=[lin_graph.degree(key) for key in lin_graph.nodes]

    nodes_with_biggest_degree=[]

    node_list=list(lin_graph.nodes)

    t=k
    while t>0:
        bigggest_degree=max(deg)
        if bigggest_degree<=0:
            break
        idx=deg.index(bigggest_degree)
        if "_" in node_list[idx]:
            nodes_with_biggest_degree.append(node_list[idx])
        deg[idx]=0
        t-=1
#     print(nodes_with_biggest_degree)

    tn_list=[]
    dd_list=[]

    ket0=np.array([1,0])
    ket1=np.array([0,1])

    for t in range(2**k):
        b=list(bin(t)[2:])
        b.reverse()
        temp_tn=copy.copy(tn)
        temp_tn.tensors=[]
        for ts in tn.tensors:
            temp_ts=copy.copy(ts)
            for idx in ts.index_set:
                if idx.key in nodes_with_biggest_degree:
                    if nodes_with_biggest_degree.index(idx.key)< len(b) and b[nodes_with_biggest_degree.index(idx.key)]=='1':
                        temp_ts.dd=cont(temp_ts.tdd(),Tensor(ket1,[idx]).tdd())
                    else:
                        temp_ts.dd=cont(temp_ts.tdd(),Tensor(ket0,[idx]).tdd())
            temp_tn.tensors.append(temp_ts)
#         tn_list.append(temp_tn)
        dd_list.append(temp_tn.cont())

        A=[a.node_number() for a in dd_list]
        print(A)
        return dd_list