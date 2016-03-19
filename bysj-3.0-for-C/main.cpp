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
#include"gp.h"
using namespace Eigen;
using namespace std;

int main()
{


    int flow_alg = 1;
    //generalizedProblem
    //double total_T = 0;
    double opt_len = -1;

    int trials = 45;//
    for(int i=0;i<trials;i++){
        ofstream out1,out2,out3,out4,out6,out12;
        double t1 = gp(opt_len,1,flow_alg);
        double t2 = gp(opt_len,2,flow_alg);
        double t3 = gp(opt_len,3,flow_alg);
        double t4 = gp(opt_len,4,flow_alg);
        double t6 = gp(opt_len,6,flow_alg);
        double t12 = gp(opt_len,12,flow_alg);
        out1.open("C:\\c_per_N12.txt",ios::app);
        out2.open("C:\\c_per_N6.txt",ios::app);
        out3.open("C:\\c_per_N4.txt",ios::app);
        out4.open("C:\\c_per_N3.txt",ios::app);
        out6.open("C:\\c_per_N2.txt",ios::app);
        out12.open("C:\\c_per_N1.txt",ios::app);

        if(i<trials-1)
            out1<<t1<<" ";
        else
            out1<<t1;

        if(i<trials-1)
            out2<<t2<<" ";
        else
            out2<<t2;

        if(i<trials-1)
            out3<<t3<<" ";
        else
            out3<<t3;

        if(i<trials-1)
            out4<<t4<<" ";
        else
            out4<<t4;

        if(i<trials-1)
            out6<<t6<<" ";
        else
            out6<<t6;

        if(i<trials-1)
            out12<<t12<<" ";
        else
            out12<<t12;

        out1.close();out2.close();out3.close();out4.close();out6.close();out12.close();
    }
    //cout<<gp(opt_len,2,flow_alg);
    return 0;
}
