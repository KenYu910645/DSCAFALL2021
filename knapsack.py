import math
import os
import fileinput
from queue import PriorityQueue
from collections import namedtuple
import time 

C = 0 # Capcity
N_OBJ = 0 # Number of object
LOWER_BOUND = -1
OBJS = []
PQ = PriorityQueue()
DEBUG = False

def get_ub(node):
    v_frac = 0
    c_frac = C - node.w # Remain capacity
    for i in range(node.level+1, N_OBJ):
        if c_frac >= OBJS[i].w: # Can take this obj all
            c_frac -= OBJS[i].w
            v_frac += OBJS[i].v
            if c_frac == 0:
                break
        else: # Can only take fraction of the obj
            v_frac += OBJS[i].v*c_frac/OBJS[i].w
            break
    return node.v + v_frac

class Node():
    def __init__(self, level, ub, v, w):
        self.level = level
        self.ub = ub
        self.v = v
        self.w = w
    def __lt__(self, other): # Define node < node
        return (self.ub < other.ub)

Obj = namedtuple('Obj', 'v, w')

def fathom(parent):
    # Take the object!!! (l_node)
    l_node = Node(parent.level+1,
             None,
             parent.v + OBJS[parent.level+1].v,
             parent.w + OBJS[parent.level+1].w)
    l_node.ub = get_ub(l_node)

    # Don't take the object!!! (r_node)
    r_node = Node(parent.level+1,
             None,
             parent.v,
             parent.w)
    r_node.ub = get_ub(r_node)

    return l_node, r_node

if __name__ == "__main__":
    t_start = time.time()
    for i, line in enumerate(fileinput.input()):
        if i == 0:
            C, N_OBJ = [int(num) for num in line.split()]
        else:
            v_str, w_str = line.split()
            OBJS.append(Obj(int(v_str), int(w_str)))
    # Sort by cp value
    OBJS.sort(key = lambda x: x.v/x.w, reverse=True)
    # if DEBUG:
    #     print(f"OBJS = {OBJS}")
    
    PQ.put( Node(-1, -9999999999, 0, 0) )
    count = 0
    while not PQ.empty():
        count += 1
        node = PQ.get()
        node.ub = -node.ub # Reverse sign
        
        # Check if this node worth to fathom
        if LOWER_BOUND >= node.ub: # TODO equal to get all the leafs
            continue
        # if DEBUG:
        #     print(f"node_fathom: level={node.level}, ub={node.ub}, v={node.v}, w={node.w}")

        l_node, r_node = fathom(node)
        # if DEBUG:
        #     print(f"l_node: level={l_node.level}, ub={l_node.ub}, v={l_node.v}, w={l_node.w}")
        #     print(f"r_node: level={r_node.level}, ub={r_node.ub}, v={r_node.v}, w={r_node.w}")
        
        # check child
        for n_child in [l_node, r_node]:
            # check feasible and worth fathom
            if n_child.w > C or n_child.ub <= LOWER_BOUND: # TODO I'm not very sure about this 
                continue

            if n_child.level == N_OBJ-1: # It's a leaf
                LOWER_BOUND = n_child.ub
                if DEBUG:
                    print("Update lb to " + str(LOWER_BOUND))
            else:
                n_child.ub = -n_child.ub
                PQ.put( n_child )

    print(LOWER_BOUND)
    print(f"precentage = {count/(2**N_OBJ)}")
    print(time.time() - t_start)