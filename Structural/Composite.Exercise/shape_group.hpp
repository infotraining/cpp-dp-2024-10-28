#ifndef SHAPEGROUP_HPP
#define SHAPEGROUP_HPP

#include <memory>
#include <vector>

#include "shape.hpp"

namespace Drawing
{
    // TODO - implement a composite for shapes
    class ShapeGroup : public Shape
    {
        std::vector<std::unique_ptr<Shape>> shapes_;
    };
}

#endif // SHAPEGROUP_HPP
