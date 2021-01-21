//
// Created by sway on 2021/1/19.
//
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "sfm/ransac_pose_p3p.h"

int main(int argc, char* argv[]){


    // 相机内参矩阵
    math::Matrix<double, 3, 3>k_matrix;
    k_matrix.fill(0.0);
    k_matrix[0] = 0.972222;
    k_matrix[2] = 0.0; // cx =0 图像上的点已经进行了归一化（图像中心为原点，图像尺寸较长的边归一化为1）
    k_matrix[4] = 0.972222;
    k_matrix[5] = 0.0; // cy=0  图像上的点已经进行了归一化（图像中心为原点，图像尺寸较长的边归一化为1）
    k_matrix[8] = 1.0;

    // 从文件中读取3D-2D对应点，2D点已经进行归一化
    sfm::Correspondences2D3D corrs;
    std::ifstream fin("./examples/task2/correspondence2D3D.txt");
    assert(fin.is_open());
    std::string line;
    int line_id = 0;
    int n_pts = 0;
    while(getline(fin, line)){
        std::stringstream  stream(line);
        if(line_id==0){
            stream>>n_pts;
            //std::cout<<"n_pts: "<<n_pts<<std::endl;
            line_id++;
            continue;
        }
        sfm::Correspondence2D3D corr;
        stream>>corr.p3d[0]>>corr.p3d[1]>>corr.p3d[2]>>corr.p2d[0]>>corr.p2d[1];
        corrs.push_back(corr);
        //std::cout<<corr.p3d[0]<<" "<<corr.p3d[1]<<" "
        //<<corr.p3d[2]<<" "<<corr.p2d[0]<<" "<<corr.p2d[1]<<std::endl;
    }

    // Ransac中止条件，内点阈判断
    sfm::RansacPoseP3P::Options pose_p3p_opts;
    // Ransac估计相机姿态
    sfm::RansacPoseP3P::Result ransac_result;
    sfm::RansacPoseP3P ransac(pose_p3p_opts);

    ransac.estimate(corrs, k_matrix, &ransac_result);

    std::cout<<"2D-3D correspondences inliers: "<< (100 * ransac_result.inliers.size() / corrs.size())<<std::endl;
    std::cout<<"Estimated pose: "<<std::endl;
    std::cout<<ransac_result.pose<<std::endl;

    std::cout<<"The result pose should be:"<<std::endl;
    std::cout<<
    "0.99896 0.0341342 -0.0302263 -0.292601\n"
    "-0.0339703 0.999405 0.0059176 -4.6632\n"
    "0.0304104 -0.00488465 0.999526 -0.0283862\n"<<std::endl;

    return 0;
}

