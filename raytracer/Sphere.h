#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> matPtr;

public:
    Sphere() {}
    Sphere(Vec3 cen, double r, std::shared_ptr<Material> mat_ptr) : center(cen), radius(r), matPtr(mat_ptr) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const override;
};

