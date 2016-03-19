#include<Eigen/Dense>
#include<iostream>
#include<vector>
#include<fstream>
#include"markov_byte_rndngb.h"
#include"getRoute.h"
#include"flowAllocate.h"
using namespace std;
using namespace Eigen;
const int inf = 1e9;
double shuffleBytes(double opt_len,int flow_alg,int numhost,int numsw,int numlink,int numvlan,int numhash,MatrixXd C,MatrixXd projMat[],MatrixXd TMvec){
    int len = numhash*numhost*numhost;
    MatrixXd tm = TMvec;
    tm.resize(len,1);

    vector<double> tot_thr;
    tot_thr.clear();
    tot_thr.push_back(0.0);

    vector<double> opt_time;
    opt_time.clear();
    opt_time.push_back(0.0);

    double total_T = 0.0;
    double last_T = 0.0;

    vector<int> vlan_assign;
    vlan_assign.clear();
    vlan_assign = markov_byte_rndngb(tm, flow_alg, numhost, numsw, numlink,numvlan,numhash, C,projMat);
    //for(int i=0;i<vlan_assign.size();i++)
    //    cout<<vlan_assign[i]<<" ";
    //return 0;
    while(1){
        if(opt_len < 0){
            vlan_assign = markov_byte_rndngb(tm, flow_alg, numhost, numsw, numlink,numvlan,numhash, C,projMat);

        }else if(total_T > last_T + opt_len){
            vlan_assign = markov_byte_rndngb(tm, flow_alg, numhost, numsw, numlink,numvlan,numhash, C,projMat);
            last_T = total_T;
        }
        MatrixXd active = tm;
        for(int i=0;i<active.rows();i++){
            if(active(i,0)>0)
                active(i,0) = 1;
        }
        MatrixXd R;
        R = getRoute(vlan_assign,projMat,numhash,numvlan,numlink,numhost);
        MatrixXd rate;
        rate = flowAllocate(flow_alg,active,R,C);
        cout<<active.sum()<<" flows left\n";
        double duration = inf*1.0;
        bool changed = false;
        for(int i=0;i<len;i++){
            if(tm(i,0) > 0){
                double t = tm(i,0)/rate(i,0);
                if(t < duration){
                    duration = t;
                    changed = true;
                }
            }
        }
        if(changed == false)
            break;
        total_T += duration;
        for(int i=0;i<len;i++){
            if(tm(i,0) > 0)
                tm(i,0) = tm(i,0) - rate(i,0)*duration;
        }
        tot_thr.push_back(rate.sum());
        opt_time.push_back(total_T);

    }


    return total_T;
}

