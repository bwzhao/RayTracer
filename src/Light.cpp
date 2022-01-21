#include "Light.h"
#include "Material.h"

Color AreaLight::sample_ray(Ray &random_ray, double &pdf_pos, double &pdf_dir, Vec3 &normal) {
    Point3 ran_pos;
    Vec3 ran_normal, ran_dir;
    double u, v;
    object_ptr_->random_pos(ran_pos, ran_normal, pdf_pos, u, v);

    normal = ran_normal;
    pdf_ptr_->set_normal_dir(ran_normal);
    ran_dir = pdf_ptr_->get_random_dir();
    pdf_dir = pdf_ptr_->get_pdf(ran_dir);

    random_ray = Ray(ran_pos, ran_dir, 0.);

    return object_ptr_->mat_ptr_->get_Le(u, v, ran_pos);
}