/*
 * Copyright (C) 2015, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef DMRECON_SETTINGS_H
#define DMRECON_SETTINGS_H

#include <stdexcept>
#include <string>
#include <limits>

#include "math/vector.h"
#include "mvs/defines.h"

MVS_NAMESPACE_BEGIN

struct Settings
{
    /** The reference view ID to reconstruct. */
    std::size_t refViewNr = 0;

    /** Input image emebdding. */
    std::string imageEmbedding = "undistorted";

    /** Size of the patch is width * width, defaults to 5x5. */
    unsigned int filterWidth = 5;

    /**NCC的阈值，低于此值则说明匹配是无效的**/
    float minNCC = 0.3f;

    /**计算特征点在不同视角中的射线构成的夹脚**/
    float minParallax = 10.0f;

    /**可以接受的NCC的值**/
    float acceptNCC = 0.6f;

    float minRefineDiff = 0.001f;

    /**depth和normal优化时的最大迭代次数**/
    unsigned int maxIterations = 20;

    /**局部视角的个数**/
    unsigned int nrReconNeighbors = 4;

    /**全局视角global view 最大设置为20个**/
    unsigned int globalVSMax = 20;

    /**图像的尺度**/
    int scale = 0;

    /**是否采用颜色空间的尺度对图像**/
    bool useColorScale = true;
    bool writePlyFile = false;

    /** Features outside the AABB are ignored. */
    math::Vec3f aabbMin = math::Vec3f(-std::numeric_limits<float>::max());
    math::Vec3f aabbMax = math::Vec3f(std::numeric_limits<float>::max());

    std::string plyPath;

    bool keepDzMap = false;
    bool keepConfidenceMap = false;
    bool quiet = false;
};

MVS_NAMESPACE_END

#endif /* DMRECON_SETTINGS_H */
