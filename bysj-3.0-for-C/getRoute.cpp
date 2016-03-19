#include<Eigen/Dense>
#include<vector>
using namespace Eigen;
using namespace std;
MatrixXd getRoute(vector<int> x,MatrixXd projMat[],int numhash,int numvlan,int numlink,int numhost){
    int len = numhash*numhost*numhost;
    MatrixXd R(2*numlink,len);
    for(int h=0;h<numhash;h++){
        int v = x[h];
        for(int i=0;i<numlink*2;i++){
            for(int j=h*numhost*numhost;j<(h+1)*numhost*numhost;j++){
                R(i,j) = projMat[v](i,j-h*numhost*numhost);
            }
        }
    }
    return R;
}
