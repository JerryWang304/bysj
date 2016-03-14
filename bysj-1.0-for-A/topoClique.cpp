#include<string>
#include<iostream>
#include<Eigen/Dense>
using namespace Eigen;
using namespace std;

//typedef struct data{
//    int numhost;
//    int numsw;
//    int numvlan;
//    int numhash;
//    int N;
//    MatrixXd Adj,C,N2L,L2N;
//
//    MatrixXd VLAN[numvlan];
//    MatrixXd TM[numhash+1];
//}Date;
//Date d;
void topo_clique(int numsw){

    string topo_name = "clique";
    int numhash = numsw*2;
    int numhost = numsw;
    int numvlan = numsw;
    int N = numsw + numhost;
    //这些矩阵从0行0列开始。。。
    MatrixXd Adj(N,N),N2L(N,N),L2N(N*N,2);
    Adj = MatrixXd::Zero(N,N);
    N2L = MatrixXd::Zero(N,N);
    L2N = MatrixXd::Zero(N*N,2);

    int idx = 0;
    for(int h=0;h<numhost;h++){
        int sw = numhost+h;
        Adj(h,sw) = 1;
        Adj(sw,h) = 1;

        N2L(h,sw) = idx;
        N2L(sw,h) = idx+1;

        L2N(idx,0) = h;
        L2N(idx,1) = sw;
        L2N(idx+1,0) = sw;
        L2N(idx+1,1) = h;

        idx = idx+2;
    }
    for(int s1=numhost;s1<numhost+numsw;s1++){
        for(int s2=s1+1;s2<numhost+numsw;s2++){
            Adj(s1,s2) = 1;
            Adj(s2,s1) = 1;

            N2L(s1,s2) = idx;
            N2L(s2,s1) = idx+1;

            L2N(idx,0) = s1;
            L2N(idx,1) = s2;
            L2N(idx+1,0) = s2;
            L2N(idx+1,1) = s1;
            idx += 2;
        }
    }
    MatrixXd VLAN[numvlan];
    for(int i=0;i<numvlan;i++){
        VLAN[i] = MatrixXd::Zero(N,N);
    }
    for(int v=0;v<numvlan;v++){
        for(int i=0;i<numhost;i++){
            VLAN[v](i,numhost+i) = 1;
            VLAN[v](numhost+i,i) = 1;
        }
        for(int i=0;i<numsw;i++){
            if(i == v)
                continue;
            VLAN[v](numhost+v,numhost+i) = 1;
            VLAN[v](numhost+i,numhost+v) = 1;
        }
    }
    //test 和matlab的数据比对过，没问题。
    //唯一的区别是矩阵从0开始计算
//    for(int i=0;i<numvlan;i++)
//        cout<<VLAN[i]<<endl<<endl;
//    return ;
    MatrixXd TM[numhash+1];
    for(int i=0;i<numhash+1;i++)
        TM[i] = MatrixXd::Zero(numhost,numhost);
    for(int h=0;h<numhash;h++){
        for(int i=0;i<numhost;i++){
            for(int j=0;j<numhost;j++){
                if(i == j)
                    TM[h](i,j) = 0;
                else
                    TM[h](i,j) = 0.01;
            }
        }
    }
    for(int i=0;i<numhost;i++){
        for(int j=0;j<numhost;j++){
            if(i == j)
                TM[numhash](i,j) = 0;
            else
                TM[numhash](i,j) = numhash*1.0/100;

        }
    }
    //test TM 正确
//    for(int i=0;i<numhash+1;i++)
//        cout<<TM[i]<<endl<<endl;

    int numlink = Adj.sum();
    L2N = L2N.topRows(numlink);
    MatrixXd C(numlink*2,1);
    C = MatrixXd::Zero(numlink*2,1);
    idx = 0;
    for(int i=0;i<numhost;i++){
        C(idx,0) = 1;
        C(idx+1,0) = 1;
        idx += 2;
    }
    for(int i=numhost;i<numlink;i++){
        C(idx,0) = 0.01;
        C(idx+1,0) = 0.01;
        idx += 2;
    }
    //save variables
//    d.Adj = Adj;
//    d.C = C;
//    d.L2N = L2N;
//    d.N2L = N2L;
//    d.N = N;
//
//    d.numvlan = numvlan;
//    d.numhash = numhash;
//    d.numhost = numhost;
//    d.numsw = numsw;
//
//    d.VLAN = VLAN;
//    d.TM = TM;
    return ;
}
