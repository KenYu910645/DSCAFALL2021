#include <vector>
#include <iostream>
#include <float.h>
#include <algorithm>
#include <math.h>
using namespace std;

double epsilon = 1e-0;

vector<vector<double> > lines;

vector<vector<double> > I0;
vector<vector<double> > Iminus;
vector<vector<double> > Iplus;

vector<vector<double> > pointm_set;

double xm;

double ymin;

double u1 = -DBL_MAX;
double u2 = DBL_MAX;

int condition_no = -1;  //0:xm < x*, 1:x* < xm, 2:xm = x*, 3:no feasible region

int linesnum = 0; 

int isterminate = 0;


void input(){
	cin >> linesnum;
	for(int i = 0; i < linesnum; i++){
		vector<double> tmp(3);
		cin >> tmp[0] >> tmp[1] >> tmp[2];
		lines.push_back(tmp);
	}

	return;
}

void classify_I(){
	for(vector<vector<double> >::iterator it= lines.begin(); it != lines.end(); it++){
		if((*it)[1] > 0){
			Iplus.push_back((*it));
		}
		else if((*it)[1] < 0){
			Iminus.push_back((*it));
		}
		else{
			I0.push_back((*it));
		}
	}
	return;
}


void x_bound(){
	for(vector<vector<double> >::iterator it= I0.begin(); it != I0.end(); it++){
		if((*it)[0] > 0){
			if(((*it)[2]/(*it)[0]) < u2){
				u2 = (*it)[2]/(*it)[0];
			}
		}
		else if((*it)[0] < 0){
			if(((*it)[2]/(*it)[0]) > u1){
				u1 = (*it)[2]/(*it)[0];
			}
		}
	}
	return;
}

