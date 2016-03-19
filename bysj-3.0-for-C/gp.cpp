#include<iostream>
#include<Eigen/Dense>
#include<random>
#include<chrono>
#include<cmath>
#include<ctime>
#include<fstream>
#include"shuffleBytes.h"
#include"getGPMatrix.h"
#include"FloydWarshall.h"
#include"FWGetPath.h"
using namespace std;
using namespace Eigen;
double gp(int opt_len, int sub_number, int flow_alg){
    int numsw = 4;
    int numhash = numsw*2;
    int numhost = numsw;
    int numvlan = numsw;
    int N = numsw + numhost;

    double mu = 1.0;
    double sigma = 0.2;
    int old_numhash = numhash;
    numhash = numhash*sub_number;//之前少了这句话
    MatrixXd tm ;
    tm = MatrixXd::Zero(4*(numhash+1),4);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    normal_distribution<double> distribution (mu,sigma);
    for(int j=0;j<4*(numhash+1);j++){
        for(int i=0;i<4;i++){
            if(j%4 != i){
                double f = distribution(generator);
                tm(j,i) = max(f,0.0)/100;
            }
        }
    }
    //cout<<tm<<endl;
    //getGPMatrix()
    MatrixXd gp_tm;
    gp_tm = getGPMatrix(sub_number,tm,numhost,numsw,numvlan,numhash);
    //cout<<gp_tm<<endl;
    MatrixXd Adj(N,N);
    Adj<<0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,1,0,1,0,1,1,
1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,0;

    int numlink = Adj.sum()/2;
    MatrixXd C(numlink*2,1),L2N(numlink*2,2),N2L(N,N),vlan(32,8);
    C<<100.000000,
    100.000000,
    100.000000,
    100.000000,
    100.000000,
    100.000000,
    100.000000,
    100.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000;
    L2N<<1,7,
    7,1,
    2,8,
    8,2,
    3,6,
    6,3,
    4,5,
    5,4,
    7,8,
    8,7,
    7,6,
    6,7,
    7,5,
    5,7,
    8,6,
    6,8,
    8,5,
    5,8,
    6,5,
    5,6;

    for(int r=0;r<numlink*2;r++){
        for(int c=0;c<2;c++){
            L2N(r,c)--;
        }
    }

    N2L<<0,0,0,0,0,0,1,0,
         0,0,0,0,0,0,0,3,
         0,0,0,0,0,5,0,0,
         0,0,0,0,7,0,0,0,
         0,0,0,8,0,20,14,18,
         0,0,6,0,19,0,12,16,
         2,0,0,0,13,11,0,9,
         0,4,0,0,17,15,10,0;
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            if(N2L(r,c)>0)
                N2L(r,c)--;
        }
    }

    vlan<<0,0,0,0,0,0,1,0,
          0,0,0,0,0,0,0,1,
          0,0,0,0,0,1,0,0,
          0,0,0,0,1,0,0,0,
          0,0,0,1,0,0,1,0,
          0,0,1,0,0,0,1,0,
          1,0,0,0,1,1,0,1,
          0,1,0,0,0,0,1,0,
          0,0,0,0,0,0,1,0,
          0,0,0,0,0,0,0,1,
          0,0,0,0,0,1,0,0,
          0,0,0,0,1,0,0,0,
          0,0,0,1,0,0,0,1,
          0,0,1,0,0,0,0,1,
          1,0,0,0,0,0,0,1,
          0,1,0,0,1,1,1,0,
          0,0,0,0,0,0,1,0,
          0,0,0,0,0,0,0,1,
          0,0,0,0,0,1,0,0,
          0,0,0,0,1,0,0,0,
          0,0,0,1,0,1,0,0,
          0,0,1,0,1,0,1,1,
          1,0,0,0,0,1,0,0,
          0,1,0,0,0,1,0,0,
          0,0,0,0,0,0,1,0,
          0,0,0,0,0,0,0,1,
          0,0,0,0,0,1,0,0,
          0,0,0,0,1,0,0,0,
          0,0,0,1,0,1,1,1,
          0,0,1,0,1,0,0,0,
          1,0,0,0,1,0,0,0,
          0,1,0,0,1,0,0,0;


    C = C/100;
    //cout<<C<<endl;
    MatrixXd VLAN[numvlan],TM[numhash+1];
    for(int i=0;i<numvlan;i++){
        VLAN[i] = MatrixXd::Zero(N,N);
    }

    for(int i=0;i<numvlan;i++){

        for(int r=i*8;r<(i+1)*8;r++){
            for(int c=0;c<8;c++){
                VLAN[i](r-i*8,c) = vlan(r,c);
            }
        }

    }
    //vlan是对的
    for(int i=0;i<numhash+1;i++)
        TM[i] = MatrixXd::Zero(numhost,numhost);


    for(int i=0;i<numhash+1;i++){
        for(int r=i*numhost;r<(i+1)*numhost;r++){
            for(int c=0;c<4;c++){
                TM[i](r-i*numhost,c) = gp_tm(r,c);
            }
        }
        //cout<<TM[i]<<endl;
    }

    MatrixXd TMvec;
    TMvec = MatrixXd::Zero(numhost*numhost,numhash);
    for(int h=0;h<numhash;h++){
        MatrixXd tm = TM[h];
        tm.transposeInPlace();
        tm.resize(numhost*numhost,1);
        //cout<<tm<<endl<<endl;
        for(int i=0;i<TMvec.rows();i++){
            TMvec(i,h) = tm(i,0);
        }

    }
//    ofstream out;
//    out.open("C:\\test_TMvec.txt");
//    out<<TMvec;
//    out.close();
    //cout<<N2L<<endl;
    MatrixXd projMat[numvlan];
    for(int v=0;v<numvlan;v++){
        MatrixXd next;
        next = FloydWarshall(VLAN[v]);
        MatrixXd out;
        out = MatrixXd::Zero(numlink*2,numhost*numhost);
        projMat[v] = MatrixXd::Zero(numlink*2,numhost*numhost);
        for(int n1=0;n1<numhost;n1++){
            //n1等于3时，循环挂掉
            for(int n2=0;n2<numhost;n2++){
                int col = n1*numhost + n2;

                vector<int> nodepath = FWGetPath(next,n1,n2);
                int len = nodepath.size();

                // begin
                for(int i=0;i<len/2;i++){
                    int link = N2L(nodepath[2*i],nodepath[2*i+1]);
                   // if(n1==3 && v==2)
                   //     cout<<"link="<<link<<" "<<"col="<<col<<endl;
                    out(link,col) = 1;//link可能等于20，out行下标：0~19
                    //由于link是直接从数据中的，所以会出现这种错误。
                }
                //end
                //if(n1==3 && v==2)
                 //   cout<<"hi"<<endl;
            }
        // v=2时，第四次循环挂掉
        //cout<<"up "<<v<<endl;
        }
        //cout<<"v = "<<v<<endl;
        projMat[v] = out;
    }
    //projMat是正确的
//    for(int i=0;i<numvlan;i++){
//        cout<<"i = "<<i<<endl;
//        cout<<projMat[i]<<endl<<endl;
//    }
    return shuffleBytes(opt_len,flow_alg,numhost,numsw,numlink,numvlan,numhash,C, projMat,TMvec);

}
