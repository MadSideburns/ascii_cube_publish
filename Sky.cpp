#include "raytracing.h"

float Sky::get_color(const Vector3D& direction) const
{
    int ntheta = 50, nphi = 100;

    float r = Vector3D(direction.X, direction.Y, 0).norm();
    float theta = PI/2 - atan2(direction.Z, r);
    float phi = atan2(direction.Y, direction.X) + PI;
    
    int indtheta = floor(theta / PI * ntheta);
    int indphi = floor(phi / (2*PI) * nphi);

    float grid = ((indtheta + indphi) % 2) * .8; 
    float gradient = atan(50 * (-theta + PI/2)) * .3;

    return .5 + grid + gradient;
}