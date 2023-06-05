#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const {
    Vec3 oc = r.origin() - center; // ������㵽Բ�ĵ�����
    auto a = dot(r.direction(), r.direction()); // ���ߵķ��������ĳ��ȵ�ƽ��
    auto b = 2 * dot(oc, r.direction()); // ���ߵķ��������͹�����㵽Բ�ĵ������ĵ��
    auto c = dot(oc, oc) - radius * radius; // ������㵽Բ�ĵ������ĳ��ȵ�ƽ����ȥԲ�İ뾶��ƽ��
    auto discriminant = b * b - 4 * a * c; // ���� �� �жϹ��ߺ�Բ�Ƿ��н���

    if (discriminant > 0) {
        auto temp = (-b - sqrt(discriminant)) / (2 * a); // �н���ʱ, ȡ��С���Ǹ�

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = unitVector(rec.p - center);

            Vec3 outwardNormal = (rec.p - center) / radius; // ���㽻�㴦�ķ�����
            rec.setFaceNormal(r, outwardNormal); // ���÷������ĳ���

            rec.matPtr = matPtr; // ���㴦�Ĳ���

            return true;
        }

        temp = (-b + sqrt(discriminant)) / (2 * a); // �н���ʱ, ȡ�ϴ���Ǹ�

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = unitVector(rec.p - center);

            Vec3 outwardNormal = (rec.p - center) / radius; // ���㽻�㴦�ķ�����
            rec.setFaceNormal(r, outwardNormal); // ���÷������ĳ���

            rec.matPtr = matPtr; // ���㴦�Ĳ���

            return true;
        }
    }

    return false;
}