#pragma once

#include "Ray.h"

class Material;

/*
    光线会如何与表面交互是由具体的材质所决定的。hitRecord 在设计上就是为了把一堆要传的参数给打包在了一起。
    当光线射入一个表面(比如一个球体), hitRecord 中的材质指针会被球体的材质指针所赋值, 而球体的材质指针
    是在 main() 函数中构造时传入的。当 color() 函数获取到 hitRecord 时, 他可以找到这个材质的指针, 
    然后由材质的函数来决定光线是否发生散射, 怎么散射。
*/
struct hitRecord {
    Vec3 p; // 交点
    Vec3 normal; // 交点处的法向量
    double t; // 交点处的 t 值
    bool frontFace; // 是否是从正面(物体外面)打到物体的光线
    std::shared_ptr<Material> matPtr; // 交点处的材质

    // 设置法向量的朝向
    inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal; // 如果是从正面打到物体, 法向量朝外; 否则, 法向量朝内
    }
};

class Hittable {
public:
    // 判断光线是否与物体有交点
    // t_min: 光线的起点, t_max: 光线的终点
    virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const = 0;
};

