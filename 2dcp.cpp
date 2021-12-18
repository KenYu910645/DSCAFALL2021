#include <iostream>
#include <vector>
#include <algorithm> // for sort
#include <cmath>
#include <queue>
using namespace std;

int N_P = 0; //Number of points
bool DEBUG = false;

struct Point{
    int x; // value 
    int y; // weight
};
vector<Point> P;

void print_vector(vector<Point> p){
    cout << "[";
    for (int i = 0; i < p.size(); i++){
        cout << "(" << p[i].x << ", " << p[i].y << "), ";
    }
    cout << "]" << endl;
}

void print_vector(vector<int> p){
    cout << "[";
    for (int i = 0; i < p.size(); i++){
        cout << p[i] << " ";
    }
    cout << "]" << endl;
}

bool cmp_x(Point a, Point b){
	return a.x > b.x;
}
bool cmp_y(Point a, Point b){
	return a.y > b.y;
}
long long find_cp(vector<int> p){
    // Base cases
    if (p.size() == 2){
        long long ans = pow((P[p[0]].x - P[p[1]].x), 2) + pow((P[p[0]].y - P[p[1]].y), 2);
        return ans;
    }
    if (p.size() == 3){
        int x1, y1, x2, y2, x3, y3;
        x1 = P[p[0]].x;
        y1 = P[p[0]].y;
        x2 = P[p[1]].x;
        y2 = P[p[1]].y;
        x3 = P[p[2]].x;
        y3 = P[p[2]].y;

        long long ans1 = pow((x2 - x1), 2) + pow((y2 - y1), 2);
        long long ans2 = pow((x3 - x1), 2) + pow((y3 - y1), 2);
        long long ans3 = pow((x3 - x2), 2) + pow((y3 - y2), 2);
        
        return min({ans1, ans2, ans3});
    }

    // Divide, find median of these points
    vector<Point> p_recon;
    for (int i = 0 ; i < p.size(); i++){
        p_recon.push_back( P[ p[i] ] );
    }
    const auto median_it = p_recon.begin() + p_recon.size()/2;
    std::nth_element(p_recon.begin(), median_it , p_recon.end(), cmp_x);
    // auto median = *median_it;
    // cout << (*median_it).x  << ", " << (*median_it).y << endl;

    vector<int> l_p, r_p;
    for(int i = 0 ; i < p.size(); i++){
        if ( P[ p[i] ].x <= (*median_it).x and l_p.size() < (p.size()+1)/2 ){
            l_p.push_back( p[i] );
            //cout << "l_p : (" << P[p[i]].x << ", " << P[p[i]].y << ")" << endl;
        }
        else{
            r_p.push_back( p[i] );
            //cout << "r_p : (" << P[p[i]].x << ", " << P[p[i]].y << ")" << endl;
        }
    }
    if (DEBUG){
        cout << "l_p.size() = " << l_p.size() << endl;
        cout << "r_p.size() = " << r_p.size() << endl;
    }

    long long l_cp = find_cp(l_p);
    long long r_cp = find_cp(r_p);
    if (DEBUG){
        cout << "l_cp = " << l_cp << endl;
        cout << "r_cp = " << r_cp << endl;
    }

    
    /******************
    *****  Merge  *****
    ******************/
    long long d = min(l_cp, r_cp);

    vector<int> l_slab, r_slab;
    for (int i = 0 ; i < p.size(); i++){
        int dis_2d = P[p[i]].x - (*median_it).x;
        if (pow(dis_2d, 2) <= d){ // inside slab
            if (dis_2d <= 0 and l_slab.size() < (p.size()+1)/2  ){// left slab
                l_slab.push_back( p[i] );
            }
            else{ // right slab
                r_slab.push_back( p[i] );
            }
        }
    }
    if (DEBUG){
        cout << "l_slab.size() = " << l_slab.size() << endl;
        cout << "r_slab.size() = " << r_slab.size() << endl;
    }

    // These should be sorted array
    // print_vector(l_slab); 
    // print_vector(r_slab);

    long long ans;
    int j = 0; // idx in right slab
    for (int i = 0 ; i < l_slab.size(); i++){
        // cout << " r_slab[j] = " << r_slab[j] << endl;
        //cout << "d = " << d  << endl;
        
        // int dis = P[ r_slab[j] ].y - P[ l_slab[i] ].y;
        // long long dis_2 = pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2);

        //Reverse j to init position
        // while(j > 0 and 
        //       pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) <= d and 
        //           P[ r_slab[j] ].y - P[ l_slab[i] ].y < 0){
        //     j--;
        // }
        // while(true){
        //     if (j == 0 or 
        //         (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) > d and  // TODO not sure about the euqual sign //  
        //              P[ r_slab[j] ].y - P[ l_slab[i] ].y >= 0))
        //         break;
        //     j--;
        //

        while(j > 0 and 
              (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) <= d or  // TODO not sure about the euqual sign //  
                   P[ r_slab[j] ].y - P[ l_slab[i] ].y < 0)) {
                break;
            j--;
        }
        int tmp = j;
        // start scan 2d block 
        // while(true){
        //     if (j == r_slab.size() or 
        //         (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) > d and  // TODO not sure about the euqual sign //  
        //              P[ r_slab[j] ].y - P[ l_slab[i] ].y <= 0) )
        //         break;
        while((j < r_slab.size() and 
              (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) <= d or  // TODO not sure about the euqual sign //  
                   P[ r_slab[j] ].y - P[ l_slab[i] ].y > 0) )){
            
            // cout << "P[ r_slab[j] ].y = " << P[ r_slab[j] ].y << endl;
            // cout << "P[ l_slab[i] ].y = " << P[ l_slab[i] ].y << endl;
            ans = pow((P[ r_slab[j] ].x - P[ l_slab[i] ].x), 2) +
                  pow((P[ r_slab[j] ].y - P[ l_slab[i] ].y), 2);
            d = min(d, ans);
            j++;
        }
        j--; // prevent j = r_slab.size()
        // cout << "scan_count = " << j - tmp << endl;
    }
    return d;
}

int main (){
    //Load input file 
    cin >> N_P;
    for(int i = 0; i < N_P; i++){
        Point point = {0, 0};
        cin >> point.x >> point.y;
        P.push_back(point);
    }
    cout << "Number of Points = " << N_P << endl;
    // Sort by y-coordiante
    sort(P.begin(), P.end(), cmp_y);
    // print_vector(P);
    
    vector<int> l_it;
    for(int i = 0; i < P.size(); i++){
        l_it.push_back(i);
    }

    cout << find_cp(l_it) << endl;

}