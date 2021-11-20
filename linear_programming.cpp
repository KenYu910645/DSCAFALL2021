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
using namespace std;

bool DEBUG = false;
int N_FORMULAR = 0;
double ETA = 1e-10; //  Can't smaller than 2.220446049 * 10^-16
double SLOPE_DUMMY = 1e-15; // to avoid inf slope
string INPUT_FILE = "2DLP/fuck.in";
vector<vector<int>> FORMULARS;
string ANS = "";

struct Formular{
    double a;
    double b;
    double c;
    int idx; //my idx in FORMULARS
};
vector<Formular> I_a; // b > 0, face down
vector<Formular> I_m; // b < 0, face up 

struct Intersection{
    double x;
    double y;
    Formular f1; // tow formular that intersected
    Formular f2;
    int g_idx; // group id
};

void print_vector(vector<int> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << x[i] << ' '; 
    }
    cout << "]" << endl;
}

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

void print_vector(vector<pair<int,int>> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << "(" << x[i].first << ", " << x[i].second << ")";
        if (i != x.size()-1)
            cout << ", "; 
    }
    cout << "]" << endl;
}

void print_vector(vector<Formular> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << "(" << x[i].a << ", " << x[i].b << ", " << x[i].c << ", " << x[i].idx << ")";
        if (i != x.size()-1)
            cout << ", "; 
    }
    cout << "]" << endl;
}

void print_vector(vector<Intersection> x){
    cout << "[";
    for (int i = 0; i < x.size(); i++){
        cout << "(" << x[i].x << ", " << x[i].y << ", " << x[i].f1.idx << ", " << x[i].f2.idx << ")";
        if (i != x.size()-1)
            cout << ", "; 
    }
    cout << "]" << endl;
}

bool cmp(Intersection a, Intersection b){
    return (a.x < b.x); 
}

int which_line_prune(Intersection c, int step, bool prune_upper){
    double y1 = (c.f1.a*(c.x + step) + c.f1.c)/double(-c.f1.b);
    double y2 = (c.f2.a*(c.x + step) + c.f2.c)/double(-c.f2.b);
    // delete upper
    if (prune_upper){
        if (y1 >= y2)
            return c.f1.idx;
        else
            return c.f2.idx;
    }
    // delete lower
    else{
        if (y1 >= y2)
            return c.f2.idx;
        else
            return c.f1.idx;
    }
}

// int which_line_prune_vertical(Intersection c){
//     double x1 = -c.f1.c/double(c.f1.a);
//     double x2 = -c.f2.c/double(c.f2.a);
//     if (x1 >= x2) // prune the bigger x value line
//         return c.f1.idx;
//     else
//         return c.f2.idx;
// }

int sign(double x){
    if (abs(x - 0.0) < ETA)
        return 0;
    else if (x > 0.0)
        return 1;
    else
        return -1;
}

