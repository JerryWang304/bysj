#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <chrono>
#include<random>
using namespace Eigen;
using namespace std;
MatrixXd get_knock_tm(MatrixXd tm,int knock,MatrixXd ori_tm)
{
    MatrixXd knock_tm;
    knock_tm = MatrixXd::Zero(128,1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0,3);
    for(int i=0;i<32;i++){
        if(knock == 1){
            int index = distribution(generator);
            //cout<<"index = "<<index<<endl;
            //int cur_index ;
            int knock_index = 0;

            for(int j=0;j<4;j++){
                if(ori_tm(4*i+j,0) > 0){
                    //cur_index++;
                    //cout<<cur_index<<endl
                    //cur_index = j;
                    if(j == index){
                        knock_tm(4*i+j,0) = 0;// knock out
                        knock_index = j;
                        //cout<<"Hi"<<" "<<j<<endl;
                    }else{
                        knock_tm(4*i+j,0) = tm(4*i+j,0);
                    }
                }else{
                    knock_tm(4*i+j,0) = tm(4*i+j,0);
                }
            }

            //estimate
            double sum=0;
            for(int k=4*i;k<=4*i+3;k++){
                sum+=knock_tm(k,0);
            }
            knock_tm(i*4+knock_index,0) = sum/2;
            //cout<<"sum = "<<sum<<endl;
        }//k=2的就不写了，反正也没用到
    }
    //cout<<knock_tm;
    return knock_tm;
}

