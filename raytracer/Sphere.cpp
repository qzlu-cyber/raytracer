#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const {
    Vec3 oc = r.origin() - center; // 光线起点到圆心的向量
    auto a = dot(r.direction(), r.direction()); // 光线的方向向量的长度的平方
    auto b = 2 * dot(oc, r.direction()); // 光线的方向向量和光线起点到圆心的向量的点积
    auto c = dot(oc, oc) - radius * radius; // 光线起点到圆心的向量的长度的平方减去圆的半径的平方
    auto discriminant = b * b - 4 * a * c; // 根据 Δ 判断光线和圆是否有交点

    if (discriminant > 0) {
        auto temp = (-b - sqrt(discriminant)) / (2 * a); // 有交点时, 取较小的那个

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = unitVector(rec.p - center);

            Vec3 outwardNormal = (rec.p - center) / radius; // 计算交点处的法向量
            rec.setFaceNormal(r, outwardNormal); // 设置法向量的朝向

            rec.matPtr = matPtr; // 交点处的材质

            return true;
        }

        temp = (-b + sqrt(discriminant)) / (2 * a); // 有交点时, 取较大的那个

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = unitVector(rec.p - center);

            Vec3 outwardNormal = (rec.p - center) / radius; // 计算交点处的法向量
            rec.setFaceNormal(r, outwardNormal); // 设置法向量的朝向

            rec.matPtr = matPtr; // 交点处的材质

            return true;
        }
    }

    return false;
}