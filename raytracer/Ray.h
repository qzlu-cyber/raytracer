#pragma once
#include "Vec3.h"

class Ray
{
private:
    Vec3 ori; // �������
    Vec3 dir; // ���߷���
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction)
        : ori(origin), dir(direction)
    {}

    Vec3 origin() const { return ori; }
    Vec3 direction() const { return dir; }

    // ������ t ʱ�̵�λ��
    Vec3 at(double t) const {
        return ori + t * dir;
    }
};

