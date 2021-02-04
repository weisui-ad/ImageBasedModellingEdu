/*
 * Copyright (C) 2015, Ronny Klowsky, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef DMRECON_VIEW_SELECTION_H
#define DMRECON_VIEW_SELECTION_H

#include <memory>

#include "mvs/defines.h"
#include "mvs/settings.h"

MVS_NAMESPACE_BEGIN

/**
 * class for view selection
 */
class ViewSelection{

public:
    typedef std::shared_ptr<ViewSelection> Ptr;

    /**
     * constructor
     */
    ViewSelection();

    /**
     * constructor with settings
     * @param settings
     */
    ViewSelection(Settings const& settings);

public:
    IndexSet const& getSelectedIDs() const;

protected:

    Settings const& settings;     // 所有的参数
    std::vector<bool> available;  // flag--用于指示视角是否可用，默认除了参考视角全部都是可用
    IndexSet selected;           // 选择的视角的索引
};


inline
ViewSelection::ViewSelection(Settings const& settings)
    :
    settings(settings){
}

inline IndexSet const&
ViewSelection::getSelectedIDs() const
{
    return selected;
}

MVS_NAMESPACE_END

#endif
