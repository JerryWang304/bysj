#include<Eigen/Dense>
#include<Eigen/Dense>
#include<iostream>
#include<vector>
#include<fstream>
#include"markov_byte_rndngb.h"
#include"getRoute.h"
#include"flowAllocate.h"
#include"get_knock_tm.h"
using namespace Eigen;
using namespace std;
const int inf = 1e9;
double shuffleBytesKnock(MatrixXd ori_tm,int knock,double opt_len,int flow_alg,int numhost,int numsw,int numlink,int numvlan,int numhash,MatrixXd C,MatrixXd projMat[],MatrixXd TMvec){
    int len = numhash*numhost*numhost;
    MatrixXd tm;
    tm = TMvec;
    tm.resize(len,1);
    double total_T = 0;
    double last_T = 0;
    MatrixXd knock_tm;

    knock_tm = get_knock_tm(tm,knock,ori_tm);

    vector<int> vlan_assign;
    vlan_assign.clear();
    vlan_assign = markov_byte_rndngb(tm, flow_alg, numhost, numsw, numlink,numvlan,numhash, C,projMat);

    while(1){
        if(opt_len<0){
            knock_tm = get_knock_tm(tm,knock,ori_tm);
            vlan_assign = markov_byte_rndngb(tm, flow_alg, numhost, numsw, numlink,numvlan,numhash, C,projMat);
        }else if(total_T > last_T + opt_len){
            knock_tm = get_knock_tm(tm,knock,ori_tm);
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

    }
    return total_T;

}

