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
    Vector3D(const Vector3D& vec);                  //copy constructor
    Vector3D& operator=(const Vector3D& rhs);       //assignment operator
    //vector properties and manipulation
    float norm2() const;
    float norm() const;
    Vector3D& normalize();

    //vector operations
    float dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;

    //arithmetic overloading
    Vector3D& operator+();                          //unary +
    Vector3D operator+(const Vector3D& vec) const;  //binary +
    Vector3D& operator+=(const Vector3D& vec);      //
    Vector3D operator-() const;                     //unary -
    Vector3D operator-(const Vector3D& vec) const;  //binary -
    Vector3D& operator-=(const Vector3D& vec);      //
    Vector3D operator*(float num) const;            //scalar mult *
    Vector3D& operator*=(float num);                //
    Vector3D operator/(float num) const;            //scalar div /
    Vector3D& operator/=(float num);                //

    //rotation in space
    Vector3D& rotate(const RotationMatrix& rotation);

    //utility
    void print() const;

    //subscript operator
    float operator[](int index) const;                    //member access
    float& operator[](int index);                         //member assignment
};

Vector3D operator*(float num, const Vector3D& vec);       //scalar * vector mult

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
    RotationMatrix();                                       //default constructor
    RotationMatrix(const Vector3D& axis, float angle);      //axis - angle constructor      
    RotationMatrix(const RotationMatrix& rotation);         //copy constructor
    RotationMatrix& operator=(const RotationMatrix& rhs);   //assignment

    //mathematical operations (just rotation group)
    RotationMatrix operator*(const RotationMatrix& rotation) const;     //rotation composition
    RotationMatrix& operator*=(const RotationMatrix& rotation);         //composition - assign
    Vector3D operator*(const Vector3D& vec) const;                      //action on vector

    //subscript operator (only access)
    const float* operator[](int index) const;

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
    Face();                                                                         //default constructor
    Face(const Face& face);                                                         //copy constructor
    Face(const Vector3D& center, const Vector3D& axis1, const Vector3D& axis2);     //position and axes
    Face& operator=(const Face& rhs);

    Face& translate(const Vector3D& offset);
    Face& rotate(const RotationMatrix& rotation);
    Face& scale(float scalex, float scaley);

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
    Cube(const Cube& cube);
    Cube(const Vector3D& center, const Vector3D& axis1, const Vector3D& axis2, const Vector3D& axis3);
    Cube& operator=(const Cube& rhs);

    Cube& translate(const Vector3D& offset);
    Cube& rotate(const RotationMatrix& rotation);

    Vector3D get_pos() const;
    std::array<Vector3D, 3> get_axes() const;
    std::array<Face, 6> get_faces() const;
};

class Sky
{
public:
    float avg_color = .2;
    float get_color(const Vector3D& view_direction) const;
};

class Ray
{
private:
    Vector3D origin;
    Vector3D direction;
public:
    Ray();
    Ray(const Ray& ray);
    Ray(const Vector3D& origin, const Vector3D& direction);
    Ray& operator=(const Ray& rhs);

    bool intersects(const Face& face) const;
    int get_intersection(const Cube& cube) const;
    float get_color(const Face& face, const Vector3D& sun_direction, const Sky& sky) const;

    Vector3D get_origin() const;
    Vector3D get_direction() const;
    void set_origin(const Vector3D& origin);
    void set_direction(const Vector3D& direction);
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
    Camera(const Camera& camera);
    Camera(int resx, int resy, const Vector3D& origin, const Vector3D& direction, float fov, float aspect_ratio);
    Camera& operator=(const Camera& rhs);

    Camera& translate(const Vector3D& offset);
    Camera& rotate(const RotationMatrix& rotation);

    int get_xres() const;
    int get_yres() const;
    Vector3D get_origin() const;
    Vector3D get_sensor_offset() const;
    std::array<Vector3D, 2> get_axes() const;
    std::array<Vector3D, 2> get_screenspace_basis() const;

    Ray get_pixel_ray(int x, int y) const;
    std::ostringstream snapshot(const Cube& cube, const Vector3D& sun_direction, const Sky& sky) const;
};

#endif