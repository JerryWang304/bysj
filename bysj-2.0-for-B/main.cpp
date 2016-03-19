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
using namespace Eigen;
using namespace std;

int main()
{
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
    //test 和matlab的数据比对过，没问题。
    //唯一的区别是矩阵从0开始计算
//    for(int i=0;i<numvlan;i++)
//        cout<<VLAN[i]<<endl<<endl;
//    return ;


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

    int flow_alg = 1;

    int t = 0;

    double mu = 1.0;
    double sigma = 0.2;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::normal_distribution<double> distribution (mu,sigma);
    int trials = 84;
    while(t<trials)
    {
        ofstream out0,out1,out2,out3,out4,out5,out6;
        ofstream out7,out8,out9,out10;
        out0.open("C:\\b_T0.txt",ios::app);
        out1.open("C:\\b_T1.txt",ios::app);
        out2.open("C:\\b_T2.txt",ios::app);
        out3.open("C:\\b_T3.txt",ios::app);
        out4.open("C:\\b_T4.txt",ios::app);
        out5.open("C:\\b_T5.txt",ios::app);
        out6.open("C:\\b_T6.txt",ios::app);
        out7.open("C:\\b_T7.txt",ios::app);
        out8.open("C:\\b_T8.txt",ios::app);
        out9.open("C:\\b_T9.txt",ios::app);
        out10.open("C:\\b_T10.txt",ios::app);
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
                        double temp = distribution(generator);
                        TM[h](i,j) = max(temp,0.0)*0.01;
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

        double total_time0 =shuffleBytes(-1,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out0<<" "<<total_time0;
        else
            out0<<total_time0;
        //cout<<total_time0<<endl;
        // T = 1
        double total_time1 = shuffleBytes(1,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out1<<" "<<total_time1;
        else
            out1<<total_time1;
        // T = 2
        double total_time2 = shuffleBytes(2,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out2<<" "<<total_time2;
        else
            out2<<total_time2;
        // T = 3
        double total_time3 = shuffleBytes(3,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out3<<" "<<total_time3;
        else
            out3<<total_time3;
        // T = 4
        double total_time4 = shuffleBytes(4,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out4<<" "<<total_time4;
        else
            out4<<total_time4;
        // T = 5
        double total_time5 = shuffleBytes(5,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out5<<" "<<total_time5;
        else
            out5<<total_time5;
        // T = 6
        double total_time6 = shuffleBytes(6,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out6<<" "<<total_time6;
        else
            out6<<total_time6;
        // T = 7
        double total_time7 = shuffleBytes(7,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out7<<" "<<total_time7;
        else
            out7<<total_time7;
        // T = 8
        double total_time8 = shuffleBytes(8,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out8<<" "<<total_time8;
        else
            out8<<total_time8;
        // T = 9
        double total_time9 = shuffleBytes(9,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out9<<total_time9<<" ";
        else
            out9<<total_time9;
        // T = 10
        double total_time10 = shuffleBytes(10,flow_alg,numhost,numsw,numlink,numvlan,numhash,C,projMat,TMvec);
        if(t<trials-1)
            out10<<" "<<total_time10;
        else
            out10<<total_time10;

        t++;
        cout<<"t = "<<t<<endl;
        out0.close();
        out1.close();
        out2.close();
        out3.close();
        out4.close();
        out5.close();
        out6.close();
        out7.close();
        out8.close();
        out9.close();
        out10.close();
    }

    //cout<<endl<<"total_time = "<<total_time<<endl;
    return 0;
}
