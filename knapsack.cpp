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
priority_queue <int> PQ; // for nodes that wait for famthom

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

struct Node{
    int ub;
    vector<int> decision;
};

bool cmp(pair<int, double>a, pair<int, double>b)
{
    return a.second > b.second;
}

vector<vector<int>> OBJS;
int main(void){
    //Load input file 
    cin >> C >> N_OBJ;
    for(int i = 0; i < N_OBJ; i++){
        vector<int> tmp(2);
        cin >> tmp[0] >> tmp[1];
        OBJS.push_back(tmp);
    }
    //print_vector(OBJS);

    //Get fraction-knapsanck order
    vector <pair<int, double>> cp_arr;
    for(int i = 0; i < N_OBJ; i++){
        double cp_value = double(OBJS[i][0]) / double(OBJS[i][1]);
        //pair<int, double> p(i, cp_value);
        cp_arr.push_back({i, cp_value});
    }
    sort(cp_arr.begin(), cp_arr.end(), cmp);
    // print_vector(cp_arr);

    //Calculate [0]
    // Calcualte upper bound 
    // delete decision's weight and add value
    vector<int> tmp{0};
    Node n1 = {-1, tmp};
    int cap = C;
    int val = 0;
    for (int i = 0 ; i < n1.decision.size(); i++){
        val += OBJS[i][0];
        cap -= OBJS[i][1];
    }

    vector<int> cp_arr_aug;
    for (int i = 0 ; i < cp_arr.size(); i++){
        if (cp_arr[i].first >= n1.decision.size()){
            cp_arr_aug.push_back(cp_arr[i].first);
        }
    }
    // print_vector(cp_arr_aug);

    // Calcualte fraction knapsnack problem 
    while(cap > 0){// TODO, i asssume all objects have weight greate than zero

        val // TOOD O
    }
    //Node()
    
    //PQ.push({4, 5});



    return 0;
}