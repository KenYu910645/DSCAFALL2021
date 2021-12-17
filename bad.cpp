#include <iostream>
#include <queue>
#include <algorithm>
#include<cmath>
#define MAXN 1005
using namespace std;
 
typedef struct item{
	int value;
	int weight;
}Item;
 
struct Node{
	int level;
	int profit;
	float bound;
	int weight;
	Node(int a = 0, int b = 0, float c = 0.0, int d = 0.0):
	level(a), profit(b), bound(c), weight(d) {}
};
 
Item items[MAXN];
int W, N;
 
bool operator<(Node a, Node b){
	if(a.bound == b.bound){
		return a.level < b.level;
	}
	else{
		return a.bound > b.bound;
	}
}
 
bool cmp(Item a, Item b){
	return (float)a.value/a.weight > (float)b.value/b.weight;
}
 
int bound(Node v){
	if(v.weight >= W){
		return 0;
	}
	int profit_bound = v.profit;
	int j = v.level + 1;
	int totweight = v.weight;
 
	while((j < N) && (totweight + items[j].weight <= W)){
		totweight += items[j].weight;
		profit_bound += items[j].value;
		j++;
	}
 
	if(j < N){
		profit_bound += (W - totweight) * items[j].value / items[j].weight;
	}
	return profit_bound;
}
 
int knapsack(){
	sort(items, items+N, cmp);
 
	priority_queue<Node> QQ;
	Node u, v;
	u.level = -1;
	u.profit = 0;
	u.weight = 0;
	QQ.push(u);
 
	int maxprofit = 0;
    int count = 0;
	while(!QQ.empty()){
        count++;
		u = QQ.top();
		QQ.pop();
 
		if(u.level == -1){
			v.level = 0;
		}
 
		if(u.level == N-1){
			continue;
		}
 
		v.level = u.level + 1;
 
		v.weight = u.weight + items[v.level].weight;
		v.profit = u.profit + items[v.level].value;
 
		if(v.weight <= W && v.profit >= maxprofit){
			maxprofit = v.profit;
	//int profit_bound = v.profit;
			int j = v.level + 1;
			int totweight = v.weight;
 
			while((j < N) && (totweight + items[j].weight <= W)){
				totweight += items[j].weight;
				maxprofit += items[j].value;
				j++;
			}
		}
 
		v.bound = bound(v);
 
		if(v.bound > maxprofit){
			QQ.push(v);
		}
 
		v.weight = u.weight;
		v.profit = u.profit;
		v.bound = bound(v);
		if(v.bound > maxprofit){
			QQ.push(v);
		}
	}
    cout << count/double(pow(2, N)) << endl;
	return maxprofit;
 
}
 
int main(void){
	scanf("%d%d", &W, &N);
	for(int i = 0; i < N; i++){
		scanf("%d%d", &items[i].value, &items[i].weight);
	}
	printf("%d\n", knapsack());
 
	return 0;
}