#include "GLConv.h"
#include <cmath>

void GLConv::Perspective(float &x, float &y, float &z, float L, float R, float B, float T, float N, float F)
{
    float vx = x;
    float vy = y;
    float vz = z;
    x = Caramer_to_NDC_X_LRN(vx, vz, L, R, N);
    y = Caramer_to_NDC_Y_BTN(vy, vz, B, T, N);
    z = Caramer_to_NDC_Z(vz, N, F);
}

void GLConv::Perspective(float &x, float &y, float &z, float fov, float N, float F, float aspectRatio)
{
    float vx = x;
    float vy = y;
    float vz = z;
    x = GLConv::Caramer_to_NDC_X_FOV(vx, vz, fov, aspectRatio);
    y = GLConv::Caramer_to_NDC_Y_FOV(vy, vz, fov);
    z = GLConv::Caramer_to_NDC_Z(vz, N, F);
}

float GLConv::Caramer_to_NDC_X_LRN(float x,  float z, float frustumLeft, float frustumRight, float frustumNear)
{
    //摄像机 -> 近景面
    float near = frustumNear * x / -z;
    //近景面 -> NDC
    float ndc = ( 2.f * near / (frustumRight - frustumLeft) ) - ( (frustumRight + frustumLeft) / (frustumRight - frustumLeft) );
    return ndc;
}

float GLConv::Caramer_to_NDC_X_FOV(float x, float z, float fov, float aspectRatio)
{   
    /*
    1.摄像机 -> 近景面 
        float nearX = frustumNear * x / -z; ①
        
    2.近景面 -> NDC
        float ndc = ( 2.f * nearX / (frustumRight - frustumLeft) ) - ( (frustumRight + frustumLeft) / (frustumRight - frustumLeft) ); ②
       
    3. 将①带入②, 得到, 摄像机 -> NDC
        float ndc = ( 2.f * (frustumNear * x / -z) / (frustumRight - frustumLeft) ) - ( (frustumRight + frustumLeft) / (frustumRight - frustumLeft) ); ③
        
    4.已知  frustumLeft = -frustumLeftRight, 带入 ③, 得到
        float ndc = ((frustumNear * x / (-z)) / frustumLeftRight); ④
       
    5.已知 frustumLeftRight = frustumBottomTop * aspectRatio, 带入 ④, 得到
        float ndc = ((frustumNear * x / (-z)) / (frustumBottomTop * aspectRatio)); ⑤
        
    6.已知 std::tan(DegreeTORadius(fov / 2.f) = frustumBottomTop / frustumNear, 带入 ⑤, 得到
        float ndc = x / (std::tan(DegreeTORadius(fov / 2.f)) * aspectRatio * (-z));
    */
    float ndc = x / (std::tan(Degree_to_Radian(fov / 2.f)) * aspectRatio * (-z));
    return ndc;
}

float GLConv::Caramer_to_NDC_Y_BTN(float view, float z, float frustumBottum, float frustumTop, float frustumNear)
{
    //摄像机 -> 近景面
    float near = frustumNear * view / -z;
    //近景面 -> ndc[-1, 1]
    float ndc = (2.f * near / (frustumTop - frustumBottum)) - ((frustumTop + frustumBottum) / (frustumTop - frustumBottum));
    return ndc;
}

float GLConv::Caramer_to_NDC_Y_FOV(float y, float z, float fov)
{
    /*
    1.摄像机 -> 近景面 
        float nearY = frustumNear * y / -z; ①
        
    2.近景面 -> NDC
        float ndc = ( 2.f * nearY / (frustumTop - frustumBottom) ) - ( (frustumTop + frustumBottom) / (frustumTop - frustumBottom) ); ②
       
    3. 将①带入②, 得到, 摄像机 -> NDC
        float ndc = ( 2.f * (frustumNear * y / -z) / (frustumTop - frustumBottom) ) - ( (frustumTop + frustumBottom) / (frustumTop - frustumBottom) ); ③
        
    4.已知  frustumTop = -frustumBottom, 带入 ③, 得到
        float ndc = ((frustumNear * y / (-z)) / frustumTop); ④
        
    5.已知 std::tan(DegreeTORadius(fov / 2.f) = frustumBottomTop / frustumNear, 带入 ④, 得到
        float ndc = y / (std::tan(DegreeTORadius(fov / 2.f)) * (-z));
    */
    float ndc = y / (std::tan(Degree_to_Radian(fov / 2.f)) * (-z));
    
    return ndc;
}

float GLConv::Caramer_to_NDC_Z(float z, float near, float far)
{
    float ndc = 2.f * far * near / (z * (far - near)) + (near + far) / (far - near);
    return ndc;
}


float GLConv::Degree_to_Radian(float degree)
{
    return degree * PI / 180.f;
}

float GLConv::Radian_to_Degree(float radius)
{
    return radius * 180.f / PI;
}
