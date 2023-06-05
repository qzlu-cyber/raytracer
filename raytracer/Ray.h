#pragma once
#include "Vec3.h"

class Ray
{
private:
    Vec3 ori; // 光线起点
    Vec3 dir; // 光线方向
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction)
        : ori(origin), dir(direction)
    {}

    Vec3 origin() const { return ori; }
    Vec3 direction() const { return dir; }

    // 光线在 t 时刻的位置
    Vec3 at(double t) const {
        return ori + t * dir;
    }
};

