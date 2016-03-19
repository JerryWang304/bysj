#include<Eigen/Dense>
#include<iostream>
#include"maxminRate.h"
using namespace std;
using namespace Eigen;

MatrixXd flowAllocate(int flow_alg,MatrixXd active, MatrixXd R, MatrixXd C);
