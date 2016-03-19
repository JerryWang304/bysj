#include<Eigen/Dense>
#include<iostream>
#include"maxminRate.h"
//#include"maxtoRate.h"
using namespace std;
using namespace Eigen;

MatrixXd flowAllocate(int flow_alg,MatrixXd active,MatrixXd R,MatrixXd C){
    switch(flow_alg){
    case 1:
        return maxminRate(active,R,C);
        break;
    //case 2:
    //    return maxtoRate(active,R,C);
    default:
        cout<<"wrong flow algorithm choose"<<endl;
    }
}
