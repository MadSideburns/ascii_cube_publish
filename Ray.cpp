#include "raytracing.h"

float ambient_light = .1;

Ray::Ray() { origin = {0., 0., 0.}; direction = ez; }

Ray::Ray(const Ray& other) 
{
    origin = other.origin;
    direction = other.direction;
}

Ray::Ray(const Vector3D& r0, const Vector3D& dir)
{
    origin = r0;
    direction = dir;
    direction.normalize();
}

Ray& Ray::operator=(const Ray& rhs)
{
    if (this == &rhs)
        return *this;

    origin = rhs.origin;
    direction = rhs.direction;

    return *this;
}

bool Ray::intersects(const Face& face) const
{
    /* parametrizzo la traiettoria del raggio come x = ray_origin + t * ray_direction 
    l'equazione del piano della faccia è (x - face_origin).dot(normal) = 0 */

    //primo check: se la normale è nella stessa direzione del raggio significa che vedo il retro della faccia
    if (direction.dot(face.normal) >= 0.)
        return false;

    //trovo il parametro t che risolve l'intersezione tra raggio e piano
    float t = (face.pos - origin).dot(face.normal) / (direction.dot(face.normal));
    
    //se t < 0 significa che la faccia è nel passato del raggio quindi no
    if (t <= 0.) return false;

    //alla fine controllo che l'intersezione non esca dai limiti della faccia   
    Vector3D intersection = origin + t * direction;

    for (auto axis : face.axes)
        if (fabs((intersection - face.pos).dot(axis)) > axis.norm2())
            return false;

    //se si superano tutti i check ritorna true
    return true;
}

//ritorna l'indice della faccia che interseca, o -1 se non interseca
int Ray::get_intersection(const Cube& cube) const
{
    //guardo se interseca una faccia del cubo e se la trovo ritorno il suo ptr
    for (int i = 0; i < cube.faces.size(); ++i)
        if (intersects(cube.faces[i]))
            return i;
    //se alla fine non interseca nulla ritorno nullptr
    return -1;
}

float Ray::get_color(const Face& face, const Vector3D& sun_dir, const Sky& sky) const
{
    float direct_coeff = 1.5;
    float specular_coeff = .6;
    Vector3D specular_direction = direction - face.normal * 2 * direction.dot(face.normal);
    
    float direct = fmax(0., - face.normal.dot(sun_dir)) * direct_coeff;
    float diffuse = sky.avg_color;
    float specular = sky.get_color(specular_direction) * specular_coeff;

    return direct + diffuse + specular;
}

Vector3D Ray::get_origin() const { return origin; }

Vector3D Ray::get_direction() const { return direction; }

void Ray::set_origin(const Vector3D& r0) { origin = r0; }

void Ray::set_direction(const Vector3D& dir) { direction = dir; }