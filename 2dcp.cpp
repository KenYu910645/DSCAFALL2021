#include <iostream>
#include <vector>
#include <algorithm> // for sort
#include <cmath>
#include <queue>
#include <limits>
using namespace std;

int N_P = 0; //Number of points
bool DEBUG = false;
long long d = std::numeric_limits<long long int>::max();
struct Point{
    int x; // value 
    int y; // weight
    int idx; // index of original input
};
vector<Point> P;
vector< pair<Point, Point> > d_pts;

void print_vector(vector<Point> p){
    cout << "[";
    for (int i = 0; i < p.size(); i++){
        cout << "(" << p[i].x << ", " << p[i].y << ", " <<p[i].idx << "), ";
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
bool cmp_idx(pair<Point, Point> a, pair<Point, Point> b){
    if (a.first.idx == b.first.idx){
        return a.second.idx < b.second.idx;
    }
    else{
        return a.first.idx < b.first.idx;
    }
}

void update_d(long long new_d, Point a, Point b){
    if (new_d < d){
        d = new_d;
        d_pts.clear();
    }
    if (new_d <= d){
        if (DEBUG)
            cout << "Update d = " << d << endl;
        // Add new pair to d_pts
        pair<Point, Point> new_pair;
        if (a.idx < b.idx){
            new_pair.first  = a;
            new_pair.second = b;
        }
        else{
            new_pair.first  = b;
            new_pair.second = a;
        }
        d_pts.push_back(new_pair);
    }
}

void find_cp(vector<int> p){
    /***********************
    *****  Base cases  *****
    ***********************/
    if (p.size() == 2){
        long long ans = pow((P[p[0]].x - P[p[1]].x), 2) + pow((P[p[0]].y - P[p[1]].y), 2);
        update_d(ans, P[p[0]], P[p[1]] );
        return;
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
        
        update_d(ans1, P[p[0]], P[p[1]] );
        update_d(ans2, P[p[0]], P[p[2]] );
        update_d(ans3, P[p[1]], P[p[2]] );
        return;
    }

    /*******************
    *****  Divide  *****
    ********************/
    // find median of these points
    vector<Point> p_recon;
    for (int i = 0 ; i < p.size(); i++){
        p_recon.push_back( P[ p[i] ] );
    }
    const auto median_it = p_recon.begin() + p_recon.size()/2;
    std::nth_element(p_recon.begin(), median_it , p_recon.end(), cmp_x);
    if (DEBUG){
        auto median = *median_it;
        cout << "median = (" << (*median_it).x  << ", " << (*median_it).y << ")" << endl;
    }

    // planning eco, calculate how many median should l_p take.
    int n_smaller_than_median = 0;
    for(int i = 0 ; i < p.size(); i++){
        if (P[ p[i] ].x < (*median_it).x)
            n_smaller_than_median++;
    }
    int n_median_that_l_p_should_take = (p.size()+1)/2 - n_smaller_than_median;

    vector<int> l_p, r_p;
    int tmp = 0;
    for(int i = 0 ; i < p.size(); i++){
        if (P[ p[i] ].x == (*median_it).x){
            tmp++;
        }

        if ( P[ p[i] ].x < (*median_it).x){ //and l_p.size() < (p.size()+1)/2 ){
            l_p.push_back( p[i] );
            if (DEBUG)
                cout << "l_p : (" << P[p[i]].x << ", " << P[p[i]].y << ")" << endl;
        }
        else if (P[ p[i] ].x == (*median_it).x){
            if (n_median_that_l_p_should_take > 0){
                l_p.push_back( p[i] );
                n_median_that_l_p_should_take --;
            }
            else{
                r_p.push_back( p[i] );
            }
        }
        else{
            if (P[ p[i] ].x < (*median_it).x){
                cout << P[ p[i] ].x <<" AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
            }
            r_p.push_back( p[i] );
            if (DEBUG)
                cout << "r_p : (" << P[p[i]].x << ", " << P[p[i]].y << ")" << endl;
        }
    }
    if (DEBUG){
        cout << "l_p.size() = " << l_p.size() << endl;
        cout << "r_p.size() = " << r_p.size() << endl;
    }

    /********************
    *****  Conquer  *****
    *********************/
    find_cp(l_p);
    find_cp(r_p);

    /******************
    *****  Merge  *****
    ******************/
    //Get left slab and right slab
    vector<int> l_slab, r_slab;
    // get l_slab
    for (int i = 0 ; i < l_p.size(); i++){
        if (pow(P[l_p[i]].x - (*median_it).x, 2) <= d){ // inside slab
            l_slab.push_back( l_p[i] );
        }
    }
    // get r_slab
    for (int i = 0 ; i < r_p.size(); i++){
        if (pow(P[r_p[i]].x - (*median_it).x, 2) <= d){ // inside slab
            r_slab.push_back( r_p[i] );
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
        //Reverse j to init position
        while(j > 0 and 
              (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) <= d or  // TODO not sure about the euqual sign //  
                   P[ r_slab[j] ].y - P[ l_slab[i] ].y < 0)) {
            j--;
        }
        //Scan thourgh the 2D region
        while((j < r_slab.size() and 
              (pow(P[ r_slab[j] ].y - P[ l_slab[i] ].y, 2) <= d or  // TODO not sure about the euqual sign //  
                   P[ r_slab[j] ].y - P[ l_slab[i] ].y > 0) )){
            ans = pow((P[ r_slab[j] ].x - P[ l_slab[i] ].x), 2) +
                  pow((P[ r_slab[j] ].y - P[ l_slab[i] ].y), 2);
            update_d(ans, P[r_slab[j]], P[l_slab[i]] );
            j++;
        }
        if (j != 0)
            j--; // prevent j = r_slab.size()
    }
    return;
}

int main (){
    //Load input file 
    cin >> N_P;
    for(int i = 0; i < N_P; i++){
        Point point = {0, 0, i+1};
        cin >> point.x >> point.y;
        P.push_back(point);
    }
    if (DEBUG){
        cout << "Number of Points = " << N_P << endl;
        print_vector(P);
    }
    // Sort by y-coordiante
    sort(P.begin(), P.end(), cmp_y);
    // print_vector(P);

    vector<int> l_it;
    for(int i = 0; i < P.size(); i++){
        l_it.push_back(i);
    }
    find_cp(l_it);
    cout << d << " " << d_pts.size() <<endl;

    // sort d_pts by first idx and second idx 
    sort(d_pts.begin(), d_pts.end(), cmp_idx);

    for (int i = 0 ; i < d_pts.size(); i++){
        cout << d_pts[i].first.idx << " " << d_pts[i].second.idx << endl;
    }

}