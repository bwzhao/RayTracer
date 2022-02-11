#include "Scene.h"

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "Camera.h"
#include "integrator/PathTracingIntegrator.h"


Scene::Scene():
background_(0, 0, 0)
{
}

