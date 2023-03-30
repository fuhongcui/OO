#ifndef VIEWWINDOWCONV_H
#define VIEWWINDOWCONV_H
#include <glm/glm.hpp>

class ViewWindowConv
{
public:
    ViewWindowConv();
    ~ViewWindowConv();
    static float ViewTOWindow_X_LRN(float view, float frustumLeft, float frustumRight, float frustumNear, float z, float screenWidth);
    static float ViewTOWindow_X_FOV(float view, float fov, float cameraDistance, float screenWidth, float aspectRatio);
    static float ViewTOWindow_Y_BTN(float view, float frustumBottum, float frustumTop, float frustumNear, float z, float screenHeight);
    static float ViewTOWindow_Y_FOV(float view, float fov, float cameraDistance, float screenHeight);
    static float ViewTOWindow_Z(float view, float near, float far);
};

#endif // VIEWWINDOWCONV_H
