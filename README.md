## ImageBasedModellingEduV2.0
ImageBasedModellingEdu2.0 是用于深蓝学院基于图像的三维模型重建课程配套的代码。该代码来源于著名的开源三维重建开源系统MVE(https://github.com/simonfuhrmann/mve)。
我们其基础之上对代码的架构进行了调整，使其与课程更为相关，有助于阅读和学习。该工程项目采用CMake管理，可与方便的进行跨平台的编译。代码包含特征提取与匹配、对极几何、运动恢复结构、稠密重建、表面重建以及纹理贴图、可视化等模块，将随着课程的深入不断进行更新。

## 架构
该工程主要包含core, math, util, features, sfm, mvs, surface, texturing等主要模块，其中：
- core—提供了工程项目需要的所有的基础数据结构，包括image, depthmap, mesh, view,以及数据的输入输出等结构和功能；
- math—提供矩阵，向量，四元数等基本的数学运算操作；
- features—提供特征提取以及特征匹配功能，其中特征类型包括sift和surf两种；
- sfm—提供了与运动恢复结构相关的功能，包括相机姿态的恢复，三维点的三角化和捆绑调整等；
- mvs—提供立体匹配功能，实现稠密点云匹配；
- surface—实现点云到网格的表面重建；
- texturing—实现纹理图像的创建；
- examples—提供一些关键模块的示例代码；
- tmp—存储临时数据

## 编译（Mac和Linux下没有问题，Window下的编译未经过验证）
1.安装依赖库包含libpng, libjpeg, libtiff, eigen

 ### Linux
 sudo apt-get install libjpeg-dev
 
 sudo apt-get install libtiff-dev
 
 ### Mac
 brew install libpng 
 
 brew install libjpeg
 
 brew install libtiff
 
2.执行

    git clone https://github.com/weisui-ad/ImageBasedModellingEdu.git
    
    cd ~/ImageBasedModellingEdu
    
    mkdir build && cd build
    
    cmake -DCMAKE_BUILD_TYPE=Release .. 
    
    make -j8

3. 示例代码examples/

   ./build/examples/task3/task3-1_incremental_sfm ./examples/data/sequence ./examples/data/sequence_scene

 
