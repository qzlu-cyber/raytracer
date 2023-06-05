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

    // ���� Vec3 ��ģ��
    double length() const {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // ������������ʵ����壬�����������ȡһ����Ϊ������ķ���
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

// ��λ��
inline Vec3 unitVector(Vec3 v) {
    return v / v.length();
}

// �ӵ�λ������ȡ��, �䳤���������
inline Vec3 randomInUnitSphere() {
    while (true) {
        auto p = Vec3::random(-1, 1); // ���ȡһ����
        if (p.lengthSquared() >= 1) continue; // ����������⣬����ȡ��

        return p;
    }
}

// ʹ�� lambertian ɢ�����ȡһ����λ�������ӵ�λ������ȡ��, ���ȹ̶�Ϊ��λ��������
inline Vec3 randomUnitVector() {
    auto a = randomDouble(0, 2 * PI);
    auto z = randomDouble(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

// ʹ�� cosine ɢ�����ȡһ����λ������ֱ�Ӵ�����㿪ʼѡȡһ������ķ���, Ȼ�����ж��Ƿ��ڷ��������ڵ��Ǹ�����
inline Vec3 randomInHemisphere(const Vec3& normal) {
    Vec3 inUnitSphere = randomInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0) // �ڰ�����
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

// ����
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// ���� etaiOverEtat: ������ʵ������� / ������ʵ�������
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etaiOverEtat) {
    auto cosTheta = dot(-uv, n); // cos��
    Vec3 rOutParallel = etaiOverEtat * (uv + cosTheta * n); // ƽ�й��ߵ�����
    Vec3 rOutPerp = -sqrt(1.0 - rOutParallel.lengthSquared()) * n; // ��ֱ���ߵ�����

    return rOutParallel + rOutPerp; // ƽ�й��ߵ����� + ��ֱ���ߵ����� = �������
}