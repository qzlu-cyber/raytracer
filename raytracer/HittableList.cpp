#include "HittableList.h"

// 判断光线是否与物体有交点
bool HittableList::hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const {
    hitRecord tempRec; // 临时的 hitRecord 对象
    bool hitAnything = false; // 是否有交点
    auto closestSoFar = t_max; // 最近的交点

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}