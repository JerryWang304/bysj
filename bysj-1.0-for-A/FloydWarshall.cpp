#include<iostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;
const int inf = 1e9;

MatrixXd FloydWarshall(MatrixXd G){

    int N = G.diagonal().size();
    MatrixXd path = G;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(i == j)
                path(i,j) = 0;
            else if(G(i,j) == 0)
                path(i,j) = inf;
        }
    }
    MatrixXd next;
    next = -1*MatrixXd::Ones(N,N);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(path(i,j) < inf)
                next(i,j) = 0;
        }
    }
    for(int k=0;k<N;k++){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(path(i,k)+path(k,j)<path(i,j)){
                    path(i,j) = path(i,k)+path(k,j);
                    next(i,j) = k;
                }
            }
        }
    }
    return next;
}
