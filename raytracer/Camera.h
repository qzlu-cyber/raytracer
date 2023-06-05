#pragma once

#include "Vec3.h"
#include "Ray.h"

class Camera
{
private:
    Vec3 origin; // 相机的位置
    Vec3 lowerLeftCorner; // 屏幕的左下角
    Vec3 horizontal; // 屏幕的水平方向
    Vec3 vertical; // 屏幕的垂直方向
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
        double theta = degreesToRadians(vfov); // 角度转弧度
        double halfHeight = tan(theta / 2); // 屏幕的高度
        double halfWidth = aspect * halfHeight; // 屏幕的宽度

        lensRadius = aperture / 2;

        w = unitVector(lookFrom - lookAt); // 相机的方向
        u = unitVector(cross(vUp, w)); // 相机的右方向
        v = cross(w, u); // 相机的上方向

        origin = lookFrom;
        lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w; // 屏幕的左下角
        horizontal = 2 * halfWidth * focusDist * u; // 屏幕的水平方向
        vertical = 2 * halfHeight * focusDist * v;
    }

    Ray getRay(double s, double t) {
        Vec3 rd = lensRadius * randomInUnitDisk(); // 随机生成一个圆盘内的点
        Vec3 offset = u * rd.x() + v * rd.y(); // 生成一个相机的偏移量

        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }
};

