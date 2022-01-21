#pragma once

#include "geometry/Object.h"
#include "Pdf.h"
#include "Material.h"

class Light{
public:
    Light(shared_ptr<Object> p, Color color)
    : object_ptr_(p){
        p->mat_ptr_ = std::make_shared<DiffuseLight>(color);
    }

    virtual Color sample_ray(Ray &random_ray, double &pdf_pos, double &pdf_dir, Vec3 &normal) = 0;

public:
    shared_ptr<Object> object_ptr_;
    shared_ptr<CosinePdf> pdf_ptr_;
};

class AreaLight: public Light{
public:
    AreaLight(shared_ptr<Object> p, Color color): Light(p, color){
        pdf_ptr_ = std::make_shared<CosinePdf>(Vec3(1., 0., 0.));
    }

    virtual Color sample_ray(Ray &random_ray, double &pdf_pos, double &pdf_dir, Vec3 &normal);
};


