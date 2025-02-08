#include "raytracing.h"
#include "ascii_ramp.h"

int ascii_range(float value)
{
    return (int) ((fmin(fmax(value, 0.), VALUE_CLAMP) / VALUE_CLAMP) * (nchars - 1));
}

Camera::Camera()
{
   resolution = {100, 60};
   origin = {0., 0., 0.};
   sensor_center = ex/10.;
   axes = {ey/10., ez/10. * .6};
   generate_screenspace_basis();
}

Camera::Camera(const Camera& other)
{
    resolution = other.resolution;
    origin = other.origin;
    sensor_center = other.sensor_center;
    axes = other.axes;
    generate_screenspace_basis();
}

Camera::Camera(int resx, int resy, const Vector3D& position, const Vector3D& direction, float fov, float aspect)
{
    resolution = {resx, resy};
    origin = position;
    sensor_center = direction;

    /* di default la direzione è nota, l'asse maggiore è sul piano polare
    e quello minore è ortogonale a entrambi. Casi speciali se dir // ez */
    
    Vector3D a1, a2;

    //caso limite in cui guardo lungo ez
    if (direction.X == 0. && direction.Y == 0.)
    {
        a1 = ey * direction.norm() * tan(fov * PI / 360.);
        
    }
    //tutti gli altri casi
    else
    {
        a1 = (ez.cross(direction)).normalize() * tan(fov * PI / 360.) * direction.norm();
    }
    
    a2 = (direction.cross(a1)).normalize() * a1.norm() / aspect;

    axes = {a1, a2};
    generate_screenspace_basis();
}

void Camera::generate_screenspace_basis()
{
    Vector3D u, v;
    u = -2 * axes[0] / resolution[0];
    v = -2 * axes[1] / resolution[1];
    screenspace_basis = {u, v};
}

Camera& Camera::operator=(const Camera& rhs)
{
    if (this == &rhs) 
        return *this;

    resolution = rhs.resolution;
    origin = rhs.origin;
    sensor_center = rhs.sensor_center;
    axes = rhs.axes;
    screenspace_basis = rhs.screenspace_basis;

    return *this;
}

Camera& Camera::translate(const Vector3D& offset)
{
    origin += offset;

    return *this;
}

Camera& Camera::rotate(const RotationMatrix& rot)
{
    sensor_center = rot * sensor_center;
    axes[0] = rot * axes[0];
    axes[1] = rot * axes[1];
    screenspace_basis[0] = rot * screenspace_basis[0];
    screenspace_basis[1] = rot * screenspace_basis[1];

    return *this;
}

int Camera::get_xres() const { return resolution[0]; }

int Camera::get_yres() const { return resolution[1]; }

Vector3D Camera::get_origin() const { return origin; }

Vector3D Camera::get_sensor_offset() const { return sensor_center; }

std::array<Vector3D, 2> Camera::get_axes() const { return axes; }

std::array<Vector3D, 2> Camera::get_screenspace_basis() const { return screenspace_basis; }

Ray Camera::get_pixel_ray(int x, int y) const
{
    Vector3D screen_corner = origin + sensor_center + axes[0] + axes[1];
    Vector3D screen_space_position = x * screenspace_basis[0] + y * screenspace_basis[1];
    Vector3D pixel_position = screen_corner + screen_space_position;

    return Ray(pixel_position, pixel_position - origin);
}

std::ostringstream Camera::snapshot(const Cube& cube, const Vector3D& sun_direction, const Sky& sky) const
{
    Ray ray;
    Face intersected_face;
    int intersection_index;
    float light_value;
    char ascii_color;

    std::ostringstream buffer;

    for (int y = 0; y < resolution[1]; ++y)
    {
        for (int x = 0; x < resolution[0]; ++x)
        {
            ray = get_pixel_ray(x, y);
            intersection_index = ray.get_intersection(cube);
            if (intersection_index == -1)
            {
                light_value = sky.get_color(ray.get_direction());
            }
            else
            {
                intersected_face = cube.get_faces()[intersection_index];
                light_value = ray.get_color(intersected_face, sun_direction, sky);
            }
            ascii_color = ramp[nchars - ascii_range(light_value) - 1];
            buffer << ascii_color;
            //printf("%2d", ascii_range(light_value));
        }
        buffer << '\n';
    }
    return buffer;
}