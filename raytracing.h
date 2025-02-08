#ifndef RAYTRACING
#define RAYTRACING

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <array>
#include <sstream>

#define VALUE_CLAMP 3.0
#define PI 3.14159

int sign(float x);
//kronecher delta
int kd(int i, int j);

class RotationMatrix;

class Vector3D
{
public:
//member variables
    float X, Y, Z;
//member methods
    Vector3D();                                     //default constructon
    Vector3D(float x, float y, float z);            //initializer constructor
    Vector3D(const Vector3D&);                      //copy constructor
    Vector3D& operator=(const Vector3D&);           //assignment operator
    //vector properties and manipulation
    float norm2() const;
    float norm() const;
    Vector3D& normalize();

    //vector operations
    float dot(const Vector3D&) const;
    Vector3D cross(const Vector3D&) const;

    //arithmetic overloading
    Vector3D& operator+();                          //unary +
    Vector3D operator+(const Vector3D&) const;      //binary +
    Vector3D& operator+=(const Vector3D&);          //
    Vector3D operator-() const;                     //unary -
    Vector3D operator-(const Vector3D&) const;      //binary -
    Vector3D& operator-=(const Vector3D&);          //
    Vector3D operator*(float) const;                //scalar mult *
    Vector3D& operator*=(float);                    //
    Vector3D operator/(float) const;                //scalar div /
    Vector3D& operator/=(float);                    //

    //rotation in space
    Vector3D& rotate(const RotationMatrix&);

    //utility
    void print() const;

    //subscript operator
    float operator[](int) const;                    //member access
    float& operator[](int);                         //member assignment
};

Vector3D operator*(float, const Vector3D&);         //scalar * vector mult

//vector basis constants
const Vector3D ex = {1, 0, 0}, ey = {0, 1, 0}, ez = {0, 0, 1};
const Vector3D e[] = {ex, ey, ez};

class RotationMatrix
{
//private:
public:
    const int dim = 3;
    float R[3][3];
public:
    RotationMatrix();                                   //default constructor
    RotationMatrix(const Vector3D&, float);             //axis - angle constructor      
    RotationMatrix(const RotationMatrix&);              //copy constructor
    RotationMatrix& operator=(const RotationMatrix&);   //assignment

    //mathematical operations (just rotation group)
    RotationMatrix operator*(const RotationMatrix&) const;  //rotation composition
    RotationMatrix& operator*=(const RotationMatrix&);      //composition - assign
    Vector3D operator*(const Vector3D&) const;              //action on vector

    //subscript operator (only access)
    const float* operator[](int) const;

    void print() const;
};

class Face
{
private:
    Vector3D pos;
    std::array<Vector3D, 2> axes;
    Vector3D normal;
    friend class Ray;

public:
    Face();                                                         //default constructor
    Face(const Face&);                                              //copy constructor
    Face(const Vector3D&, const Vector3D&, const Vector3D&);        //position and axes
    Face& operator=(const Face&);

    Face& translate(const Vector3D&);
    Face& rotate(const RotationMatrix&);
    Face& scale(float, float);

    Vector3D get_position() const;
    std::array<Vector3D, 2> get_axes() const;
    Vector3D get_axis(int) const;
    Vector3D get_normal() const;

    void print() const;
};

class Cube
{
private:
    friend class Ray;
    Vector3D pos;
    std::array<Vector3D, 3> axes;
    std::array<Face, 6> faces;
    
    //build faces from axes
    void build_faces();

public:
    Cube();
    Cube(const Cube&);
    Cube(const Vector3D&, const Vector3D&, const Vector3D&, const Vector3D&);
    Cube& operator=(const Cube&);

    Cube& translate(const Vector3D&);
    Cube& rotate(const RotationMatrix&);

    Vector3D get_pos() const;
    std::array<Vector3D, 3> get_axes() const;
    std::array<Face, 6> get_faces() const;
};

class Sky
{
public:
    float avg_color = .2;
    float get_color(const Vector3D&) const;
};

class Ray
{
private:
    Vector3D origin;
    Vector3D direction;
public:
    Ray();
    Ray(const Ray&);
    Ray(const Vector3D&, const Vector3D&);
    Ray& operator=(const Ray&);

    bool intersects(const Face&) const;
    int get_intersection(const Cube&) const;
    float get_color(const Face&, const Vector3D&, const Sky&) const;

    Vector3D get_origin() const;
    Vector3D get_direction() const;
    void set_origin(const Vector3D&);
    void set_direction(const Vector3D&);
};

class Camera
{
private:
    std::array<int, 2> resolution;
    Vector3D origin;
    Vector3D sensor_center;
    std::array<Vector3D, 2> axes;
    std::array<Vector3D, 2> screenspace_basis;
    void generate_screenspace_basis();
public:
    Camera();
    Camera(const Camera&);
    Camera(int, int, const Vector3D&, const Vector3D&, float, float);
    Camera& operator=(const Camera&);

    Camera& translate(const Vector3D&);
    Camera& rotate(const RotationMatrix&);

    int get_xres() const;
    int get_yres() const;
    Vector3D get_origin() const;
    Vector3D get_sensor_offset() const;
    std::array<Vector3D, 2> get_axes() const;
    std::array<Vector3D, 2> get_screenspace_basis() const;

    Ray get_pixel_ray(int, int) const;
    std::ostringstream snapshot(const Cube&, const Vector3D&, const Sky&) const;
};

#endif