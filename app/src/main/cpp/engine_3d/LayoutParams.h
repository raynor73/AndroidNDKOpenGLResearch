//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_LAYOUT_PARAMS_H
#define ENGINE_3D_LAYOUT_PARAMS_H


#include <memory>
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
    int paddingLeft;
    int paddingTop;
    int paddingRight;
    int paddingBottom;
    VerticalLayoutType verticalLayoutType;
    HorizontalLayoutType horizontalLayoutType;
    OriginVerticalLayoutType originVerticalLayoutType;
    OriginHorizontalLayoutType originHorizontalLayoutType;
};


#endif //ENGINE_3D_LAYOUT_PARAMS_H
