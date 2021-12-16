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
int C = 0;// Capcity
int N_OBJ = 0; // Number of object
struct Node{
    int ub;
    vector<int> decision;
};
priority_queue < pair<int, Node> > PQ; // for nodes that wait for famthom
vector<vector<int>> OBJS;

void print_vector(vector<vector<int>> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        if (i != 0)
            cout << " ";
        cout << "[";
        for (int j = 0; j < x[i].size(); j++){
            cout << x[i][j] << ' '; 
        }
        cout << "]";
        if (i != x.size()-1){
            cout << "," << endl;
        }   
    }
    cout << "]" << endl;
}

void print_vector(vector<pair<int,double>> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << "(" << x[i].first << ", " << x[i].second << ")";
        if (i != x.size()-1)
            cout << ", "; 
    }
    cout << "]" << endl;
}

void print_vector(vector<int> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << x[i] << ' '; 
    }
    cout << "]" << endl;
}


bool cmp(pair<int, double>a, pair<int, double>b)
{
    return a.second > b.second;
}

double get_ub(Node n){
    //Get fraction-knapsanck order
    vector <pair<int, double>> cp_arr;
    for(int i = 0; i < N_OBJ; i++){
        double cp_value = double(OBJS[i][0]) / double(OBJS[i][1]);
        cp_arr.push_back({i, cp_value});
    }
    sort(cp_arr.begin(), cp_arr.end(), cmp);
    // print_vector(cp_arr);

    // deduce already used obj weight and add value
    int cap = C;
    double val = 0;
    for (int i = 0 ; i < n.decision.size(); i++){
        if (n.decision[i] == 1){// It DID take this obj.
            val += OBJS[i][0];
            cap -= OBJS[i][1];
        }
    }

    // delete already used obj from cp_arr
    vector<int> cp_arr_aug;
    for (int i = 0 ; i < cp_arr.size(); i++){
        if (cp_arr[i].first >= n.decision.size()){
            cp_arr_aug.push_back(cp_arr[i].first);
        }
    }
    // print_vector(cp_arr_aug);

    // Calcualte fraction knapsnack problem
    int idx = 0;
    while(cap > 0){// TODO, asssume all objects have weight greate than zero
        int v = OBJS[cp_arr_aug[idx]][0];
        int w = OBJS[cp_arr_aug[idx]][1];
        if (cap >= w){// Can't take all object
            val += v;
            cap -= w;
        }
        else{ // Can only take partial of object
            double percent = double(cap) / double(w);
            val += percent*v;
            break;
        }
        idx++;
    }
    cout << val << endl;
    return val;
}



int main(void){
    //Load input file 
    cin >> C >> N_OBJ;
    for(int i = 0; i < N_OBJ; i++){
        vector<int> tmp(2);
        cin >> tmp[0] >> tmp[1];
        OBJS.push_back(tmp);
    }
    //print_vector(OBJS);
    
    // Init first two node
    vector<int> tmp1{1};
    Node n1 = {-1, tmp1};
    n1.ub = get_ub(n1);

    vector<int> tmp2{0};
    Node n2 = {-1, tmp2};
    n2.ub = get_ub(n2);
    
    //pair<int, Node> p1({n1.ub, n1});
    //pair<int, Node> p2({n2.ub, n2});
    PQ.push(make_pair(n1.ub, n1));
    PQ.push(make_pair(n2.ub, n2));

    return 0;
}