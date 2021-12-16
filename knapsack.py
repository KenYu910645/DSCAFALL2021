import math
import os
import fileinput
from queue import PriorityQueue

C = 0# Capcity
N_OBJ = 0 # Number of object
LOWER_BOUND = -1
OBJS = []
CP_OBJS = []
PQ = PriorityQueue()
DEBUG = True

def get_ub(decision):
    #  deduce already used obj weight and add value
    cap = C
    val = 0
    for i, taken in enumerate(decision):
        if taken == 1: # It DID take this obj.
            val += OBJS[i][0]
            cap -= OBJS[i][1]

    #  delete already used obj from cp_arr
    cp_objs = []
    for i in CP_OBJS:
        if (i[0] >= len(decision)):
            cp_objs.append(i)

    # Calcualte fraction knapsnack problem
    idx = 0
    while(cap > 0 and len(cp_objs) > idx ): # TODO, asssume all objects have weight greate than zero
        _,v,w,_ = cp_objs[idx]
        if cap >= w: # Can't take all object
            val += v
            cap -= w
        else: # Can only take partial of object
            val += (cap/w)*v
            break
        idx += 1
    return val

def get_lb(decision):
    sum_value = 0
    for i, d in enumerate(decision):
        if d == 1:
            sum_value += OBJS[i][0]
    return sum_value

def is_feasible(decision):
    sum_weight = 0
    for i, d in enumerate(decision):
        if d == 1:
            sum_weight += OBJS[i][1]

    if sum_weight > C:
        return False
    else:
        return True


if __name__ == "__main__":
    for i, line in enumerate(fileinput.input()):
        if i == 0:
            C, N_OBJ = [int(num) for num in line.split()]
        else:
            OBJS.append(tuple( [int(num) for num in line.split()] ))
    
    # Get CP value order
    for i in range(N_OBJ):
        CP_OBJS.append( (i, OBJS[i][0], OBJS[i][1], OBJS[i][0] / OBJS[i][1]) ) # (index, value, weight, cp_value)
    CP_OBJS.sort(key = lambda x: x[3], reverse=True)
    # print(f"CP_OBJS = {CP_OBJS}")
    
    # print (f"get_ub([0]) = {get_ub([0])}")
    # print (f"get_ub([1]) = {get_ub([1])}")
    
    PQ.put( (-get_ub([0]), [0]) ) # minus sign to have max queue
    PQ.put( (-get_ub([1]), [1]) )
    while not PQ.empty():
        n_fathom = PQ.get()
        n_fathom = (-n_fathom[0], n_fathom[1])# Reverse the ub's sign 

        # Check if this node worth to fathom
        if LOWER_BOUND >= n_fathom[0]: # TODO equal to get all the leafs
            continue
        
        if DEBUG:
            print("n_fathom = " + str(n_fathom))

        l_decision = n_fathom[1] + [0]
        r_decision = n_fathom[1] + [1]
        
        for child in [l_decision, r_decision]:
            if is_feasible(child):
                if len(child) == N_OBJ: # It's a leaf
                    # Calculate lb
                    lb = get_lb(child)
                    if lb > LOWER_BOUND: # Tighter lb # TODO equal to get all the leafs
                        LOWER_BOUND = lb
                        if DEBUG:
                            print("Update lb to " + str(LOWER_BOUND))
                else:
                    PQ.put( (-get_ub(child), child) )

    print(LOWER_BOUND)