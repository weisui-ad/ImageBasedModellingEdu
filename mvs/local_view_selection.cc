/*
 * Copyright (C) 2015, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include "math/defines.h"
#include "math/functions.h"
#include "mvs/local_view_selection.h"
#include "mvs/mvs_tools.h"
#include "mvs/patch_sampler.h"
#include "mvs/settings.h"

MVS_NAMESPACE_BEGIN

LocalViewSelection::LocalViewSelection(
    std::vector<SingleView::Ptr> const& views,
    Settings const& settings,
    IndexSet const& globalViewIDs,
    IndexSet const& propagated,
    PatchSampler::Ptr sampler):
    ViewSelection(settings),
    success(false),
    views(views),
    sampler(sampler){

    // inherited attribute
    this->selected = propagated;

    /*patch采样在参考视角中失败*/
    if (!sampler->success[settings.refViewNr]) {
        return;
    }

    /**
     * 选择的视角个数等于设定的个数
     */
    if (selected.size() == settings.nrReconNeighbors)
        success = true;

    else if (selected.size() > settings.nrReconNeighbors) {
        std::cerr << "ERROR: Too many local neighbors propagated!" << std::endl;
        selected.clear();
    }

    /**将所有视角的flag设置成false**/
    available.clear();
    available.resize(views.size(), false);

    /**将所有global的视角设置成true**/
    IndexSet::const_iterator id;
    for (id = globalViewIDs.begin(); id != globalViewIDs.end(); ++id) {
        available[*id] = true;
    }

    /**将所有的已经选择的视角设置成false**/
    IndexSet::const_iterator sel;
    for (sel = selected.begin(); sel != selected.end(); ++sel) {
        available[*sel] = false;
    }
}

void LocalViewSelection::performVS(){

    /**
     * 如果选择的视角个数等于设定的个数，则不需要再进行局部视角选择
     */
    if (selected.size() == settings.nrReconNeighbors) {
        success = true;
        return;
    }

    /*获取参考视角*/
    SingleView::Ptr refV = views[settings.refViewNr];

    /*获取patch的中心点*/
    math::Vec3f p(sampler->getMidWorldPoint());

    // pixel print in reference view
    float mfp = refV->footPrintScaled(p);
    math::Vec3f refDir = (p - refV->camPos).normalized();

    /*计算patch在参考视角和其他所有视角的的ncc, 极平面的法向量*/
    std::map<std::size_t, math::Vec3f> viewDir;
    std::map<std::size_t, math::Vec3f> epipolarPlane; // plane normal
    std::map<std::size_t, float> ncc;
    for (std::size_t i = 0; i < views.size(); ++i) {
        if (!available[i])
            continue;
        /*计算patch在第i个视角和参考视角的NCC值*/
        float tmpNCC = sampler->getFastNCC(i);

        /*如果NCC的值小于设定的阈值*/
        assert(!MATH_ISNAN(tmpNCC));
        if (tmpNCC < settings.minNCC) {
            available[i] = false;
            continue;
        }

        ncc[i] = tmpNCC;

        /*计算极平面之间的法向量*/
        viewDir[i] = (p - views[i]->camPos).normalized();
        epipolarPlane[i] = (viewDir[i].cross(refDir)).normalized();
    }

    /*对于所有的已经选择的视角，计算极平面的法向量*/
    IndexSet::const_iterator sel;
    for (sel = selected.begin(); sel != selected.end(); ++sel) {
        viewDir[*sel] = (p - views[*sel]->camPos).normalized();
        epipolarPlane[*sel] = (viewDir[*sel].cross(refDir)).normalized();
    }


    bool foundOne = true;
    while (selected.size() < settings.nrReconNeighbors && foundOne){
        foundOne = false;
        std::size_t maxView = 0;
        float maxScore = 0.f;
        for (std::size_t i = 0; i < views.size(); ++i) {
            if (!available[i])
                continue;
            float score = ncc[i];

            // 1. 考虑分辨率差异--计算参考视角和第i个视角的尺度
            // resolution difference
            float nfp = views[i]->footPrint(p);
            if (mfp / nfp < 0.5f) {  //todo  第i个视角的分辨率高于参考视角分分辨率？？
                score *= 0.01f;
            }

            // 2. 考虑视线之间的夹角
            // parallax w.r.t. reference view
            float dp = math::clamp(refDir.dot(viewDir[i]), -1.f, 1.f);
            float plx = std::acos(dp) * 180.f / pi;
            score *= parallaxToWeight(plx);
            assert(!MATH_ISNAN(score));

            /*3. 考虑和其它已经选择的视角之间的关联*/
            for (sel = selected.begin(); sel != selected.end(); ++sel) {
                // parallax w.r.t. other selected views
                dp = math::clamp(viewDir[*sel].dot(viewDir[i]), -1.f, 1.f);
                plx = std::acos(dp) * 180.f / pi;
                score *= parallaxToWeight(plx);

                // epipolar geometry
                dp = epipolarPlane[i].dot(epipolarPlane[*sel]);
                dp = math::clamp(dp, -1.f, 1.f);
                float angle = std::abs(std::acos(dp) * 180.f / pi);
                if (angle > 90.f)
                    angle = 180.f - angle;

                angle = std::max(angle, 1.f);
                if (angle < settings.minParallax)
                    score *= angle / settings.minParallax;
                assert(!MATH_ISNAN(score));
            }

            /*如果score> maxScore*/
            if (score > maxScore) {
                foundOne = true;
                maxScore = score;
                maxView = i;
            }
        }

        if (foundOne) {
            selected.insert(maxView);
            available[maxView] = false;
        }
    }

    /*如果选择的视角个数等于设定的个数，则表示选择成功*/
    if (selected.size() == settings.nrReconNeighbors) {
        success = true;
    }
}

void
LocalViewSelection::replaceViews(IndexSet const & toBeReplaced){

    IndexSet::const_iterator tbr = toBeReplaced.begin();
    while (tbr != toBeReplaced.end()) {
        available[*tbr] = false;
        selected.erase(*tbr);
        ++tbr;
    }
    success = false;
    performVS();
}


MVS_NAMESPACE_END
