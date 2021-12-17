#include <iostream>
#include <vector>
#include <algorithm> // for sort
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
	int ub;
	int v;
	int w;
    int v_int;
    int v_frac;
};
priority_queue<Node> PQ;

void get_ub(Node& node){
    int v_int = 0;
    int v_frac = 0;
    int c_frac = C - node.w; // Remain capacity
    for (int i = node.level+1; i < N_OBJ; i++){
        if (c_frac >= OBJS[i].w) { // Can take this obj all
            c_frac -= OBJS[i].w;
            v_int += OBJS[i].v;
            if (c_frac == 0){
                break;
            }
        }
    
        else{ // Can only take fraction of the obj
            v_frac = OBJS[i].v*c_frac/OBJS[i].w;
            break;
        }
    }

    node.v_int = v_int;
    node.v_frac = v_frac;
    node.ub = node.v + v_int + v_frac;
    // cout << double(node.v) + double(v_int) << endl;
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

bool operator<(Node a, Node b){ 
    // if(a.ub == b.ub)
    //     return a.level < b.level;
    // else
    //     return (a.ub) < b.ub;
    if(a.ub == b.ub)
        return a.level < b.level;
    else
        // return (a.ub)*a.level < b.ub*b.level;
        return a.ub < b.ub;
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

    Node p_node = {-1, 999999, 0, 0, 0 ,0};
    PQ.push(p_node);

    Node l_node= {0,0,0,0,0,0};
    Node r_node= {0,0,0,0,0,0};
    int count = 0;
    int tmp = 0;

    while (not PQ.empty()){
        count++;
        p_node = PQ.top();
        PQ.pop();
        // Check if this node worth to fathom
        if (LOWER_BOUND >= p_node.ub) // TODO equal to get all the leafs
            continue;
        
        // Take the object!!! (l_node)
        l_node.level = p_node.level+1;
        l_node.v = p_node.v + OBJS[p_node.level+1].v;
        l_node.w = p_node.w + OBJS[p_node.level+1].w;
        get_ub(l_node);

        // Don't take the object!!! (r_node)
        r_node.level = p_node.level+1;
        r_node.v = p_node.v;
        r_node.w = p_node.w;
        get_ub(r_node);

        if (DEBUG){
            cout << "fathom_node = (" << p_node.level << ", " << p_node.ub << ", " << p_node.v << ", " << p_node.w << ")" << endl;
            cout << "l_node = (" << l_node.level << ", " << l_node.ub << ", " << l_node.v << ", " << l_node.w << ")" << endl;
            cout << "r_node = (" << r_node.level << ", " << r_node.ub << ", " << r_node.v << ", " << r_node.w << ")" << endl;
        }

        // TODO, ignore leaf?

        // Update lower bound by A solution.
        if (l_node.w <= C and l_node.v + l_node.v_int > LOWER_BOUND){
            LOWER_BOUND = l_node.v + l_node.v_int;
            //cout << "Updat lb to " << l_node.v + l_node.v_int << endl;
        }
        if (r_node.w <= C and r_node.v + r_node.v_int > LOWER_BOUND){
            LOWER_BOUND = r_node.v + r_node.v_int;
            //cout << "Updat lb to " << r_node.v + r_node.v_int << endl;
        }

        // l_node
        if (l_node.w <= C){ // check feasible
            if (l_node.ub > LOWER_BOUND){// TODO I'm not very sure about this 
                if (l_node.level == N_OBJ-1){// It's a leaf
                    LOWER_BOUND = l_node.ub;
                    //cout << "Updat lb to " << l_node.v + l_node.v_int << endl;
                    //LOWER_BOUND = l_node.v;
                }
                else{
                    PQ.push( l_node );
                }
            }
        }

        // r_node
        if (r_node.w <= C){
            if (r_node.ub > LOWER_BOUND){// TODO I'm not very sure about this 
                if (r_node.level == N_OBJ-1){ // It's a leaf
                    LOWER_BOUND = r_node.ub;
                    //cout << "Updat lb to " << r_node.v + r_node.v_int << endl;
                    //LOWER_BOUND = r_node.v;
                }
                else{
                    PQ.push( r_node );
                }
            }
        }
    }


    cout << LOWER_BOUND << endl;
    // cout << "percent = " << (double)count/pow(2, N_OBJ) << endl;
}
