#pragma once

#include "Ray.h"

class Material;

/*
    ���߻��������潻�����ɾ���Ĳ����������ġ�hitRecord ������Ͼ���Ϊ�˰�һ��Ҫ���Ĳ������������һ��
    ����������һ������(����һ������), hitRecord �еĲ���ָ��ᱻ����Ĳ���ָ������ֵ, ������Ĳ���ָ��
    ���� main() �����й���ʱ����ġ��� color() ������ȡ�� hitRecord ʱ, �������ҵ�������ʵ�ָ��, 
    Ȼ���ɲ��ʵĺ��������������Ƿ���ɢ��, ��ôɢ�䡣
*/
struct hitRecord {
    Vec3 p; // ����
    Vec3 normal; // ���㴦�ķ�����
    double t; // ���㴦�� t ֵ
    bool frontFace; // �Ƿ��Ǵ�����(��������)������Ĺ���
    std::shared_ptr<Material> matPtr; // ���㴦�Ĳ���

    // ���÷������ĳ���
    inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal; // ����Ǵ����������, ����������; ����, ����������
    }
};

class Hittable {
public:
    // �жϹ����Ƿ��������н���
    // t_min: ���ߵ����, t_max: ���ߵ��յ�
    virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const = 0;
};

