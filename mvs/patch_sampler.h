/*
 * Copyright (C) 2015, Ronny Klowsky, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef DMRECON_PATCH_SAMPLER_H
#define DMRECON_PATCH_SAMPLER_H

#include <map>
#include <memory>

#include "math/vector.h"
#include "mvs/defines.h"
#include "mvs/settings.h"
#include "mvs/single_view.h"

MVS_NAMESPACE_BEGIN

/**
* PatchSampler 是采样给定的三维点以及对应的patch，采样patch上的点
*/
class PatchSampler{

public:
    typedef std::shared_ptr<PatchSampler> Ptr;

public:
    /** Default constructor */
    PatchSampler();

    /** Constructor */
    PatchSampler(
        std::vector<SingleView::Ptr> const& _views,
        Settings const& _settings,
        int _x,          // pixel 在图像中的
        int _y,
        float _depth,    // 中心点处的depth
        float _dzI,      // hs(s,t)
        float _dzJ);     // ht(s,t)

    /** Smart pointer PatchSampler constructor. */
    static PatchSampler::Ptr create(std::vector<SingleView::Ptr> const& views,
        Settings const& settings, int x, int _y,
        float _depth, float _dzI, float _dzJ);

    /** Draw color samples and derivatives in neighbor view v */
    void fastColAndDeriv(std::size_t v, Samples & color, Samples& deriv);

    /** Compute NCC between reference view and a neighbor view */
    float getFastNCC(std::size_t v);

    /**获取参考视角中样本点的颜色值 */
    Samples const& getMasterColorSamples() const;

    /**获取参考视角的的平均颜色值*/
    float getMasterMeanColor() const;

    /**Patch的中心点对应的三维点*/
    math::Vec3f const & getMidWorldPoint() const;

    /** Compute NCC between two neighboring views */
    float getNCC(std::size_t u, std::size_t v);

    /** Computes the sum of absolute differences between reference
        view and neighbor v with respect to color scale cs */
    float getSAD(std::size_t v, math::Vec3f const& cs);

    /** Computes the sum of squared differences between reference
        view and neighbor v with respect to color scale cs */
    float getSSD(std::size_t v, math::Vec3f const& cs);

    /**获取邻域视角的的样本点颜色*/
    Samples const& getNeighColorSamples(std::size_t v);

    /**获取样本点的个数*/
    std::size_t getNrSamples() const;

    /**获取patch的法向量*/
    math::Vec3f getPatchNormal() const;

    /**是否采样成功*/
    bool succeeded(std::size_t v) const;

    /**
     * 根据新的newDepth, newDzI, newDzJ对patch进行更新， patch的位置和法向量都会发生变化
     * @param newDepth -- h(s,t)
     * @param newDzI -- hs(s,t)
     * @param newDzJ -- ht(s,t)
     */
    void update(float newDepth, float newDzI, float newDzJ);

    /**  */
    float varInMasterPatch();


private:
    // 所有的视角
    std::vector<SingleView::Ptr> const& views;
    Settings const& settings;

    /** precomputed mean and variance for NCC */
    math::Vec3f meanX;
    float sqrDevX;

    // patch 投影到图像中的位置
    /** patch pos in image */
    math::Vec2i midPix;
    math::Vec2i topLeft;
    math::Vec2i bottomRight;

    /** mean patch color in master image before normalization */
    float masterMeanCol;

    /** filter width = 2 * offset + 1 */
    std::size_t offset;

    size_t nrSamples;

    /** depth and encoded normal */
    float depth;
    float dzI, dzJ;

    /** viewing rays according to patch in master view */
    std::vector<math::Vec3f> masterViewDirs;

    /** 3d position of patch points */
    Samples patchPoints;  // patch上所有的点对应的3D点坐标

    /** pixel colors of patch in master image */
    Samples masterColorSamples;

    /** samples in neighbor images */
    std::map<std::size_t, Samples> neighColorSamples;    // 所有局部视角中的颜色
    std::map<std::size_t, Samples> neighDerivSamples;    // 所有局部视角的中的颜色梯度
    std::map<std::size_t, PixelCoords> neighPosSamples;  // 所有局部视角中的像素坐标

    std::map<std::size_t, float> stepSize;

    /**
     * 计算patch中所有点的3D坐标
     * */
    void computePatchPoints();

    /**
     * 计算所有采样点在refView上的颜色值
     * */
    void computeMasterSamples();

    /**
     * 计算patch中所有的点在第v个视角中的颜色值
     * @param v
     */
    void computeNeighColorSamples(std::size_t v);

public:
    std::vector<bool> success;   // 在相邻视角上是否成功
};

inline PatchSampler::Ptr
PatchSampler::create(std::vector<SingleView::Ptr> const& views
        , Settings const& settings
        , int x
        , int y
        , float depth
        , float dzI
        , float dzJ){
    return PatchSampler::Ptr(new PatchSampler
        (views, settings, x, y, depth, dzI, dzJ));
}

inline Samples const&
PatchSampler::getMasterColorSamples() const{
    return masterColorSamples;
}

inline Samples const&
PatchSampler::getNeighColorSamples(std::size_t v){
    if (neighColorSamples[v].empty())
        computeNeighColorSamples(v);
    return neighColorSamples[v];
}

inline float
PatchSampler::getMasterMeanColor() const{
    return masterMeanCol;
}

inline math::Vec3f const&
PatchSampler::getMidWorldPoint() const{
    return patchPoints[nrSamples/2];
}

inline std::size_t
PatchSampler::getNrSamples() const{
    return nrSamples;
}

inline float
PatchSampler::varInMasterPatch(){
    return sqrDevX / (3.f * (float) nrSamples);
}

MVS_NAMESPACE_END

#endif
