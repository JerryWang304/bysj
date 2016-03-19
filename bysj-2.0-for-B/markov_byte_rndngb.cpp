#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <chrono>
#include <cmath>
#include "getRoute.h"
#include "flowAllocate.h"
using namespace Eigen;
using namespace std;
vector<int> markov_byte_rndngb(MatrixXd tm, int flow_alg, int numhost, int numsw, int numlink,int numvlan,int numhash,MatrixXd C,MatrixXd projMat[]){
    //markov_byte_rndngb

    double beta = 5.0;
    int maxT = 100;
    //cout<<tm;

    MatrixXd active = tm;
    for(int i=0;i<active.rows();i++){
        if(active(i,0)>0)
            active(i,0) = 1;
    }
    //cout<<active;
    int rlen = numhash*numhost*numhost;
    int numcand = numvlan;
    MatrixXd history;
    history = MatrixXd::Zero(maxT,1);
    vector<int> x0;
    x0.clear();
    for(int i=0;i<numhash;i++)
        x0.push_back(i);
//    for(int i=0;i<x0.size();i++)
//        cout<<x0[i]<<" ";
    for(int i=0;i<numhash;i++){
        int t = x0[i];
        x0[i] = t%numvlan ;
    }
    // x0 = 0,1,2,3,0,1,2,3
//    for(int i=0;i<x0.size();i++)
//        cout<<x0[i]<<" ";
    MatrixXd R;
    R = getRoute(x0,projMat,numhash,numvlan,numlink,numhost);
    MatrixXd r0;
    r0 = flowAllocate(flow_alg,active,R,C);
    //cout<<r0;
    double f0 = r0.sum();
    vector<int> x_best = x0;
    double f_best = f0;
    vector<int> x_now = x0;
    double f_now = f0;
    int t = 0;
    int checkpoint = 0;//不知道这是用来干嘛的...

    while(t<maxT){

        if(t>checkpoint)
            checkpoint += 50;
        MatrixXd f_cand;
        f_cand = MatrixXd::Zero(numcand,1);
        int h;
        //不设定种子的话，每次产生的随机数序列都一样
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0,numhash-1);
       //每次编译以后生成的随机数序列是不一样的
//        for(int i=0;i<numhash;i++){
//            h = distribution(generator);
//            cout<<h;
//        }
       // break;
        //return x0;
        h = distribution(generator);
        vector<int> x_cand;
        for(int v=0;v<numvlan;v++){

            x_cand = x_now;
            x_cand[h] = v;
            R = getRoute(x_cand,projMat,numhash,numvlan,numlink,numhost);
            MatrixXd rate;
            rate = flowAllocate(flow_alg,active,R,C);
            int idx=v;
            f_cand(idx,0) = rate.sum();
        }

        f_cand = beta*f_cand;
        MatrixXd prob;
        prob = f_cand;
        //cout<<"f_cand="<<endl;
        //cout<<f_cand<<endl<<endl;
        for(int i=0;i<prob.rows();i++){
            prob(i,0) = exp(prob(i,0))+1e-9;
        }
       // prob += 1e-9;
       //cout<<h<<endl;
       //cout<<prob<<endl<<endl;
        prob = prob/prob.sum();
        //cout<<prob.sum()<<endl; 1

        //prob一共numvlan=numsw列
        //这里就很麻烦了
        double pr[numvlan]={0};
        for(int i=0;i<prob.rows();i++){
            pr[i] = prob(i,0);
        }
        //假设vlan是4个。如果有变，需要手动改下面的代码
        std::discrete_distribution<int> vlan {pr[0],pr[1],pr[2],pr[3]};
        int s = vlan(generator);
        //cout<<s;
        //return x0;
        //break;
        x_now[h] = s;

//        for(int i=0;i<x_now.size();i++)
//            cout<<x_now[i]<<" ";
//        cout<<endl;
        f_now = f_cand(s,0);
        //cout<<f_now<<endl;
        if(f_now>f_best){
            f_best = f_now;
            x_best = x_now;
            R = getRoute(x_best,projMat,numhash,numvlan,numlink,numhost);
        }
        history(t,0) = f_best;
        t++;
    }
    return x_best;
}

