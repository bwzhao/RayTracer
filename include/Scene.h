#pragma once

#include "geometry/ObjectList.h"
#include "Camera.h"
#include "Vec3.h"
#include <fstream>
#include "integrator/Integrator.h"

class PathTracingIntegrator;
class BDPTIntegrator;
class Vertex;

class Scene {
private:
    Color background_;

    shared_ptr<ObjectList> world_ptr_;
    shared_ptr<ObjectList> focus_lights_ptr_;
    std::shared_ptr<Light> light_ptr_;
public:
    Scene();
    void set_world(ObjectList &world) {world_ptr_ = std::make_shared<ObjectList>(world);}
    void set_focus_lights(shared_ptr<ObjectList> lights_ptr) { focus_lights_ptr_ = std::move(lights_ptr);}
    void set_light(shared_ptr<Light> light_ptr) {light_ptr_ = std::move(light_ptr);}
    shared_ptr<Light> get_light_ptr() const{return light_ptr_;}

    friend PathTracingIntegrator;
    friend BDPTIntegrator;
    friend Vertex;
};


