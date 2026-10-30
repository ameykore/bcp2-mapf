#include "problem/map3d.h"

Map3D::Map3D(const Position width, const Position height, const Position elevation) : elevation_(elevation + 2)
{
    resize(width + 2, height + 2, elevation + 2);
}

void Map3D::resize(const Position width, const Position height, const Position elevation)
{
    DEBUG_ASSERT(empty()); // not sure what this is. 

    passable_.resize(width * height * elevation, false);

    width_ = width;
    height_ = height;
}
