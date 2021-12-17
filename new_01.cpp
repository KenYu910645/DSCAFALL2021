#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <set>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <sstream>
#include <limits>
#include <cmath> 
#include <queue>
using namespace std;

int C = 0; // Capcity
int N_OBJ = 0; // Number of object
int LOWER_BOUND = -1;

bool DEBUG = false;

struct Obj{
    int v; // value 
    int w; // weight
};
vector<Obj> OBJS;

struct Node{
	int level;
	double ub;
	double v;
	int w;
};
priority_queue<Node> PQ;

double get_ub(Node node){
    double v_frac = 0;
    double c_frac = C - node.w; // Remain capacity
    for (int i = node.level+1; i < N_OBJ; i++){
        if (c_frac >= OBJS[i].w) { // Can take this obj all
            c_frac -= OBJS[i].w;
            v_frac += OBJS[i].v;
            if (c_frac == 0){
                break;
            }
        }
    
        else{ // Can only take fraction of the obj
            v_frac += (double)OBJS[i].v*c_frac/OBJS[i].w;
            break;
        }
    }
    return node.v + v_frac;
}


Node fathom(Node parent, bool is_take){
    // Take the object!!! (l_node)
    if (is_take){
        Node node = {parent.level+1,
                     0,
                     parent.v + OBJS[parent.level+1].v, 
                     parent.w + OBJS[parent.level+1].w};
        node.ub = get_ub(node);
        return node;
    }

    else{
        // Don't take the object!!! (r_node)
        Node node = {parent.level+1,
                     0,
                     parent.v,
                     parent.w};
        node.ub = get_ub(node);
        return node;
    }
}

void print_vector(vector<Obj> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << "(" << x[i].v << ", " << x[i].w << "), ";
    }
    cout << "]" << endl;
}

bool cmp(Obj a, Obj b){
	return (double)a.v/a.w > (double)b.v/b.w;
}

bool operator<(Node a, Node b){ // TODO, this kind of weird
	// if(a.ub == b.ub){
	// 	return a.level < b.level;
	// }
	// else{
	// 	return a.ub > b.ub;
	// }
    return a.ub < b.ub;;
}

int main (){
    //Load input file 
    cin >> C >> N_OBJ;
    for(int i = 0; i < N_OBJ; i++){
        Obj n = {0, 0};
        cin >> n.v >> n.w;
        OBJS.push_back(n);
    }
    //print_vector(OBJS);
    
    //Sort by cp value
    sort(OBJS.begin(), OBJS.end(), cmp);
    // print_vector(OBJS);

    Node n = {-1, 9999999999, 0, 0};
    PQ.push(n);

    int count = 0;
    while (not PQ.empty()){
        count++;
        Node node = PQ.top();
        PQ.pop();
        
        // Check if this node worth to fathom
        if (LOWER_BOUND >= node.ub) // TODO equal to get all the leafs
            continue;
        
        
        Node l_node = fathom(node, true); // take
        Node r_node = fathom(node, false); //"not take"
        if (DEBUG){
            cout << "fathom_node = (" << node.level << ", " << node.ub << ", " << node.v << ", " << node.w << ")" << endl;
            cout << "l_node = (" << l_node.level << ", " << l_node.ub << ", " << l_node.v << ", " << l_node.w << ")" << endl;
            cout << "r_node = (" << r_node.level << ", " << r_node.ub << ", " << r_node.v << ", " << r_node.w << ")" << endl;
        }

        // l_node
        // check feasible and worth fathom
        if (l_node.w > C or l_node.ub <= LOWER_BOUND){// TODO I'm not very sure about this 
            ;
        }
        else{
            if (l_node.level == N_OBJ-1)// It's a leaf
                LOWER_BOUND = l_node.ub;
            else{
                PQ.push( l_node );
            }
        }

        // r_node
        if (r_node.w > C or r_node.ub <= LOWER_BOUND){// TODO I'm not very sure about this 
            ;
        }
        else{
            if (r_node.level == N_OBJ-1)// It's a leaf
                LOWER_BOUND = r_node.ub;
            else{
                PQ.push( r_node );
            }
        }
    }


    cout << LOWER_BOUND << endl;
}
