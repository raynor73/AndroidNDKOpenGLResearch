//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_LAYOUT_PARAMS_H
#define ENGINE_3D_LAYOUT_PARAMS_H


#include <memory>
#include <game/UnitsConverter.h>
#include "ViewBoundsComponent.h"

enum class VerticalLayoutType {
    TOP_INSIDE, TOP_OUTSIDE, CENTER, BOTTOM_INSIDE, BOTTOM_OUTSIDE
};

enum class HorizontalLayoutType {
    LEFT_INSIDE, LEFT_OUTSIDE, CENTER, RIGHT_INSIDE, RIGHT_OUTSIDE
};

enum class OriginVerticalLayoutType {
    TOP, CENTER, BOTTOM
};

enum class OriginHorizontalLayoutType {
    LEFT, CENTER, RIGHT
};

struct LayoutParams {
    ComplexValue paddingLeft;
    ComplexValue paddingTop;
    ComplexValue paddingRight;
    ComplexValue paddingBottom;
    VerticalLayoutType verticalLayoutType;
    HorizontalLayoutType horizontalLayoutType;
    OriginVerticalLayoutType originVerticalLayoutType;
    OriginHorizontalLayoutType originHorizontalLayoutType;
    std::shared_ptr<ViewBoundsComponent> referenceViewBounds;
};


#endif //ENGINE_3D_LAYOUT_PARAMS_H
