#include<Eigen/Dense>
#include<iostream>
#include"maxminRate.h"
using namespace std;
using namespace Eigen;

MatrixXd flowAllocate(int flow_alg,MatrixXd active,MatrixXd R,MatrixXd C){
    switch(flow_alg){
    case 1:
        return maxminRate(active,R,C);
        break;
    default:
        cout<<"wrong flow algorithm choose"<<endl;
    }
}
