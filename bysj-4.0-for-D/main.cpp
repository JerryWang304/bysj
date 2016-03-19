#include<string>
#include<iostream>
#include<Eigen/Dense>
#include<vector>
#include"FloydWarshall.h"
#include"FWGetPath.h"
#include"getRoute.h"
#include"flowAllocate.h"
#include"markov_byte_rndngb.h"
#include"shuffleBytes.h"
#include<random>
#include<fstream>
#include<chrono>
#include<cmath>
#include"shuffleBytesBia.h"
using namespace Eigen;
using namespace std;

int main()
{
    double mu = 1.0;
    double sigma = 0.2;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::normal_distribution<double> distribution (mu,sigma);
    //topo_clique(4); //currently 16-clique, can also use other size
    int numsw = 4;
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
    for(int h=0; h<numhost; h++)
    {
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
    for(int s1=numhost; s1<numhost+numsw; s1++)
    {
        for(int s2=s1+1; s2<numhost+numsw; s2++)
        {
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
    for(int i=0; i<numvlan; i++)
    {
        VLAN[i] = MatrixXd::Zero(N,N);
    }
    for(int v=0; v<numvlan; v++)
    {
        for(int i=0; i<numhost; i++)
        {
            VLAN[v](i,numhost+i) = 1;
            VLAN[v](numhost+i,i) = 1;
        }
        for(int i=0; i<numsw; i++)
        {
            if(i == v)
                continue;
            VLAN[v](numhost+v,numhost+i) = 1;
            VLAN[v](numhost+i,numhost+v) = 1;
        }
    }

    int numlink = Adj.sum()/2;
    // L2N = L2N.topRows(numlink);
    MatrixXd C(numlink*2,1);
    C = MatrixXd::Zero(numlink*2,1);
    idx = 0;
    for(int i=0; i<numhost; i++)
    {
        C(idx,0) = 1;
        C(idx+1,0) = 1;
        idx += 2;
    }
    for(int i=numhost; i<numlink; i++)
    {
        C(idx,0) = 0.01;
        C(idx+1,0) = 0.01;
        idx += 2;
    }

    //probSetup();
    MatrixXd projMat[numvlan];
    for(int v=0; v<numvlan; v++)
    {
        MatrixXd next(numhost,numhost);
        next = FloydWarshall(VLAN[v]);
        //projMat[v] = FWAllPath(next);
        MatrixXd out;
        out = MatrixXd::Zero(numlink*2,numhost*numhost);
        for(int n1=0; n1<numhost; n1++)
        {
            for(int n2=0; n2<numhost; n2++)
            {
                int col = n1*numhost + n2;
                vector<int> nodepath = FWGetPath(next,n1,n2);
                int len = nodepath.size();
                for(int i=0; i<len/2; i++)
                {
                    int link = N2L(nodepath[2*i],nodepath[2*i+1]);
                    out(link,col) = 1;
                }
            }

        }
        projMat[v] = out;

    }
    MatrixXd TM[numhash+1];
    for(int i=0; i<numhash+1; i++)
        TM[i] = MatrixXd::Zero(numhost,numhost);
    for(int h=0; h<numhash; h++)
    {
        for(int i=0; i<numhost; i++)
        {
            for(int j=0; j<numhost; j++)
            {
                if(i == j)
                    TM[h](i,j) = 0;
                else
                {
                    //double temp = distribution(generator);
                    TM[h](i,j) = 1;
                    //cout<<temp<<endl;
                }
            }
        }
    }
    //这段代码暂时并没有用到
    for(int i=0; i<numhost; i++)
    {
        for(int j=0; j<numhost; j++)
        {
            if(i == j)
                TM[numhash](i,j) = 0;
            else
                TM[numhash](i,j) = numhash*1.0/100;

        }
    }
    //test TM 正确
//    for(int i=0;i<numhash+1;i++)
//        cout<<TM[i]<<endl<<endl;

    //per flow

    MatrixXd TMvec;
    TMvec = MatrixXd::Zero(numhost*numhost,numhash);
    for(int h=0; h<numhash; h++)
    {
        MatrixXd tm = TM[h];
        tm.transposeInPlace();
        tm.resize(numhost*numhost,1);
        //cout<<tm<<endl<<endl;
        for(int i=0; i<TMvec.rows(); i++)
        {
            TMvec(i,h) = tm(i,0);
        }
    }
    //cout<<TMvec;
    MatrixXd ori_tm ;
    ori_tm = TMvec;
    ori_tm.resize(numhash*numhost*numhost,1);
    //cout<<ori_tm;
    int trials = 34;//10次试验要跑12分钟
    double opt_time = 1;
    for(int i=0; i<trials; i++)
    {


        for(int i=0; i<numhash+1; i++)
            TM[i] = MatrixXd::Zero(numhost,numhost);
        for(int h=0; h<numhash; h++)
        {
            for(int i=0; i<numhost; i++)
            {
                for(int j=0; j<numhost; j++)
                {
                    if(i == j)
                        TM[h](i,j) = 0;
                    else
                    {
                        double temp = distribution(generator);
                        TM[h](i,j) = max(temp,0.0)/100;

                    }
                }
            }
        }
        //这段代码暂时并没有用到
        for(int i=0; i<numhost; i++)
        {
            for(int j=0; j<numhost; j++)
            {
                if(i == j)
                    TM[numhash](i,j) = 0;
                else
                    TM[numhash](i,j) = numhash*1.0/100;

            }
        }

        for(int h=0; h<numhash; h++)
        {
            MatrixXd tm = TM[h];
            tm.transposeInPlace();
            tm.resize(numhost*numhost,1);
            //cout<<tm<<endl<<endl;
            for(int i=0; i<TMvec.rows(); i++)
            {
                TMvec(i,h) = tm(i,0);
            }
        }
        ofstream out2,out4,out6,out8,out10;

        out2.open("C:\\d_2.txt",ios::app);
        out4.open("C:\\d_4.txt",ios::app);
        out6.open("C:\\d_6.txt",ios::app);
        out8.open("C:\\d_8.txt",ios::app);
        out10.open("C:\\d_10.txt",ios::app);
        double t2,t4,t6,t8,t10;
        t2 = shuffleBytesBia(opt_time,1,0.02,ori_tm,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        t4 = shuffleBytesBia(opt_time,1,0.04,ori_tm,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        t6 = shuffleBytesBia(opt_time,1,0.06,ori_tm,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        t8 = shuffleBytesBia(opt_time,1,0.08,ori_tm,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        t10 = shuffleBytesBia(opt_time,1,0.10,ori_tm,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        cout<<i+1<<" trials finished"<<endl;
        out2<<t2<<" ";
        out4<<t4<<" ";
        out6<<t6<<" ";
        out8<<t8<<" ";
        out10<<t10<<" ";
        out2.close();out4.close();out6.close();out8.close();out10.close();
    }
    return 0;
}
