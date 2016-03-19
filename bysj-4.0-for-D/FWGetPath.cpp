#include<string>
#include<iostream>
#include<vector>
#include<Eigen/Dense>
using namespace Eigen;
using namespace std;

vector<int> FWGetPath(MatrixXd next,int i,int j){
    if(i == j){
        vector<int> v;
        v.clear();
        return v;
    }
    int k = next(i,j);
    if(k == -1){
        vector<int> v;
        v.clear();
        return v;
    }else if(k == 0){
        vector<int> v;
        v.clear();
        v.push_back(i);
        v.push_back(j);
        return v;
    }else{
        vector<int> v;
        v.clear();
        vector<int> first = FWGetPath(next,i,k);
        vector<int> second = FWGetPath(next,k,j);

        for(int i=0;i<first.size();i++)
            v.push_back(first[i]);

        for(int i=0;i<second.size();i++)
            v.push_back(second[i]);
        return v;
    }
}
