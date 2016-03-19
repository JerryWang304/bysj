#include<iostream>
#include<Eigen/Dense>
using namespace Eigen;

double shuffleBytesBia(double opt_time,int flow_alg,double bia,MatrixXd ori_tm,int numhost,int numsw,int numlink,int numvlan,int numhash,MatrixXd C,MatrixXd projMat[],MatrixXd TMvec);
