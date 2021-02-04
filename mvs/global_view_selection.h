/*
 * Copyright (C) 2015, Ronny Klowsky, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef DMRECON_GLOBAL_VIEW_SELECTION_H
#define DMRECON_GLOBAL_VIEW_SELECTION_H

#include <map>

#include <mvs/defines.h>
#include "core/bundle.h"
#include "mvs/single_view.h"
#include "mvs/view_selection.h"
#include "mvs/settings.h"

MVS_NAMESPACE_BEGIN

/**
 * 全局视角选择为每个一帧参考图像R选取N=10幅相邻帧作为候选用于立体匹配
 * 全局视角的选择会考虑到图像内容，图像尺度，同时也需要保证相邻视角和参考R之间的石碴足够大
 */
class GlobalViewSelection : public ViewSelection{
public:
    /**
     *  Constructor
     * @param views     -- 传入所有的视角
     * @param features  -- SFM生成的特征（Tracks)
     * @param settings  -- 参数设置，包含参考帧，以及要选取的候选帧的个数
     */
    GlobalViewSelection(std::vector<SingleView::Ptr> const& views,
        core::Bundle::Features const& features,
        Settings const& settings);

    /**进行全局视角选择**/
    void performVS();

private:

    /**
     * 计算参考帧和第i个视角之间的score
     * @param i
     * @return
     */
    float benefitFromView(std::size_t i);

    std::vector<SingleView::Ptr> const& views;
    core::Bundle::Features const& features;
};

MVS_NAMESPACE_END

#endif
