import math
import os
import fileinput
from queue import PriorityQueue

C = 0# Capcity
N_OBJ = 0 # Number of object
LOWER_BOUND = -1
OBJS = []
CP_OBJS = []
DEBUG = True

class lazyQueue():
    def __init__(self):
        self.top_pointer = 0
        self.queue = []
    
    def put(self, x):
        self.queue.append(x)
    
    def get(self):
        top = self.queue[self.top_pointer]
        self.top_pointer += 1
        return top
    
    def clean(self, lb):
        # Delete nodes that ub is smaller than lb
        new_queue = []
        for i in range(self.top_pointer, len(self.queue)):
            if self.queue[i][0] > lb:
                new_queue.append(self.queue[i])
        # sort new_queue
        # print(new_queue)
        new_queue.sort(key = lambda x: x[0], reverse=True)
        # print(new_queue)
        self.queue = new_queue
        self.top_pointer = 0
    
    def empty(self):
        return (len(self.queue) == 0 or self.top_pointer == len(self.queue))

PQ = lazyQueue() # PriorityQueue()

def get_ub(decision, val, weight):
    # Calcualte fraction knapsnack problem
    idx = 0
    cap = C - weight
    while(cap > 0 and len(CP_OBJS) > idx ): # TODO, asssume all objects have weight greate than zero
        # if already have this decision
        if (CP_OBJS[idx][0] >= len(decision)):
            _,v,w,_ = CP_OBJS[idx]
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

def get_v_w(decision):
    sum_value = 0
    sum_weight = 0
    for i, d in enumerate(decision):
        if d == 1:
            sum_value  += OBJS[i][0]
            sum_weight += OBJS[i][1]
    return (sum_value, sum_weight)

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
    
    # PQ.put( (-get_ub([0], get_v_w([0])[0], get_v_w([0])[1] ), [0]) )# + get_v_w([0]) ) # minus sign to have max queue
    # PQ.put( (-get_ub([1], get_v_w([1])[0], get_v_w([1])[1] ), [1]) )# + get_v_w([1]) )
    PQ.put( (get_ub([0], get_v_w([0])[0], get_v_w([0])[1] ), [0]) )# + get_v_w([0]) )
    PQ.put( (get_ub([1], get_v_w([1])[0], get_v_w([1])[1] ), [1]) )# + get_v_w([1]) )
    PQ.clean(LOWER_BOUND)

    while not PQ.empty():
        n_fathom = PQ.get()

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
                        # delete all node small than now lower bound
                        PQ.clean(LOWER_BOUND)
                        if DEBUG:
                            print("Update lb to " + str(LOWER_BOUND))
                else:
                    v, w = get_v_w(child)
                    ub = get_ub(child, v, w)
                    if ub > LOWER_BOUND: # Don't put if get_ub([0]) is smaller than lb# TODO equal to get all the leafs
                        PQ.put( (ub, child) )
                        PQ.clean(LOWER_BOUND)
    print(LOWER_BOUND)