// Online judge
// http://140.112.28.213:4422/problem/1

// Sample Input 1
// AACEGGECAEF
// ACCEEGGECAFE
// Ans: 9 , ACEGGECAF

// Sample Input 2
// AACDCCFSEDA
// ABCDEADFSA

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <set>
#include <algorithm>
using namespace std;

bool DEBUG = false;

void print_table(std::vector<std::vector<int>> table){
    for (int i = 0; i < table.size(); i++){
        for (int j = 0; j < table[i].size(); j++){
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

// Global variable
set<string> myset;
vector<string> lcs_record;
vector<vector<int>> t_num;
vector<vector<int>> t_pre;
int lcs_len = 0;
string s1,s2;

void back_tracking(string ans ,int i, int j, string debug){
    // base case
    if ((ans.size() == lcs_len and lcs_len != 0)){
        lcs_record.push_back(ans);
        return;
    }
    // find break points
    for (int a = 1; a <= i; a++){
        for (int b = 1; b <= j; b++){
            if (t_num[a][b] == t_num[i][j] and s1[a] == s2[b]){
                back_tracking(s1[a] + ans, a-1, b-1, "");
            }
        }
    }
}

int main(void){
    if (DEBUG){
        // p1
        s1 = "AACEGGECAEF";
        s2 = "ACCEEGGECAFE";

        // p2
        // s1 = "AACDCCFSEDA";
        // s2 = "ABCDEADFSA";
        
        // p3
        // s1 = "JGEFDDFIJFIGJIG";
        // s2 = "IGHCEJCHGDBJIFAFIEGI";
        
        // s1 = "properties";
        // s2 = "prosperity";
        
        // s1 = "spider";
        // s2 = "spiderkiller";

        cout << "Input string 1 = " << s1 << endl;
        cout << "Input string 2 = " << s2 << endl;
    }
    else{
        cin >> s1 >> s2;
        //cout << "Input string 1 = " << s1 << endl;
        //cout << "Input string 2 = " << s2 << endl;
    }

    s1 = "$" + s1;
    s2 = "$" + s2;

    vector<vector<int>> a (s1.size(), vector<int>(s2.size(), 0));
    vector<vector<int>> b (s1.size(), vector<int>(s2.size(), 0));
    t_num = a;
    t_pre = b;

    for (int i = 0 ; i < s1.size(); i++){
        for (int j = 0 ; j < s2.size(); j++){
            //base case
            if (i == 0 or j == 0){
                t_num[i][j] = 0;
                continue;
            }
            
            //There're three choices
            if (s1[i] == s2[j]){
                int max_num = max( max(t_num[i-1][j-1] + 1, t_num[i-1][j]), t_num[i][j-1]);
                t_num[i][j] = max_num;
                //For back-tracking
                if (t_num[i-1][j-1] + 1 == max_num) // from up-left
                    t_pre[i][j] += 4;
                if (t_num[i-1][j] == max_num) // from up 
                    t_pre[i][j] += 2;
                if (t_num[i][j-1] == max_num) // from left
                    t_pre[i][j] += 1;
            }
            //There're two choices
            else{
                int max_num = max(t_num[i-1][j], t_num[i][j-1]);
                t_num[i][j] = max_num;
                //For back-tracking
                if (t_num[i-1][j] == max_num) // from up 
                    t_pre[i][j] += 2;
                if (t_num[i][j-1] == max_num) // from left
                    t_pre[i][j] += 1;
            }
        }
    }

    // back-tracking, DFS
    lcs_len = t_num[s1.size()-1][s2.size()-1];
    back_tracking("", s1.size()-1, s2.size()-1, "");
    std::sort(lcs_record.begin(), lcs_record.end());

    if (DEBUG){
        cout << "========== t_num =============" << endl;
        print_table(t_num);
        cout << "========== t_pre =============" << endl;
        print_table(t_pre);
        
        for(vector<string>::iterator it = lcs_record.begin(); it != lcs_record.end(); it++){
            cout << *it << endl;
        }

        cout << "Length of LCS: " << t_num[s1.size()-1][s2.size()-1] << endl;
        cout << "Number of LCS: " << lcs_record.size() << endl;
    }
    else{
        cout << lcs_len << " " << lcs_record.size() << endl;
        for(vector<string>::iterator it = lcs_record.begin(); it != lcs_record.end(); it++){
            cout << *it << endl;
        }
    }
    return 0;
}