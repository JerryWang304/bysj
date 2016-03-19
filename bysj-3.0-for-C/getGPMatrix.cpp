#include<iostream>
#include<ctime>
#include<random>
#include<cstdlib>
#include<algorithm>
#include<Eigen/Dense>
#include<vector>
using namespace std;
using namespace Eigen;
MatrixXd getGPMatrix(int sub_number,MatrixXd otm,int numhost,int numsw,int numvlan,int numhash){
    int old_numhash = numhash;
    numhash = numhash*sub_number;
    MatrixXd tm_temp;
    tm_temp = MatrixXd::Zero(4*(numhash+1)+1,4+1);
    //根据sub_number生成新的流量矩阵
    int original_1_entry[] = {0,2,3,4,5,7,8,9,10,12,13,14,15};

    int flow_per_class = 12/sub_number;
    for(int i=1;i<=old_numhash;i++){
        vector<int> shuffle;
        shuffle.clear();
        for(int k=1;k<=12;k++)
            shuffle.push_back(k);

        //permutation
        srand (unsigned( std::time(0)));//保证每次运行，结果都不一样
        std::random_shuffle(shuffle.begin(),shuffle.end());
        //这里纯粹是为了让下标从1开始
        shuffle.insert(shuffle.begin(),0);

        for(int j=1;j<=sub_number;j++){
            MatrixXd sub_matrix;
            sub_matrix = MatrixXd::Zero(5,5);

            for(int k=1;k<=flow_per_class;k++){
                int index = original_1_entry[shuffle[(j-1)*flow_per_class+k]];
                //
                if(index%4 == 0)
                    sub_matrix(floor(index/4),4) = otm(floor(index/4)-1,3);
                else
                    sub_matrix(floor(index/4)+1,index%4) = otm(floor(index/4),index%4-1);
            }
            for(int r=(i-1)*sub_number*4+(j-1)*4+1;r<=(i-1)*sub_number*4+(j-1)*4+4;r++){
                for(int c=1;c<=4;c++){
                    tm_temp(r,c) = sub_matrix(r-((i-1)*sub_number*4+(j-1)*4),c);
                }
            }
            //cout<<"j = "<<j<<endl<<sub_matrix<<endl<<endl;
        }

    }
    for(int r = 4*numhash+1;r<=4*numhash+4;r++){
        for(int c=1;c<=4;c++){
            if(r-4*numhash == c)
                tm_temp(r,c) = 0;
            else
                tm_temp(r,c) = 0.08;
        }

    }
    //cout<<"tm_temp= "<<tm_temp<<endl<<endl;
    //用这种方法好傻
    MatrixXd tm;
    tm = MatrixXd::Zero(4*(numhash+1),4);
    for(int r=0;r<4*(numhash+1);r++){
        for(int c=0;c<4;c++){
            tm(r,c) = tm_temp(r+1,c+1);
        }
    }

    return tm;

}