void partion_Iminus_and_Iplus(){
	//vector<vector<double> > Iplus_tmp;
	for(vector<vector<double> >::iterator it= Iplus.begin(); it+1 < Iplus.end(); it += 2){
		if((*it)[0]/(*it)[1] == (*(it+1))[0]/(*(it+1))[1]){  //two lines in parallel
			if ((*it)[2]/(*it)[1] <= (*(it+1))[2]/(*(it+1))[1]){
				//Iplus_tmp.push_back((*it));
				(*(it+1))[0] = 0.0;
				(*(it+1))[1] = 0.0;
				(*(it+1))[2] = 0.0;
			}
			else{
				//Iplus_tmp.push_back((*(it+1)));
				(*it)[0] = 0.0;
				(*it)[1] = 0.0;
				(*it)[2] = 0.0;
			}
		}
		else{  //two lines intersect
			double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
			double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
			vector<double> point(2);
			point[0] = x;
			point[1] = y;
			if(point[0] < u1){     //out of boundary
				if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
					//Iplus_tmp.push_back((*(it+1)));
					(*it)[0] = 0.0;
					(*it)[1] = 0.0;
					(*it)[2] = 0.0;
				}
				else{
					//Iplus_tmp.push_back((*it));
					(*(it+1))[0] = 0.0;
					(*(it+1))[1] = 0.0;
					(*(it+1))[2] = 0.0;
				}
			}
			else if(point[0] > u2){    //out of boundary
				if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
					//Iplus_tmp.push_back((*it));
					(*(it+1))[0] = 0.0;
					(*(it+1))[1] = 0.0;
					(*(it+1))[2] = 0.0;					
				}
				else{
					//Iplus_tmp.push_back((*(it+1)));
					(*it)[0] = 0.0;
					(*it)[1] = 0.0;
					(*it)[2] = 0.0;					
				}
			}
			else{
				pointm_set.push_back(point);
				//Iplus_tmp.push_back((*it));
				//Iplus_tmp.push_back((*(it+1)));
			}
		}
	}
	/*
	if(Iplus_tmp.size() > 0){
		if(Iplus.size()%2==1){
			Iplus_tmp.push_back(Iplus[Iplus.size()-1]);
		}
		Iplus.clear();
		Iplus.assign(Iplus_tmp.begin(), Iplus_tmp.end());
	}
	else{
		printf("no happen!\n");
	}
	*/
	//vector<vector<double> > Iminus_tmp;
	for(vector<vector<double> >::iterator it= Iminus.begin(); it+1 < Iminus.end(); it += 2){
		if((*it)[0]/(*it)[1] == (*(it+1))[0]/(*(it+1))[1]){  //two lines in parallel
			if ((*it)[2]/(*it)[1] <= (*(it+1))[2]/(*(it+1))[1]){
				//Iminus_tmp.push_back((*(it+1)));
				(*it)[0] = 0.0;
				(*it)[1] = 0.0;
				(*it)[2] = 0.0;
			}
			else{
				//Iminus_tmp.push_back((*it));
				(*(it+1))[0] = 0.0;
				(*(it+1))[1] = 0.0;
				(*(it+1))[2] = 0.0;
			}
		}
		else{  //two lines intersect
			double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
			double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
			vector<double> point(2);
			point[0] = x;
			point[1] = y;
			if(point[0] < u1){     //out of boundary
				if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
					//Iminus_tmp.push_back((*it));
					(*(it+1))[0] = 0.0;
					(*(it+1))[1] = 0.0;
					(*(it+1))[2] = 0.0;
				}
				else{
					//Iminus_tmp.push_back((*(it+1)));
					(*it)[0] = 0.0;
					(*it)[1] = 0.0;
					(*it)[2] = 0.0;					
				}
			}
			else if(point[0] > u2){    //out of boundary
				if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
					//Iminus_tmp.push_back((*(it+1)));
					(*it)[0] = 0.0;
					(*it)[1] = 0.0;
					(*it)[2] = 0.0;
				}
				else{
					//Iminus_tmp.push_back((*it));
					(*(it+1))[0] = 0.0;
					(*(it+1))[1] = 0.0;
					(*(it+1))[2] = 0.0;
				}
			}
			else{
				pointm_set.push_back(point);
				//Iminus_tmp.push_back((*it));
				//Iminus_tmp.push_back((*(it+1)));
			}
		}
	}
	/*
	if(Iminus_tmp.size() > 0){
		if(Iminus.size()%2==1){
			Iminus_tmp.push_back(Iminus[Iminus.size()-1]);
		}
		Iminus.clear();
		Iminus.assign(Iminus_tmp.begin(), Iminus_tmp.end());
	}
	else{
		printf("no happen!\n");
	}
	*/

	return;
}

bool funccmp(vector<double> p1, vector<double> p2){
	return p1[0] < p2[0];
}

void find_x_median(){
	sort(pointm_set.begin(), pointm_set.end(), funccmp);
	xm = pointm_set[pointm_set.size()/2][0];
	//printf("pointm_set.size()/2 = %lu\n", pointm_set.size()/2);
	pointm_set.clear();
	return;
}


