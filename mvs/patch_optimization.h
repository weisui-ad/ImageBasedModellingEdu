/*
 * Copyright (C) 2015, Ronny Klowsky, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef DMRECON_PATCH_OPTIMIZATION_H
#define DMRECON_PATCH_OPTIMIZATION_H

#include <iostream>

#include "math/vector.h"
#include "mvs/defines.h"
#include "mvs/patch_sampler.h"
#include "mvs/single_view.h"
#include "mvs/local_view_selection.h"

MVS_NAMESPACE_BEGIN

struct Status{
    std::size_t iterationCount;   // 最大迭代次数
    bool converged;               // 是否收敛
    bool optiSuccess;             //
};


/**
 * \description 对patch的深度进行优化
 */
class PatchOptimization{

public:

    /**
     * Constructor
     * @param _views
     * @param _settings
     * @param _x
     * @param _y
     * @param _depth
     * @param _dzI
     * @param _dzJ
     * @param _globalViewIDs
     * @param _localViewIDs
     */
    PatchOptimization(
        std::vector<SingleView::Ptr> const& _views,
        Settings const& _settings,
        int _x,          // Pixel position  像素位置
        int _y,
        float _depth,    // depth的深度
        float _dzI,      // hs(s,t)
        float _dzJ,      // ht(s.t)
        IndexSet const& _globalViewIDs,   // 全局的视角
        IndexSet const& _localViewIDs);   // 局部视角


    /**
     * 计算颜色尺度
     */
    void computeColorScale();

    /**
     * 计算置信度
     * @return
     */
    float computeConfidence();

    float derivNorm();

    /**
     * 自动进行优化
     */
    void doAutoOptimization();

    /**
     * 获取depth
     * @return
     */
    float getDepth() const;

    /**
     * 获取hs(s,t)
     * @return
     */
    float getDzI() const;

    /**
     * 获取ht(s,t)
     * @return
     */
    float getDzJ() const;

    /**
     * 获取局部视角的索引
     * @return
     */
    IndexSet const& getLocalViewIDs() const;

    /**
     * 通过path 3D点的坐标，计算patch的法向量
     * @return
     */
    math::Vec3f getNormal() const;
    float objFunValue();

    /**
     * 仅优化depth
     */
    void optimizeDepthOnly();

    /**
     * 同时优化depth和normal
     */
    void optimizeDepthAndNormal();

private:
    std::vector<SingleView::Ptr> const& views;
    Settings const& settings;

    // initial values and settings
    const int midx;
    const int midy;

    float depth;     // depth 值
    float dzI, dzJ;  // represents patch normal
    std::map<std::size_t, math::Vec3f, std::less<std::size_t> > colorScale;  // 每个视角的颜色尺度

    Status status;

    /**
     * patch sampler
     */
    PatchSampler::Ptr sampler;

    // patch点的x 和 y 坐标
    std::vector<int> ii, jj;  // ii, jj坐标

    /**
     * 像素的权重值
     */
    std::vector<float> pixel_weight;

    /**
     * 局部视角选取
     */
    LocalViewSelection localVS;
};

inline float
PatchOptimization::getDepth() const{
    return depth;
}

inline float
PatchOptimization::getDzI() const{
    return dzI;
}

inline float
PatchOptimization::getDzJ() const
{
    return dzJ;
}

inline IndexSet const&
PatchOptimization::getLocalViewIDs() const{
    return localVS.getSelectedIDs();
}

inline math::Vec3f
PatchOptimization::getNormal() const{
    return sampler->getPatchNormal();
}

MVS_NAMESPACE_END

#endif
