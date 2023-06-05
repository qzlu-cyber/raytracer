#pragma once

#include "Ray.h"
#include "Hittable.h"

// 抽象材质类
class Material
{
public:
    // 散射函数, attenuation: 如果发生散射, 决定光线会变暗多少, scattered: 散射后的光线
    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

// Lambertian 漫反射材质
class Lambertian : public Material
{
public:
    Vec3 albedo; // 反射率

public:
    Lambertian(const Vec3& a) : albedo(a) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 scatterDirection = rec.normal + randomUnitVector(); // 散射后的光线方向
        scattered = Ray(rec.p, scatterDirection); // 散射后的光线
        attenuation = albedo; // 散射后的光线强度

        return true;
    }

};

// Metal 金属材质
class Metal : public Material
{
public:
    Vec3 albedo; // 反射率
    double fuzz; // 模糊程度

public:
    Metal(const Vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unitVector(inRay.direction()), rec.normal); // 反射后的光线方向
        scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere()); // 反射后的光线
        attenuation = albedo; // 反射后的光线强度

        return (dot(scattered.direction(), rec.normal) > 0); // 如果反射后的光线与法向量的夹角大于 90 度, 则不会发生反射
    }
};

class Dielectric : public Material
{
public:
    double ir; // 折射率

public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        attenuation = Vec3(1.0, 1.0, 1.0); // 透射后的光线强度
        double etaiOverEtat = (rec.frontFace) ? (1.0 / ir) : (ir);; // 入射介质的折射率与出射介质的折射率的比值

        if (rec.frontFace) {
            etaiOverEtat = 1.0 / ir; // 如果是从正面打到物体, 则入射介质的折射率为 1.0
        }
        else {
            etaiOverEtat = ir;
        }

        Vec3 unitDirection = unitVector(inRay.direction());

        double cos_theta = ffmin(dot(-unitDirection, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etaiOverEtat * sin_theta > 1.0) {
            Vec3 reflected = reflect(unitDirection, rec.normal);
            scattered = Ray(rec.p, reflected);
            return true;
        }

        double reflectProb = schlick(cos_theta, etaiOverEtat);
        if (randomDouble() < reflectProb)
        {
            Vec3 reflected = reflect(unitDirection, rec.normal);
            scattered = Ray(rec.p, reflected);
            return true;
        }

        Vec3 refracted = refract(unitDirection, rec.normal, etaiOverEtat);
        scattered = Ray(rec.p, refracted);

        return true;
    }
};