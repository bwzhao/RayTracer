#pragma once

#include "geometry/Object.h"
#include "Pdf.h"

class Light{
public:
    Light(shared_ptr<Object> p)
    : object_ptr_(p){}

    virtual Color sample_ray(Ray & random_ray, double & pdf_pos, double & pdf_dir) = 0;

public:
    shared_ptr<Object> object_ptr_;
    shared_ptr<CosinePdf> pdf_ptr;
};

class AreaLight: public Light{
public:
    virtual Color sample_ray(Ray & random_ray, double & pdf_pos, double & pdf_dir);
public:
    shared_ptr<CosinePdf> pdf_ptr;
};