void shrink_x_range(){
	vector<double> alpha(2);
	vector<double> beta(2);
	alpha[0] = xm;
	beta[0] = xm;

	alpha[1] = -DBL_MAX;
	for(vector<vector<double> >:: iterator it = Iminus.begin(); it != Iminus.end(); it++){
		if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){
			double tmp = ((*it)[2] - (*it)[0]*alpha[0])/(*it)[1];
			if(tmp > alpha[1]){
				alpha[1] = tmp;
			}
		}
	}
	/*
	cout << "alpha:" << endl;
	cout << alpha[0] << "," << alpha[1] << endl;
	*/
	double Smax = -DBL_MAX;
	double Smin = DBL_MAX;

	for(vector<vector<double> >:: iterator it = Iminus.begin(); it != Iminus.end(); it++){
		if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){	
			//printf("a = %f, b = %f, c = %f\n", (*it)[0], (*it)[1], (*it)[2]);	
			if(((-(*it)[0])/(*it)[1]) > Smax && (fabs((*it)[0]*alpha[0]+(*it)[1]*alpha[1] - (*it)[2]) < epsilon)){
				Smax = (-(*it)[0])/(*it)[1];
				//printf("a = %f, b = %f, c = %f\n", (*it)[0], (*it)[1], (*it)[2]);
			}
			if(((-(*it)[0])/(*it)[1]) < Smin && (fabs((*it)[0]*alpha[0]+(*it)[1]*alpha[1] - (*it)[2]) < epsilon)){
				Smin = (-(*it)[0])/(*it)[1];
				//printf("a = %f, b = %f, c = %f\n", (*it)[0], (*it)[1], (*it)[2]);
			}
		}
	}

	//printf("Smax = %f, Smin = %f\n", Smax, Smin);

	beta[1] = DBL_MAX;
	for(vector<vector<double> >:: iterator it = Iplus.begin(); it != Iplus.end(); it++){
		if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){	
			double tmp = ((*it)[2] - (*it)[0]*beta[0])/(*it)[1];
			if(tmp < beta[1]){
				beta[1] = tmp;
			}
		}
	}

	double Tmax = -DBL_MAX;
	double Tmin = DBL_MAX;

	for(vector<vector<double> >:: iterator it = Iplus.begin(); it != Iplus.end(); it++){
		if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){
			//printf("a = %f, b = %f, c = %f\n", (*it)[0], (*it)[1], (*it)[2]);
			if(((-(*it)[0])/(*it)[1]) > Tmax && (fabs((*it)[0]*beta[0]+(*it)[1]*beta[1] - (*it)[2]) < epsilon)){
				Tmax = (-(*it)[0])/(*it)[1];
			}
			if(((-(*it)[0])/(*it)[1]) < Tmin && (fabs((*it)[0]*beta[0]+(*it)[1]*beta[1] - (*it)[2]) < epsilon)){
				Tmin = (-(*it)[0])/(*it)[1];
			}
		}
	}

	if(alpha[1] <= beta[1]){
		if(Smax < 0){    //xm < x*
			u1 = xm;
			condition_no = 0;
		}
		if(Smin > 0){    //x* < xm
			u2 = xm;
			condition_no = 1;
		}
		if(Smin <= 0 && 0 <= Smax){    //xm = x*
			xm = alpha[0];
			ymin = alpha[1];
			condition_no = 2;

		}
	}
	else{
		if(Smax < Tmin){    //xm < x* if feasible region exist
			u1 = xm;    //feasible region exist?
			condition_no = 0;
		}
		if(Smin > Tmax){    //x* < xm if feasible region exist
			u2 = xm;    //feasible region exist?
			condition_no = 1;
		}
		if(Smax >= Tmin && Smin <= Tmax){      //no feasible region
			condition_no = 3;
		}
	}

	return;
}

