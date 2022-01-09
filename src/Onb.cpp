#include "Onb.h"

void Onb::build_from_w(const Vec3& n) {
    axis_[2] = unit_vector(n);
    Vec3 a = (fabs(w().x()) > 0.9) ? Vec3(0,1,0) : Vec3(1,0,0);
    axis_[1] = unit_vector(cross(w(), a));
    axis_[0] = cross(w(), v());
}
