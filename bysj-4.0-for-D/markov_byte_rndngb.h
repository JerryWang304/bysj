#include<iostream>
#include<Eigen/Dense>
using namespace Eigen;
using namespace std;
vector<int> markov_byte_rndngb(MatrixXd tm, int flow_alg, int numhost, int numsw, int numlink,int numvlan,int numhash,MatrixXd C,MatrixXd projMat[]);