void prune_redundant_contraints(){
	if(condition_no == 0){  //xm < x*
		//vector<vector<double> > Iplus_tmp;
		for(vector<vector<double> >::iterator it= Iplus.begin(); it+1 < Iplus.end(); it += 2){
			//two lines intersect
			if(((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0 ) && ((*(it+1))[0] != 0.0 || (*(it+1))[1] != 0.0 || (*(it+1))[2] != 0.0)) {
				double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				vector<double> point(2);
				point[0] = x;
				point[1] = y;
				if(point[0] <= u1){  //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iplus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
					else{
						//Iplus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
				}
				else if(point[0] >= u2){    //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iplus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
					else{
						//Iplus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
				}
			}
		}

		//vector<vector<double> > Iminus_tmp;
		for(vector<vector<double> >::iterator it= Iminus.begin(); it+1 < Iminus.end(); it += 2){
			//two lines intersect
			if(((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0 ) && ((*(it+1))[0] != 0.0  || (*(it+1))[1] != 0.0 || (*(it+1))[2] != 0.0)) {
				double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				vector<double> point(2);
				point[0] = x;
				point[1] = y;
				if(point[0] <= u1){   //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iminus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
					else{
						//Iminus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
				}
				else if(point[0] >= u2){    //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iminus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
					else{
						//Iminus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
				}
			}
		}

		vector<vector<double> > Iplus_tmp;
		for(vector<vector<double> >::iterator it = Iplus.begin(); it != Iplus.end(); it++){
			if((*it)[0] != 0 || (*it)[1] != 0 || (*it)[2] != 0){
				Iplus_tmp.push_back((*it));
			}
		}
		Iplus.clear();
		Iplus.assign(Iplus_tmp.begin(), Iplus_tmp.end());
		/*
		for(vector<vector<double> >::iterator it = Iplus.begin(); it != Iplus.end(); it++){
			cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
		}
		*/
		vector<vector<double> > Iminus_tmp;
		for(vector<vector<double> >::iterator it = Iminus.begin(); it != Iminus.end(); it++){
			if((*it)[0] != 0 || (*it)[1] != 0 || (*it)[2] != 0){
				Iminus_tmp.push_back((*it));
			}
		}
		Iminus.clear();
		Iminus.assign(Iminus_tmp.begin(), Iminus_tmp.end());
		/*
		for(vector<vector<double> >::iterator it = Iminus.begin(); it != Iminus.end(); it++){
			cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
		}
		*/
	}
	else if(condition_no == 1){
		for(vector<vector<double> >::iterator it= Iplus.begin(); it+1 < Iplus.end(); it += 2){
			//two lines intersect
			if(((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0 ) && ((*(it+1))[0] != 0.0  || (*(it+1))[1] != 0.0 || (*(it+1))[2] != 0.0)) {
				double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				vector<double> point(2);
				point[0] = x;
				point[1] = y;
				if(point[0] <= u1){  //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iplus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
					else{
						//Iplus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
				}
				else if(point[0] >= u2){    //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iplus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
					else{
						//Iplus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
				}
			}
		}

		//vector<vector<double> > Iminus_tmp;
		for(vector<vector<double> >::iterator it= Iminus.begin(); it+1 < Iminus.end(); it += 2){
			//two lines intersect
			if(((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0 ) && ((*(it+1))[0] != 0.0  || (*(it+1))[1] != 0.0 || (*(it+1))[2] != 0.0)) {
				double x = ((*it)[2]*(*(it+1))[1] - (*(it+1))[2]*(*it)[1])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				double y = ((*it)[0]*(*(it+1))[2] - (*(it+1))[0]*(*it)[2])/((*it)[0]*(*(it+1))[1]-(*it)[1]*(*(it+1))[0]);
				vector<double> point(2);
				point[0] = x;
				point[1] = y;
				if(point[0] <= u1){   //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iminus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
					else{
						//Iminus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
				}
				else if(point[0] >= u2){    //out of boundary
					if((-(*it)[0]/(*it)[1]) > (-(*(it+1))[0]/(*(it+1))[1])){
						//Iminus_tmp.push_back((*(it+1)));
						(*it)[0] = 0.0;
						(*it)[1] = 0.0;
						(*it)[2] = 0.0;
					}
					else{
						//Iminus_tmp.push_back((*it));
						(*(it+1))[0] = 0.0;
						(*(it+1))[1] = 0.0;
						(*(it+1))[2] = 0.0;
					}
				}
			}
		}

		vector<vector<double> > Iplus_tmp;
		for(vector<vector<double> >::iterator it = Iplus.begin(); it != Iplus.end(); it++){
			if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){
				Iplus_tmp.push_back((*it));
			}
		}
		Iplus.clear();
		Iplus.assign(Iplus_tmp.begin(), Iplus_tmp.end());
		/*
		for(vector<vector<double> >::iterator it = Iplus.begin(); it != Iplus.end(); it++){
			cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
		}
		*/
		vector<vector<double> > Iminus_tmp;
		for(vector<vector<double> >::iterator it = Iminus.begin(); it != Iminus.end(); it++){
			if((*it)[0] != 0.0 || (*it)[1] != 0.0 || (*it)[2] != 0.0){
				Iminus_tmp.push_back((*it));
			}
		}
		Iminus.clear();
		Iminus.assign(Iminus_tmp.begin(), Iminus_tmp.end());
		/*
		for(vector<vector<double> >::iterator it = Iminus.begin(); it != Iminus.end(); it++){
			cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
		}
		*/
	}
	else if(condition_no == 2){
		printf("%d\n", int(roundl(ymin)));
		isterminate = 1;
	}
	else if(condition_no == 3){
		printf("NA\n");
		isterminate = 1;
	}
	return;
}

int main(int argc, char * argv[]){
	input();

	classify_I();

	x_bound();   //u1 <= u2  step1

	while(!isterminate && (Iminus.size() > 1 || Iplus.size() > 1)){
		//printf("Iminus.size()=%lu\n", Iminus.size());
		//printf("Iplus.size()=%lu\n", Iplus.size());

		partion_Iminus_and_Iplus();   //step2

		find_x_median();   //step3
		/*
		for(vector<vector<double> >::iterator it = pointm_set.begin(); it != pointm_set.end(); it++){m
			cout << (*it)[0] << "," << (*it)[0] << endl;
		}
		cout << xm << endl;
		*/
		shrink_x_range();    //step4

		prune_redundant_contraints();     //step5

	}
	//printf("Iminus.size()=%lu\n", Iminus.size());
	//printf("Iplus.size()=%lu\n", Iplus.size());
	//printf("a = %f, b = %f, c = %f\n", Iminus[0][0], Iminus[0][1], Iminus[0][2]);
	//printf("a = %f, b = %f, c = %f\n", Iplus[0][0], Iplus[0][1], Iplus[0][2]);
	
	if(!isterminate){
		//printf("%f\n", (Iminus[0][0]*Iplus[0][1] - Iplus[0][0]*Iminus[0][1]));
		if((Iminus.size() == 0 && Iplus.size() != 0) || (Iminus.size() != 0 && Iplus.size() == 0)){
			printf("-INF\n");
		}
		else{
			if(Iminus[0][0]/Iminus[0][1] == Iplus[0][0]/Iplus[0][1]){
				printf("NA\n");
			}
			else{
				double solx = (Iminus[0][2]*Iplus[0][1] - Iplus[0][2]*Iminus[0][1])/(Iminus[0][0]*Iplus[0][1] - Iplus[0][0]*Iminus[0][1]);
				double soly = (Iminus[0][0]*Iplus[0][2] - Iplus[0][0]*Iminus[0][2])/(Iminus[0][0]*Iplus[0][1] - Iplus[0][0]*Iminus[0][1]);
				double x[4] = {};
				double y[4] = {};
				x[0] = solx + Iminus[0][1];
				y[0] = soly + (-Iminus[0][0]);
				x[1] = solx - Iminus[0][1];
				y[1] = soly - (-Iminus[0][0]);
				x[2] = solx + Iplus[0][1];
				y[2] = soly + (-Iplus[0][0]);
				x[3] = solx - Iplus[0][1];
				y[3] = soly - (-Iplus[0][0]);
				double x_up[2];
				double y_up[2];
				int idx = 0;
				for(int i = 0; i < 4; i++){
					if(y[i] > soly){
						x_up[idx] = x[i];
						y_up[idx] = y[i];
						idx++;
					}
				}
				//printf("idx = %d\n", idx);
				int cnt = 0;
				for(int i = 0; i < 2; i++){
					if(Iminus[0][0]*x_up[i]+Iminus[0][1]*y_up[i] < Iminus[0][2]+epsilon) {
						cnt++;
					}
					if(Iplus[0][0]*x_up[i]+Iplus[0][1]*y_up[i] < Iplus[0][2]+epsilon){
						cnt++;
					}
				}
				//printf("cnt = %d\n", cnt);

				if(cnt == 4){
					ymin = soly;
					printf("%d\n", int(roundl(ymin)));
				}
				else{
					printf("-INF\n");
				}

			}
		}
	}

	return 0;
}