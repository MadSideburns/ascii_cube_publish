#include "raytracing.h"

//funzione che genera le facce del cubo a partire da posizione e assi
void Cube::build_faces()
{
    //loop sulle facce da fare
    for (int i = 0; i < 6; ++i)
    {
        int s = i < 3 ? 1 : -1;
        Vector3D r0 = pos + s * axes[i % 3];
        Vector3D a = axes[(i+1) % 3], b = s * axes[(i+2) % 3];
        faces[i] = Face(r0, a, b);
    } 
}

//default cube
Cube::Cube()
{
    pos = {0., 0., 0.};
    axes = {ex, ey, ez};
    build_faces();
}

Cube::Cube(const Cube& other)
{
    pos = other.pos;
    axes = other.axes;
    faces = other.faces;
}

Cube::Cube(const Vector3D& position, const Vector3D& a1, const Vector3D& a2, const Vector3D& a3)
{
    pos = position;
    //ortogonalizzo tutti gli assi to be safe (devo ricordare di usare la versione già ortogonale)
    axes[0] = a1;
    axes[1] = a2 - (a2.dot(a1) / a1.norm2()) * a1;
    axes[2] = a3 - (a3.dot(a1) / a1.norm2()) * a1 - (a3.dot(axes[1]) / axes[1].norm2()) * axes[1];

    build_faces();
}

Cube& Cube::operator=(const Cube& rhs)
{
    if (this == &rhs)
        return *this;

    pos = rhs.pos;
    axes = rhs.axes;
    faces = rhs.faces;

    return *this;
}

Cube& Cube::translate(const Vector3D& offset)
{
    pos += offset;
    build_faces();

    return *this;
}

Cube& Cube::rotate(const RotationMatrix& rot)
{
    for (int i = 0; i < 3; ++i)
        axes[i] = rot * axes[i];
    build_faces();

    return *this;
}

Vector3D Cube::get_pos() const { return pos; }

std::array<Vector3D, 3> Cube::get_axes() const { return axes; }

std::array<Face, 6> Cube::get_faces() const { return faces; }