int main(void){
    /*******************/
    /**** Get Input ****/
    /*******************/
    if (DEBUG){
        string str;
        ifstream ifs (INPUT_FILE, ifstream::in); 
        while (getline(ifs, str)){
            string::size_type end;
            end = str.find(" ");
            if (end != string::npos) { // it's a formular 
                // Split -257 -255 120 to [-257, -255, 120]
                stringstream ss(str);
                string tmp_s;
                vector<int> tmp_line;
                while (getline(ss, tmp_s, ' ')) {
                    tmp_line.push_back(stoi(tmp_s));
                }
                FORMULARS.push_back(tmp_line);
            }
            else{ // it's a number 
                N_FORMULAR = stoi(str);
            }
        }
        ifs.close();

        //print what I read from input file.
        cout << "Number of Formular = " << N_FORMULAR << endl;
    }
    else{
        cin >> N_FORMULAR;
        for(int i = 0; i < N_FORMULAR; i++){
            vector<int> tmp(3);
            cin >> tmp[0] >> tmp[1] >> tmp[2];
            FORMULARS.push_back(tmp);
        }
    }
    
    /***************************/
    /**** Reverse sign of C ****/
    /***************************/
    for (int i = 0; i < FORMULARS.size(); i++){
        FORMULARS[i][2] = -FORMULARS[i][2];
    }

    if (DEBUG){
        cout << "======== FORMULARS =========== " << endl;
        print_vector(FORMULARS); 
        cout << "=================== " << endl;
    }
    
    int loop_time = 0;
    while (true){
        /************************/
        /**** Spilt to Group ****/
        /************************/
        I_a.clear();
        I_m.clear();
        // vector<Formular> I_0; // b = 0
        for (int i = 0; i < FORMULARS.size(); i++){
            Formular fm = {double(FORMULARS[i][0]),
                           double(FORMULARS[i][1]),
                           double(FORMULARS[i][2]), i};
            //Dirty job
            if (fm.b == 0.0)
                fm.b = SLOPE_DUMMY;
            //
            if (fm.b > 0)
                I_a.push_back(fm);
            else if (fm.b < 0)
                I_m.push_back(fm);
            else{ //fm.b == 0
                // I_0.push_back(fm);
                cout << "This is impossible. SLOPE_DUMMY didn't do its job." << endl;
            }
        }
        vector<vector<Formular>> I_group;// [I_a, I_m, I_0]
        I_group.push_back(I_a);
        I_group.push_back(I_m);
        //I_group.push_back(I_0);
        if (DEBUG){
            cout << "=========== I_a =============" << endl;
            print_vector(I_group[0]);
            cout << "=========== I_m =============" << endl;
            print_vector(I_group[1]);
            // cout << "=========== I_0 =============" << endl;
            // print_vector(I_group[2]);
            cout << "=============================" << endl;
        }

        /***************************/
        /**** Find Intersection ****/
        /***************************/
        // Find intersection between two formular
        //reference https://www.cuemath.com/geometry/intersection-of-two-lines/
        vector<Intersection> inter_vector; // intersections vector
        vector<int> prune_list;
        for (int g_idx = 0; g_idx < I_group.size(); g_idx++){
            for (int i = 1; i < I_group[g_idx].size(); i+=2){
                Formular f1 = I_group[g_idx][i-1];
                Formular f2 = I_group[g_idx][i  ];
                double x = (f1.b*f2.c - f2.b*f1.c) / double(f1.a*f2.b - f2.a*f1.b);
                double y = (f1.c*f2.a - f2.c*f1.a) / double(f1.a*f2.b - f2.a*f1.b);
                Intersection inter = {x, y, f1, f2, g_idx};

                // Deal with parallel lines
                if (double(f1.a*f2.b - f2.a*f1.b) == 0.0){
                    if (g_idx == 0){ // I_a, face down. prune upper line
                        prune_list.push_back(which_line_prune(inter, 0, true));
                    }
                    else if (g_idx == 1){ // I_m, face up, prune lower line
                        prune_list.push_back(which_line_prune(inter, 0, false));
                    }
                    else{ // I_0, the line is facing left.
                        cout << "This is impossible. SLOPE_DUMMY didn't do its job." << endl;
                    }
                }
                else{
                    inter_vector.push_back(inter);
                }
                
            }
        }

        if (DEBUG){
            cout << "========== inter_vector ============" << endl;
            print_vector(inter_vector);
        }

        // if no intersection at all
        if (inter_vector.size() != 0){

            /*************************/
            /**** Find median(xm) ****/
            /*************************/
            size_t n = inter_vector.size() / 2;
            nth_element(inter_vector.begin(), inter_vector.begin()+n, inter_vector.end(), cmp);
            Intersection median = inter_vector[n];
            double xm = median.x;
            if (DEBUG)
                cout << "xm = " << xm << endl;
            
            /**********************************/
            /**** Find X* relative to Xm  ****/
            /*********************************/
            double alpha_x, alpha_y, beta_x, beta_y, s_max, s_min, t_max, t_min;
            alpha_x = xm;
            beta_x = xm;
            alpha_y = numeric_limits<double>::lowest();
            beta_y = numeric_limits<double>::max();
            s_max = numeric_limits<double>::lowest();
            s_min = numeric_limits<double>::max();
            t_max = numeric_limits<double>::lowest();
            t_min = numeric_limits<double>::max();
            // Get alpha_y
            for (int i = 0; i < I_group[1].size(); i++){
                int a = I_group[1][i].a;
                int b = I_group[1][i].b;
                int c = I_group[1][i].c;
                double p = (-a*xm-c)/double(b);
                if (p > alpha_y){
                    alpha_y = p;
                }
            }

            // Get beta_y
            for (int i = 0; i < I_group[0].size(); i++){
                int a = I_group[0][i].a;
                int b = I_group[0][i].b;
                int c = I_group[0][i].c;
                double p = (-a*xm-c)/double(b);
                if (p < beta_y){
                    beta_y = p;
                }
            }
            if (DEBUG){
                cout << "alpha = (" << alpha_x << ", " << alpha_y << ')' << endl;
                cout << "beta = (" << beta_x << ", " << beta_y << ')' << endl;
            }

            //Get s_max, s_min in I_m
            for (int i = 0; i < I_group[1].size(); i++){
                int a = I_group[1][i].a;
                int b = I_group[1][i].b;
                int c = I_group[1][i].c;
                // check if intersect with alpha
                // cout << "abs(alpha_y - (-a*xm-c)/double(b)) = " << abs(alpha_y - (-a*xm-c)/double(b)) << endl; 
                if (abs(alpha_y - (-a*xm-c)/double(b)) < ETA){
                    double slope = -a/double(b);
                    if (slope > s_max)
                        s_max = slope;
                    if (slope < s_min)
                        s_min = slope;
                }
            }
            //Get t_max, t_min
            for (int i = 0; i < I_group[0].size(); i++){
                int a = I_group[0][i].a;
                int b = I_group[0][i].b;
                int c = I_group[0][i].c;
                // check if intersect with beta
                if (abs(beta_y - (-a*xm-c)/double(b)) < ETA){
                    double slope = -a/double(b);
                    if (slope > t_max)
                        t_max = slope;
                    if (slope < t_min)
                        t_min = slope;
                }
            }

            if (DEBUG){
                cout << "(s_min, s_max) = (" << s_min << ", " << s_max << ')' << endl;
                cout << "(t_min, t_max) = (" << t_min << ", " << t_max << ')' << endl;
            }


            string direction = "None";
            //Case 1:
            if      ( (alpha_y <= beta_y) and (s_min <= s_max) and (sign(s_max) == -1) ){
                direction = "xm<x*";
            }
            //Case 2:
            else if ( (alpha_y <= beta_y) and (s_max >= s_min) and (sign(s_min) == 1) ){
                direction = "x*<xm";
            }
            //Case 3:
            else if ( (alpha_y <= beta_y) and (sign(s_min) <= 0) and (sign(s_max) >= 0) ){
                direction = "xm=x*";
            }
            //Case 4:
            else if ( (alpha_y > beta_y) and (s_max < t_min) ){
                direction = "xm<x*";
            }
            //Case 5:
            else if ( (alpha_y > beta_y) and (s_min > t_max) ){
                direction = "x*<xm";
            }
            //Case 6:
            else if ( (alpha_y > beta_y) and (s_max >= t_min) and (s_min <= t_max) ){
                direction = "NA";
            }
            else{
                // cout << "direction is not define, something wrong happend" << endl; // TODO 
                ANS = "-INF";
            }
            if (DEBUG)
                cout << "direction = " << direction << endl;
            /************************************/
            /**** Find Formular that need to prune  ****/
            /************************************/
            
            if (direction == "xm=x*"){
                ANS = to_string(median.y);
            }
            else if (direction == "NA"){
                ANS = "NA";
            }
            else if (direction == "x*<xm"){
                for (int i = 0 ; i < inter_vector.size(); i++){
                    if (inter_vector[i].x >= xm){ //prune intersection on xm's RHS
                        if (inter_vector[i].g_idx == 1){ // I_m, face up
                            /************* Case A ***************/
                            prune_list.push_back(which_line_prune(inter_vector[i], 1, true));
                        }
                        else if (inter_vector[i].g_idx == 0){ // I_a, face down
                            /************* Case C ***************/
                            prune_list.push_back(which_line_prune(inter_vector[i], 1, false));
                        }
                        else{
                            cout << "This is impossible. SLOPE_DUMMY didn't do its job." << endl;
                        }
                    }
                }
            }
            else if (direction == "xm<x*"){
                for (int i = 0 ; i < inter_vector.size(); i++){
                    if (inter_vector[i].x <= xm){ //prune intersection on xm's LHS
                        if (inter_vector[i].g_idx == 1){ // I_m, face up
                            /************* Case B ***************/
                            prune_list.push_back(which_line_prune(inter_vector[i], -1, true));
                        }
                        else if (inter_vector[i].g_idx == 0){ // I_a, face down
                            /************* Case D ***************/
                            prune_list.push_back(which_line_prune(inter_vector[i], -1, false));
                        }
                        else{
                            cout << "This is impossible. SLOPE_DUMMY didn't do its job." << endl;
                        }
                    }
                }
            }
        }
        else{ // inter_vector.size() == 0
            if (DEBUG)
                cout << "No intersection found, reduced to base case."  << endl;
        }
        /*************************/
        /**** Prune Formular  ****/
        /*************************/
        if (DEBUG){
            cout << "================= prune list =====================" << endl;
            print_vector(prune_list);
        }

        for (int i = 0 ; i < prune_list.size(); i++){
            FORMULARS[ prune_list[i] ].clear();
        }
        vector<vector<int>> new_formulars;
        for (int i = 0 ; i < FORMULARS.size(); i++){
            if (FORMULARS[i].size() != 0){
                new_formulars.push_back(FORMULARS[i]);
            }
        }
        FORMULARS = new_formulars;
        loop_time++;
        if (DEBUG){
            cout << "================= updated fomular =====================" << endl;
            print_vector(FORMULARS);
            cout << "***************************************** Done " << loop_time << " Loop ******************************" << endl;
        }

        // Terminate condition 
        if (prune_list.size() == 0) // Can't prune anymore
            break;
    } // END OF WHILE LOOP

    if (ANS == ""){
        if (DEBUG){
            for (int i = 0; i < FORMULARS.size(); i++)
                cout << FORMULARS[i][0] << "x+" << FORMULARS[i][1] << "y+" << FORMULARS[i][2] << "=0" << endl;
        }

        /*****************/
        /*** Base Case ***/
        /*****************/
        if (FORMULARS.size() == 1){ // Only one line remind
            Formular f = {double(FORMULARS[0][0]), 
                          double(FORMULARS[0][1]), 
                          double(FORMULARS[0][2]), -1};
            double slope = -f.a/double(f.b);
            if (slope == 0 and f.b <= 0){ // If this is a horizontal line and face up // TODO equal to 0???
                ANS = to_string(-f.c/double(f.b));
            }
            else{
                ANS = "-INF";
            }
        }
        else if (I_a.size() == 1 and I_m.size() == 1){
            Formular f1 = I_a[0]; // face down
            Formular f2 = I_m[0]; // face up
            double x = (f1.b*f2.c - f2.b*f1.c) / double(f1.a*f2.b - f2.a*f1.b);
            double y = (f1.c*f2.a - f2.c*f1.a) / double(f1.a*f2.b - f2.a*f1.b);
            Intersection inter = {x, y, f1, f2, -1};
            // Deal with parallel lines
            if (double(f1.a*f2.b - f2.a*f1.b) == 0.0){
                cout << "Paralled at base case. I'm not even sure this is possible at all!" << endl;
                ANS = "NA";
            }
            else{
                Formular real_lower_bound;
                if (which_line_prune(inter, 1, true) == f1.idx) // feasible region is on RHS
                    real_lower_bound = f1;
                else // feasible region is on LHS
                    real_lower_bound = f2;
                float real_lower_bound_slope = -real_lower_bound.a/double(real_lower_bound.b);
                
                if (real_lower_bound.idx == f1.idx){ //RHS
                    if (real_lower_bound_slope < 0)
                        ANS = "-INF";
                    else
                        ANS = to_string(inter.y);
                }
                else{ // LHS
                    if (real_lower_bound_slope > 0)
                        ANS = "-INF";
                    else
                        ANS = to_string(inter.y);
                }
            }
        }
        else
            ANS = "-INF"; // FORMULARS.size() == 0
    }
    
    if (DEBUG){
        cout << "ANS = " << ANS << endl;
        if (ANS == "NA")
            cout << "Round ANS = " << ANS << endl;
        else
            cout << "Round ANS = " << round(stof(ANS)) << endl;
    }
    else{
        if (ANS == "NA" or ANS == "-INF")
            cout << ANS << endl;
        else
            cout << round(stof(ANS)) << endl;
    }
    return 0;
}