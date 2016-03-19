#include<Eigen/Dense>
#include<random>
#include<chrono>
using namespace Eigen;
using namespace std;
MatrixXd get_bia_tm(MatrixXd tm,double bia,MatrixXd ori_tm){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    MatrixXd bia_tm;
    bia_tm = MatrixXd::Zero(128,1);
    for(int i=0;i<128;i++){
        if(ori_tm(i,0)>0.0){
            std::normal_distribution<double> distribution (tm(i,0),bia);
            double temp = distribution(generator);
            bia_tm(i,0) = max(0.0,temp);
        }
    }
    return bia_tm;
}
