#include "raytracing.h"

//default constructor - returns {0, 0, 0}
Vector3D::Vector3D() { X = Y = Z = 0.; }

//initializer constructor with initializer list capability
Vector3D::Vector3D(float x, float y, float z) : X(x), Y(y), Z(z) { }

//copy constructor
Vector3D::Vector3D(const Vector3D& rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}

//assignment constructor
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    //protection against self-assignment
    if (this == &rhs)
        return *this;

    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    
    return *this;
}

//norm squared
float Vector3D::norm2() const
{
    return X*X + Y*Y + Z*Z;
}

//norm
float Vector3D::norm() const
{
    return sqrt(norm2());
}

//normalize
Vector3D& Vector3D::normalize()
{
    float n = norm();
    X /= n;
    Y /= n;
    Z /= n;

    return *this;
}

//dot product
float Vector3D::dot(const Vector3D& other) const
{
    return X*other.X + Y*other.Y + Z*other.Z;
}

//cross product
Vector3D Vector3D::cross(const Vector3D& other) const
{
    float x = Y*other.Z - Z*other.Y;
    float y = Z*other.X - X*other.Z;
    float z = X*other.Y - Y*other.X;
    return Vector3D(x, y, z);
}

//unary +
Vector3D& Vector3D::operator+()
{
    return *this;
}

//sum between vectors
Vector3D Vector3D::operator+(const Vector3D& other) const
{
    float x = X + other.X;
    float y = Y + other.Y;
    float z = Z + other.Z;
    return Vector3D(x, y, z);
}

//sum - assign
Vector3D& Vector3D::operator+=(const Vector3D& other)
{
    X += other.X;
    Y += other.Y;
    Z += other.Z;

    return *this;
}

//unary -
Vector3D Vector3D::operator-() const
{
    return Vector3D(-X, -Y, -Z);
}

//difference between vectors
Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return *this + (-other);
}

//diff - assign
Vector3D& Vector3D::operator-=(const Vector3D& other)
{
    *this += (-other);
    return *this;
}

//multiply with scalar
Vector3D Vector3D::operator*(float t) const
{
    return Vector3D(t*X, t*Y, t*Z);
}

//scalar mult - assign
Vector3D& Vector3D::operator*=(float t)
{
    X *= t;
    Y *= t;
    Z *= t;
    return *this;
}

//divide with scalar
Vector3D Vector3D::operator/(float t) const
{
    return Vector3D(X/t, Y/t, Z/t);
}

//scalar div - assign
Vector3D& Vector3D::operator/=(float t)
{
    X /= t;
    Y /= t;
    Z /= t;
    return *this;
}

Vector3D& Vector3D::rotate(const RotationMatrix& rot)
{
    *this = rot * (*this);

    return *this;
}

void Vector3D::print() const
{
    printf("{%10.2e%10.2e%10.2e}\n", X, Y, Z);
}

//coordinate access
float Vector3D::operator[](int coord) const
{
    if (coord < 0 || coord > 2)
        throw std::out_of_range("Index out of bounds. Only valid indices: 0, 1, 2.");
    
    return (coord == 0) ? X : (coord == 1) ? Y : Z;
}

//coordinate access - assign
float& Vector3D::operator[](int coord)
{
    if (coord < 0 || coord > 2)
        throw std::out_of_range("Index out of bounds. Only valid indices: 0, 1, 2.");
    
    return (coord == 0) ? X : (coord == 1) ? Y : Z;
}

Vector3D operator*(float t, const Vector3D& vec)
{
    return vec * t;
}