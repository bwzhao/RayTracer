#include "Light.h"

void Light::sample_ray(Ray &random_ray, double &pdf_pos, double &pdf_dir) {
    Point3 ran_pos;
    Vec3 ran_normal, ran_dir;
    ptr_->random_pos(ran_pos, ran_normal, pdf_pos);

    auto pdf = CosinePdf(ran_normal);
    ran_dir = pdf.get_random_dir();
    pdf_dir = pdf.get_pdf_dir(ran_dir);

    random_ray = Ray(ran_pos, ran_dir, 0.);
}