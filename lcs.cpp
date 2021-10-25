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

using namespace std;


void print_table(std::vector<std::vector<int>> table){
    for (int i = 0; i < table.size(); i++){
        for (int j = 0; j < table[i].size(); j++){
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

int main(void){
    // char s1[100];
    // char s2[100];
    // cin.getline(s1, 100);
    // cin.getline(s2, 100);

    string s1("AACEGGECAEF");
    string s2("ACCEEGGECAFE");
    cout << s1 << endl;
    cout << s2 << endl;

    s1 = "$" + s1;
    s2 = "$" + s2;

    vector<vector<int>> t_num(s1.size(), vector<int>(s2.size(), 0));
    vector<vector<int>> t_pre(s1.size(), vector<int>(s2.size(), 0));

    for (int i = 0 ; i < s1.size(); i++){
        for (int j = 0 ; j < s2.size(); j++){
            //base case
            if (i == 0 or j == 0){
                t_num[i][j] = 0;
                continue;
            }
            
            //There're three choices
            if (s1[i] == s2[j]){
                t_num[i][j] = t_num[i-1][j-1] + 1;
            }
            else{
                t_num[i][j] = max(t_num[i-1][j], t_num[i][j-1]);
            }
        }
    }

    cout << "========== t_num =============" << endl;
    print_table(t_num);
    cout << "========== t_pre =============" << endl;
    print_table(t_pre);


    return 0;
}
