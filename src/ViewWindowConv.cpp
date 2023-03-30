#include "ViewWindowConv.h"

ViewWindowConv::ViewWindowConv()
{

}

ViewWindowConv::~ViewWindowConv()
{

}

float ViewWindowConv::ViewTOWindow_X_LRN(float view, float frustumLeft, float frustumRight, float frustumNear, float z, float screenWidth)
{
    //摄像机 -> 近景面
    float near = frustumNear * view / -z;
    //近景面 -> 裁剪[-1, 1]
    float clip = ( 2.f * near / (frustumLeft - frustumRight) ) - ( (frustumLeft + frustumRight) / (frustumLeft - frustumRight) );
    //裁剪 -> 屏幕
    float window = (1.f - clip) / 2.f * screenWidth;
    return window;
}

float ViewWindowConv::ViewTOWindow_X_FOV(float view, float fov, float cameraDistance, float screenWidth, float aspectRatio)
{
    /*
     *
     * 摄像机 -> 近景面
     * near = frustumNear * view / cameraDistance
     * 已知 近景面 [frustumLeft, frustumRight] -> 裁剪 [-1, 1] 满足下面等式
     * clip = (2.f * near / (frustumLeft - frustumRight)) - ((frustumLeft + frustumRight) / (frustumLeft - frustumRight))
     * 因为 frustumLeftRight = frustumLeft = -frustumRight，带入上面等式，得到
     * clip = near / frustumLeftRight;
     * 将 near 带入上面的等式，得到
     * clip = (frustumNear / frustumLeftRight) * (view / cameraDistance)
     * 因为 frustumLeftRight = frustumBottomTop * aspectRatio，带入上面等式，得到
     * clip = (frustumNear / frustumBottomTop * aspectRatio) * (view / cameraDistance)
     * 因为 frustumNear / frustumBottomTop = 1 / tan(fov / 2.f)， 带入上面的等式， 得到
     * clip = view / (tan(fov / 2.f) * aspectRatio * cameraDistance)
    */
    float clip = view / (tan(glm::radians(fov / 2.f)) * aspectRatio * cameraDistance);
    float window = (1.f - clip) / 2.f * screenWidth;
    return window;
}

float ViewWindowConv::ViewTOWindow_Y_BTN(float view, float frustumBottum, float frustumTop, float frustumNear, float z, float screenHeight)
{
    //摄像机 -> 近景面
    float near = frustumNear * view / -z;
    //近景面 -> 裁剪[-1, 1]
    float clip = (2.f * near / (frustumTop - frustumBottum)) - ((frustumTop + frustumBottum) / (frustumTop - frustumBottum));
    //裁剪 -> 屏幕
    float window = (1.f - clip) / 2.f * screenHeight;
    return window;
}

float ViewWindowConv::ViewTOWindow_Y_FOV(float view, float fov, float cameraDistance, float screenHeight)
{
    /*
     * 摄像机 -> 近景面
     * near = frustumNear * view / cameraDistance
     * 已知 近景面 [frustumBottum, frustumTop] -> 裁剪 [-1, 1] 满足下面等式
     * clip = (2.f * near / (frustumTop - frustumBottum)) - ((frustumTop + frustumBottum) / (frustumTop - frustumBottum))
     * 因为 frustumBottomTop = frustumTop = -frustumBottum，带入上面的等式，得到
     * clip = near / frustumBottomTop
     * 将 near 带入上面的等式，得到崔额
     * 0
     * clip = (frustumNear / frustumBottomTop) * (view / cameraDistance)
     * 因为 frustumNear / frustumBottomTop = 1 / tan(fov / 2.f)， 带入上面的等式， 得到
     * clip = view / (tan(fov / 2.f) * cameraDistance)
    */
    float clip = view / (tan(glm::radians(fov / 2.f)) * cameraDistance);
    float window = (1.f - clip) / 2.f * screenHeight;
    return window;
}

float ViewWindowConv::ViewTOWindow_Z(float view, float near, float far)
{
    //摄像机 [n, f] -> [-1, 1]
//            float clip = 2.f * view / (near - far) + (near + far) / (near - far);
    float clip = 2.f * far * near / (view * (far - near)) + (near + far) / (far - near);
    return clip;
}
