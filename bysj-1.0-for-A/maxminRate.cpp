#include<iostream>
#include<Eigen/Dense>
using namespace Eigen;
using namespace std;
const int inf = 1e9;
MatrixXd maxminRate(MatrixXd x, MatrixXd R, MatrixXd C){
    int L = C.rows();
    int F = x.rows();
    MatrixXd C_res = C;
    MatrixXd f = x;
    MatrixXd rate;
    rate = MatrixXd::Zero(F,1);
    while(f.sum()>=1){
        MatrixXd fcount;
        fcount = R*f;
        MatrixXd fshare;
        fshare = MatrixXd::Ones(L,1);
        fshare = inf*fshare;
        for(int l=0;l<L;l++){
            if(fcount(l,0)>0){
                fshare(l,0) = C_res(l,0)*1.0/fcount(l,0);
            }
        }
        int minlink,c;
        double val = fshare.minCoeff(&minlink,&c);
        //cout<<r<<" "<<c<<" "<<t<<endl;
        //第一次while循环得出的结果是正确的 cout<<fshare;
        for(int i=0;i<F;i++){
            if(R(minlink,i) == 1 && f(i,0)>0){
                rate(i,0) = rate(i,0)+val;
                f(i) = 0;
            }
        }
        C_res = C - R*rate;
    }
    return rate;
}
