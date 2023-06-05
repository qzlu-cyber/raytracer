#pragma once

#include "Ray.h"
#include "Hittable.h"

// ���������
class Material
{
public:
    // ɢ�亯��, attenuation: �������ɢ��, �������߻�䰵����, scattered: ɢ���Ĺ���
    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

// Lambertian ���������
class Lambertian : public Material
{
public:
    Vec3 albedo; // ������

public:
    Lambertian(const Vec3& a) : albedo(a) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 scatterDirection = rec.normal + randomUnitVector(); // ɢ���Ĺ��߷���
        scattered = Ray(rec.p, scatterDirection); // ɢ���Ĺ���
        attenuation = albedo; // ɢ���Ĺ���ǿ��

        return true;
    }

};

// Metal ��������
class Metal : public Material
{
public:
    Vec3 albedo; // ������
    double fuzz; // ģ���̶�

public:
    Metal(const Vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unitVector(inRay.direction()), rec.normal); // �����Ĺ��߷���
        scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere()); // �����Ĺ���
        attenuation = albedo; // �����Ĺ���ǿ��

        return (dot(scattered.direction(), rec.normal) > 0); // ��������Ĺ����뷨�����ļнǴ��� 90 ��, �򲻻ᷢ������
    }
};

class Dielectric : public Material
{
public:
    double ir; // ������

public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& inRay, const hitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        attenuation = Vec3(1.0, 1.0, 1.0); // ͸���Ĺ���ǿ��
        double etaiOverEtat = (rec.frontFace) ? (1.0 / ir) : (ir);; // ������ʵ��������������ʵ������ʵı�ֵ

        if (rec.frontFace) {
            etaiOverEtat = 1.0 / ir; // ����Ǵ����������, ��������ʵ�������Ϊ 1.0
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