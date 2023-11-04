import Gate as G
from TDD import normalize,get_identity_tdd,TDD,Node
from functools import reduce
from copy import deepcopy
from collections import Counter
import hashlib
# todo: functions are not beautiful, too large to read, too danger to use.
def __generate_successor(node:Node, out_weight:list[float], window:dict)->list[TDD]:
    successor = [partition(node.successor[i], window) if out_weight[i] else get_identity_tdd() for i in range(node.succ_num)]
    for i, s in enumerate(successor):
        s.weight = out_weight[i] if out_weight[i] else 0
    return successor

def partition(node:Node, window:dict)->TDD:
    if node.key == -1 or not window : return TDD(node)
    out_weight = deepcopy(node.out_weight)
    try:
        w = window.pop(node.key)
        for i in range(node.succ_num) :
            if i != w : out_weight[i] = 0
        successor = __generate_successor(node,out_weight,window)
        window[node.key] = w
    except:
        successor = __generate_successor(node,out_weight,window)
    normalized_successor = normalize(node.key,successor)
    normalized_successor.node.out_weight = out_weight
    return normalized_successor

def __window_and(window1: dict, window2: dict) -> dict:
    return {key: window1.get(key, 0) and window2.get(key, 0) for key in {key for key in window1.keys()} | {key for key in window2.keys()}}

def __window_or(window1: dict, window2: dict) -> dict:
    return {key: window1.get(key, 0) or window2.get(key, 0) for key in {key for key in window1.keys()} | {key for key in window2.keys()}}

def check_windows(windows: list[dict]) -> bool:
    if len(windows) < 2:
        return False
    
    or_all_window = reduce(lambda window1, window2: __window_or(window1, window2), windows)
    
    if set(or_all_window.values()) == {1}:
        return not any(set(__window_and(windows[i], window2).values()) == {1} for i in range(len(windows)) for window2 in windows[i+1:])
    else:
        return False


def tdd_partition(tdd: TDD, windows: list[dict]) -> list[TDD]:
    if not check_windows(windows):
        return []

    partitions = [partition(tdd.node, window) for window in windows]
    for partitioned_tdd in partitions:
        G.tdd_initial(partitioned_tdd,tdd)
        if partitioned_tdd.weight:
            partitioned_tdd.weight = tdd.weight

    return partitions


def tdd_split(tdd:TDD, split_point:int)-> list[TDD,TDD]:
    B= split_before(tdd.node, split_point)
    A= split_after(tdd.node, split_point)
    return generate_tdd_index(B, A, tdd, split_point)

def split_before(node:Node, split_point:int)->TDD:
    successor = list()
    for i in range(node.succ_num):
        if node.successor[i].key == -1:
            new = get_identity_tdd()
            new.weight = node.out_weight[i]
            successor.append(new)
            continue
        if node.successor[i].key == split_point:
            new = get_identity_tdd()
        else :
            new = split_before(node.successor[i],split_point)
        new.weight = node.out_weight[i]
        successor.append(new)
    res_tdd = normalize(node.key,successor)
    return res_tdd

def split_after(node:Node, split_point:int)->TDD:
    if node.key == -1: return -1
    if node.key != split_point :
        for s in node.successor:
            t = split_after(s,split_point)
            if type(t) == TDD : return t
    return TDD(node)

def __change(node:Node, num:int):
    # waring: dangerous function
    S = [node]
    record_visit = [node.idx]
    if node.key < num: return
    while S :
        node = S.pop()
        node.key -= num
        for succ in node.successor:
            if succ.key == -1 : continue
            if succ.idx not in record_visit :
                S.append(succ)
                record_visit.append(succ.idx)
    return
# def __change(node:Node,num:int):
#     # waring: dangerous function
#     def dfs(node):
#         if not node: return -2
#         succ_key = []
#         for succ in node.successor:
#             succ_key.append(dfs(succ))
#         node.key = max(succ_key) + 1
#         return node.key
#     dfs(node=node)
#     return

def find_split_point(tdd:TDD)->int:
    idx_2_key = {}
    nodes = [tdd.node]
    times = [0 for i in range(tdd.node.key+1)]
    while nodes:
        node = nodes.pop()
        idx_2_key[node.idx] = node.key
        if not node.key == -1:
            times[node.key] += 1
            for succ in node.successor:
                    nodes.append(succ)
    max_time = times[-1]
    keys = list(idx_2_key.values())
    point = None
    for i in range(len(times)-1,-1,-1):
        if times[i] > max_time :
            max_time = times[i]
            if keys.count(i) == 1:
                point = i
    return point

def generate_tdd_index(before_tdd:TDD, after_tdd:TDD, origin_tdd:TDD, split_key:int)->list[TDD,TDD]:
    # todo: need be refactored
    split_key = split_key + 1
    n = len(origin_tdd.index_set) - split_key

    before_tdd.weight=origin_tdd.weight
    before_tdd.index_set=deepcopy(origin_tdd.index_set[:n])
    after_tdd.index_set=deepcopy(origin_tdd.index_set[n:])
    index_B = [b.key for b in before_tdd.index_set]
    index_A = [a.key for a in after_tdd.index_set]

    before_tdd.key_2_index[-1] = -1
    after_tdd.key_2_index[-1] = -1
    before_tdd.key_2_index.update({val-split_key:key for key,val in origin_tdd.index_2_key.items() if key in index_B})
    after_tdd.key_2_index.update({val:key for key,val in origin_tdd.index_2_key.items() if key in index_A})

    before_tdd.index_2_key={key:val for val,key in before_tdd.key_2_index.items()}
    after_tdd.index_2_key={key:val for val,key in after_tdd.key_2_index.items()}
    # todo: key_width的用途是什么
    before_tdd.key_width = {key:2 for key in list(before_tdd.key_2_index.keys())[1:]}
    after_tdd.key_width = {key:2 for key in list(after_tdd.key_2_index.keys())[1:]}
    __change(before_tdd.node, split_key)
    return(before_tdd, after_tdd)