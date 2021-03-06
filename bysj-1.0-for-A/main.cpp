#include<string>
#include<iostream>
#include<Eigen/Dense>
#include<vector>
#include"FloydWarshall.h"
#include"FWGetPath.h"
#include"getRoute.h"
#include"flowAllocate.h"
#include"markov_byte_rndngb.h"
#include<random>
#include<fstream>
using namespace Eigen;
using namespace std;
const int inf = 1e9;
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

    int numlink = Adj.sum()/2;
   // L2N = L2N.topRows(numlink);
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

    //probSetup();
    MatrixXd projMat[numvlan];
    for(int v=0;v<numvlan;v++){
        MatrixXd next(numhost,numhost);
        next = FloydWarshall(VLAN[v]);
        //projMat[v] = FWAllPath(next);
        MatrixXd out;
        out = MatrixXd::Zero(numlink*2,numhost*numhost);
        for(int n1=0;n1<numhost;n1++){
            for(int n2=0;n2<numhost;n2++){
                int col = n1*numhost + n2;
                vector<int> nodepath = FWGetPath(next,n1,n2);
                int len = nodepath.size();
                for(int i=0;i<len/2;i++){
                    int link = N2L(nodepath[2*i],nodepath[2*i+1]);
                    out(link,col) = 1;
                }
            }

        }
        projMat[v] = out;
        //test 正确
        //cout<<projMat[0].rows()<<endl;
        //cout<<projMat[0]<<endl;

    }
    //test 正确
//    vector<int> nodepath = FWGetPath(FloydWarshall(VLAN[0]),0,3);
//    int len=nodepath.size();
//    for(int i=0;i<len;i++)
//        cout<<nodepath[i]<<" ";
    //total_time = shuffleBytes(-1, 1, 'thr_uniform_per.txt');
    //save('test_main.mat','total_time');

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
    //test 数据正确
   // cout<<TMvec<<endl;
//    MatrixXd temp(3,4);
//    temp<<1,2,3,4,
//         5,6,7,8,
//         9,10,11,12;
//    temp.resize(4,3);
//    cout<<temp;


    //shuffleBytes
    double opt_len = 10.0;
    int flow_alg = 1;
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
    cout<<"tot_thr"<<endl;
    for(int i=0;i<tot_thr.size();i++){
        cout<<tot_thr[i]<<" ";
    }
    cout<<endl<<"opt_time"<<endl;
    for(int i=0;i<opt_time.size();i++){
        cout<<opt_time[i]<<" ";
    }
    ofstream outfile1;
    outfile1.open("C:\\tot_thr_10s.txt");
    for(vector<double>::iterator it=tot_thr.begin();it!=tot_thr.end();it++){
        outfile1<<*it<<" ";
    }
    outfile1.close();

    ofstream outfile2;
    outfile2.open("C:\\opt_time_10s.txt");
    for(vector<double>::iterator it=opt_time.begin();it!=opt_time.end();it++){
        outfile2<<*it<<" ";
    }
    outfile2.close();
    return 0;
}
