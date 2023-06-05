#include <iostream>
#include <fstream>

#include "util.h"
#include "Ray.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

Vec3 rayColor(const Ray& r, const HittableList& world, int depth) {
    hitRecord rec;

    if (depth < 1) return Vec3(0, 0, 0); // �ݹ���ȴﵽ����, ���ٵݹ�

    if (world.hit(r, 0.001, Infinity, rec)) {
        Ray scattered; // ɢ�����
        Vec3 attenuation; // ˥������

        if (rec.matPtr->scatter(r, rec, attenuation, scattered)) { // ��ɢ�����
            return attenuation * rayColor(scattered, world, depth - 1); // �ݹ����
        }

        return Vec3(0, 0, 0); // û��ɢ�����, �����߱�����
    }

    Vec3 unitDirection = unitVector(r.direction()); // ��λ��, �Ա�֤ y ��ȡֵ��Χ (-1.0 < y < 1.0)
    auto t = 0.5 * (unitDirection.y() + 1.0); // �� y �ķ�Χ�� -1.0 �� y �� 1.0 ӳ�䵽 0 �� y �� 1.0������ t = 1.0 ʱ������ɫ, �� t = 0.0 ʱ���ǰ�ɫ
    
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0); // ���� y �����Բ�ֵ
}

HittableList randomScene() {
    HittableList world;

    world.add(std::make_shared<Sphere>(
        Vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            Vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Vec3::random() * Vec3::random();
                    world.add(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Vec3::random(.5, 1);
                    auto fuzz = randomDouble(0, .5);
                    world.add(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else {
                    // glass
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world.add(
        std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));

    world.add(
        std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0, std::make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main() {
    const int imageWidth = 600;
    const int imageHeight = 300;
    const int samplesPerPixel = 100; // ÿ�����صĲ�������
    const int maxDepth = 50; // �ݹ���ȣ������ߵ������
    double scale = 1.0 / samplesPerPixel;

    const auto aspectRatio = double(imageWidth) / imageHeight; // ��߱�

    std::ofstream outputFile;

    Vec3 lookfrom(13, 2, 3);
    Vec3 lookat(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    Camera camera(20, aspectRatio, lookfrom, lookat, vUp, aperture, distToFocus); // ���

    HittableList world = randomScene();
    //world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Vec3(0.7, 0.3, 0.3))));
    //world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0))));
    //world.add(std::make_shared<Sphere>(Vec3(1, 0, -1), 0.5, std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.3)));
    ////world.add(std::make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 0.5)));
    //world.add(std::make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(1.5)));
    //world.add(std::make_shared<Sphere>(Vec3(-1, 0, -1), -0.45, std::make_shared<Dielectric>(1.5)));

    outputFile.open("./image.ppm");

    outputFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            Vec3 color(0, 0, 0); // ÿ��ѭ��������
            for (int s = 0; s < samplesPerPixel; s++) {
                auto u = double(i + randomDouble()) / imageWidth;
                auto v = double(j + randomDouble()) / imageHeight;
                
                Ray r = camera.getRay(u, v);

                color += rayColor(r, world, maxDepth);
            }
            outputFile << static_cast<int>(256 * clamp(sqrt(scale * color.e[0]), 0.0, 0.999)) << ' '
                       << static_cast<int>(256 * clamp(sqrt(scale * color.e[1]), 0.0, 0.999)) << ' '
                       << static_cast<int>(256 * clamp(sqrt(scale * color.e[2]), 0.0, 0.999)) << '\n';
        }
    }
    std::cerr << "\nDone.\n";

    outputFile.close();
}