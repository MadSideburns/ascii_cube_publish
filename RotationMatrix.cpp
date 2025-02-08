#include "raytracing.h"

//kronecher delta
int kd(int i, int j) { return i == j ? 1 : 0; }

//default constructor - return identity
RotationMatrix::RotationMatrix()
{
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            R[i][j] = kd(i, j);
}

//axis - angle constructor
RotationMatrix::RotationMatrix(const Vector3D& axis, float angle)
{
    //get normalized axis
    Vector3D u(axis);
    u.normalize();
    //get sine and cosine
    float c = cos(angle), s = sin(angle);

    //[u]_cross matrix (seen on wikipedia) ([u]_cross)_i = u.cross(e[i])
    float u_cross[dim][dim];
    for (int i = 0; i < dim; ++i)
    {
        Vector3D u_i = e[i].cross(u);
        for (int j = 0; j < dim; ++j)
        {
            u_cross[i][j] = u_i[j];
        }
    }
    //real initialization: R = c Id + s [u]_cross + (1 - c) u.tensor*(u)
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
        {
            R[i][j] = (c * kd(i, j)) + s * u_cross[i][j] + (1 - c) * u[i]*u[j];
        }
    
}

//copy constructor
RotationMatrix::RotationMatrix(const RotationMatrix& rhs)
{
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            R[i][j] = rhs[i][j];
}

//assignment
RotationMatrix& RotationMatrix::operator=(const RotationMatrix& rhs)
{
    if (this == &rhs)
        return *this;

    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            R[i][j] = rhs[i][j];

    return *this;
}

//composition
RotationMatrix RotationMatrix::operator*(const RotationMatrix& other) const
{
    RotationMatrix result;
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
        {
            result.R[i][j] = 0;
            for (int k = 0; k < dim; ++k)
                result.R[i][j] += R[i][k] * other[k][j];
        }
    return result;
}

//composition - assign
RotationMatrix& RotationMatrix::operator*=(const RotationMatrix& other)
{
    *this = (*this) * other;
    return *this;
}

//action on vector
Vector3D RotationMatrix::operator*(const Vector3D& vec) const
{
    Vector3D result;
    for (int i = 0; i < dim; ++i)
    {
        result[i] = 0.;
        for (int j = 0; j < dim; ++j)
            result[i] += R[i][j] * vec[j];
    }
    
    return result;
}

const float* RotationMatrix::operator[](int index) const
{
    if (index < 0 || index > 2)
        throw std::out_of_range("Index out of bounds. Only valid indices: 0, 1, 2.");
    return R[index];
}

void RotationMatrix::print() const
{
    for (int i = 0; i < dim; ++i)
        printf("[%10.2e%10.2e%10.2e]\n", R[i][0], R[i][1], R[i][2]);
}