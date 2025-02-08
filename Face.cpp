#include "raytracing.h"

int sign(float x)
{
    if (x > 0.) return 1;
    else if (x < 0.) return -1;
    else return 0;
}

//default constructor: faccia con assi su x, y nell'origine
Face::Face()
{   
    pos = {0., 0., 0.};
    axes = {ex, ey};
    normal = ez;
}

Face::Face(const Face& other)
{
    pos = other.pos;
    axes = other.axes;
    normal = other.normal;
}

Face::Face(const Vector3D& origin, const Vector3D& a1, const Vector3D& a2)
{
    pos = origin;
    axes[0] = a1;
    //al secondo asse tolgo la componente lungo il primo just to be safe
    axes[1] = a2 - (a1.dot(a2) / a2.norm2()) * a1;
    normal = (a1.cross(a2)).normalize();
}

Face& Face::operator=(const Face& rhs)
{
    if (this == &rhs)
        return *this;

    pos = rhs.pos;
    axes = rhs.axes;
    normal = rhs.normal;

    return *this;
}

Face& Face::translate(const Vector3D& offset)
{
    pos += offset;
    return *this;
}

Face& Face::rotate(const RotationMatrix& rot)
{
    axes[0] = rot * axes[0];
    axes[1] = rot * axes[1];
    normal = rot * normal;

    return *this;
}

Face& Face::scale(float sa, float sb)
{
    axes[0] *= sa;
    axes[1] *= sb;
    normal *= sign(sa * sb);

    return *this;
}

Vector3D Face::get_position() const { return pos; }

std::array<Vector3D, 2> Face::get_axes() const { return axes; }

Vector3D Face::get_axis(int index) const { return axes[index]; }

Vector3D Face::get_normal() const { return normal; }

void Face::print() const
{
    std::cout << "Face object @" << this << std::endl;
    std::cout << "Origin:" << std::endl;
    pos.print();
    std::cout << "Axes:" << std::endl;
    for (auto ax : axes) ax.print();
    std::cout << "Normal:" << std::endl;
    normal.print();
}