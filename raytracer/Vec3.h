#pragma once
#include <iostream>
#include <cmath>

#include "util.h"

class Vec3
{
public:
    double e[3];
public:
    Vec3()
        : e {0, 0, 0} {}
    Vec3(double x, double y, double z) 
        : e {x, y, z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    Vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    Vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    // 计算 Vec3 的模长
    double length() const {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // 用于漫反射材质的物体，在物体内随机取一点作为漫反射的方向
    inline static Vec3 random() {
        return Vec3(randomDouble(), randomDouble(), randomDouble());
    }

    inline static Vec3 random(double min, double max) {
        return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 单位化
inline Vec3 unitVector(Vec3 v) {
    return v / v.length();
}

// 从单位球体内取的, 其长度是随机的
inline Vec3 randomInUnitSphere() {
    while (true) {
        auto p = Vec3::random(-1, 1); // 随机取一个点
        if (p.lengthSquared() >= 1) continue; // 如果点在球外，重新取点

        return p;
    }
}

// 使用 lambertian 散射随机取一个单位向量。从单位球面上取的, 长度固定为单位向量长度
inline Vec3 randomUnitVector() {
    auto a = randomDouble(0, 2 * PI);
    auto z = randomDouble(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

// 使用 cosine 散射随机取一个单位向量。直接从入射点开始选取一个随机的方向, 然后再判断是否在法向量所在的那个半球
inline Vec3 randomInHemisphere(const Vec3& normal) {
    Vec3 inUnitSphere = randomInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0) // 在半球内
        return inUnitSphere;
    else
        return -inUnitSphere;
}

inline Vec3 randomInUnitDisk() {
    while (true) {
        auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (p.lengthSquared() >= 1) continue;

        return p;
    }
}

// 反射
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// 折射 etaiOverEtat: 入射介质的折射率 / 出射介质的折射率
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etaiOverEtat) {
    auto cosTheta = dot(-uv, n); // cosθ
    Vec3 rOutParallel = etaiOverEtat * (uv + cosTheta * n); // 平行光线的折射
    Vec3 rOutPerp = -sqrt(1.0 - rOutParallel.lengthSquared()) * n; // 垂直光线的折射

    return rOutParallel + rOutPerp; // 平行光线的折射 + 垂直光线的折射 = 折射光线
}