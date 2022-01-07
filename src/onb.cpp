#include "onb.h"

void onb::build_from_w(const vec3& n) {
    axis_[2] = unit_vector(n);
    vec3 a = (fabs(w().x()) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
    axis_[1] = unit_vector(cross(w(), a));
    axis_[0] = cross(w(), v());
}
