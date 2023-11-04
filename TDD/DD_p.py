import numpy as np
from TDD import cont,add,get_identity_tdd,conjugate
from TN import Index
import Partition as P

ket0=np.array([1,0])
ket1=np.array([0,1])
def change_index(tdd,old,new):
    # Replaces the first occurrence of 'old' with 'new' in the key attribute of each Index in tdd
    temp=[]
    for k in tdd.index_set:
        try:
            temp.append(Index(k.key.replace(old,new,1),k.idx))
        except:
            temp.append(k)
    # Updates tdd's index_set, key_2_index, and index_2_key accordingly
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

def dd_image(qa,dec_key:int):
    sp= generate_initial_space(qa.initial_states)

    for op in qa.operators:
        temp_tdd = qa.operators[op].cont()
        key = temp_tdd.node.key-dec_key
        windows = [{key:0},{key:1}]
        window_partition = P.tdd_partition(temp_tdd,windows)
        for i in range(len(window_partition)):
            A,B = P.tdd_split(window_partition[i],key)
            window_partition[i] = [A,B]
        qa.operators[op] = window_partition

    node_number = []
    for op in qa.operators:
        for phi in qa.initial_states:
            for w_tdd in qa.operators[op]:
                temp_tdd = phi
                for sp_tdd in w_tdd:
                    temp_tdd = simulation(sp_tdd,temp_tdd)
                    node_number.append(temp_tdd,node_number())
                    change_index(temp_tdd,'y','x')
                sp,_=join(sp,temp_tdd)
    return sp