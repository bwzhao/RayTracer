#pragma once

#include "geometry/Object.h"
#include "Pdf.h"

class Light{
public:
    Light(shared_ptr<Object> p)
    : ptr_(p){}

    void sample_ray(Ray & random_ray, double & pdf_pos, double & pdf_dir);

public:
    shared_ptr<Object> ptr_;
};


