#ifndef VIEWWINDOWCONV_H
#define VIEWWINDOWCONV_H

static const float PI = 3.14159f;

struct Point3D
{
    Point3D() = default;
    Point3D(float x, float y, float z)
        :x(x), y(y), z(z)
    {
        
    }
    float x { 0.f };
    float y { 0.f };
    float z { 0.f };
};

struct Point3DUV
{
    Point3DUV() = default;
    Point3DUV(float x, float y, float z, float u, float v)
        :x(x), y(y), z(z), u(u), v(v)
    {
        
    }
    float x { 0.f };
    float y { 0.f };
    float z { 0.f };
    float u { 0.f };
    float v { 0.f };
};

class GLConv
{
public:
    //透视
    static void Perspective(float& x, float& y, float& z, float L, float R, float B, float T, float N, float F);
    static void Perspective(float& x, float& y, float& z, float fov, float N, float F, float aspectRatio);
    //基本函数
    static float Degree_to_Radian(float degree);
    static float Radian_to_Degree(float radius);
private:
    /*--------------------------------------- 透视 --------------------------------------------------------------*/
    //frustum
    static float Caramer_to_NDC_X_LRN(float x, float z, float frustumLeft, float frustumRight, float frustumNear);
    static float Caramer_to_NDC_Y_BTN(float y, float z, float frustumBottum, float frustumTop, float frustumNear);
    //fov
    static float Caramer_to_NDC_X_FOV(float x, float z, float fov, float aspectRatio);
    static float Caramer_to_NDC_Y_FOV(float y, float z, float fov);
    //deepth
    static float Caramer_to_NDC_Z(float z, float near, float far);
    
};

#endif // VIEWWINDOWCONV_H
