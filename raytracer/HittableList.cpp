#include "HittableList.h"

// �жϹ����Ƿ��������н���
bool HittableList::hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const {
    hitRecord tempRec; // ��ʱ�� hitRecord ����
    bool hitAnything = false; // �Ƿ��н���
    auto closestSoFar = t_max; // ����Ľ���

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}