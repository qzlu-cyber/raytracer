#pragma once

#include "Vec3.h"
#include "Ray.h"

class Camera
{
private:
    Vec3 origin; // �����λ��
    Vec3 lowerLeftCorner; // ��Ļ�����½�
    Vec3 horizontal; // ��Ļ��ˮƽ����
    Vec3 vertical; // ��Ļ�Ĵ�ֱ����
    double lensRadius;
    Vec3 u, v, w;

public:
    Camera() {
        lowerLeftCorner = Vec3(-2.0, -1.0, -1.0);
        horizontal = Vec3(4.0, 0.0, 0.0);
        vertical = Vec3(0.0, 2.0, 0.0);
        origin = Vec3(0.0, 0.0, 0.0);
    }

    Camera(double vfov, double aspect, Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, double aperture, double focusDist) {
        double theta = degreesToRadians(vfov); // �Ƕ�ת����
        double halfHeight = tan(theta / 2); // ��Ļ�ĸ߶�
        double halfWidth = aspect * halfHeight; // ��Ļ�Ŀ��

        lensRadius = aperture / 2;

        w = unitVector(lookFrom - lookAt); // ����ķ���
        u = unitVector(cross(vUp, w)); // ������ҷ���
        v = cross(w, u); // ������Ϸ���

        origin = lookFrom;
        lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w; // ��Ļ�����½�
        horizontal = 2 * halfWidth * focusDist * u; // ��Ļ��ˮƽ����
        vertical = 2 * halfHeight * focusDist * v;
    }

    Ray getRay(double s, double t) {
        Vec3 rd = lensRadius * randomInUnitDisk(); // �������һ��Բ���ڵĵ�
        Vec3 offset = u * rd.x() + v * rd.y(); // ����һ�������ƫ����

        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }
};

