#include<Eigen/Dense>
#include<iostream>
#include<vector>
using namespace std;
using namespace Eigen;
MatrixXd maxtoRate(MatrixXd x, MatrixXd R, MatrixXd C){
    vector<int> idx;
    idx.clear();
    for(int i=0;i<x.cols();i++){
        if(x(i,0) > 0.01){
            idx.push_back(i);
        }
    }
    MatrixXd Rx;
    int col=0;
    for(int i=0;i<idx.size();i++){
        int c = idx[i];
        for(int j=0;j<R.rows();j++){
            Rx(j,col) = R(j,c);
        }
        col++;
    }
    int len = idx.rows();
    MatrixXd lb,ub,Aeq,beq,A,b,f;
    lb = MatrixXd::Zero(len,1);
    A = Rx;
    b = C;
    f = MatrixXd::Ones(len,1);
    f = -1*f;

}
