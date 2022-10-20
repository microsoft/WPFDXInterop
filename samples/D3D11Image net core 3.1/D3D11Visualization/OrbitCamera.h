//------------------------------------------------------------------------------
// <copyright file="OrbitCamera.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>

#ifdef DIRECTX_SDK  // requires DirectX SDK June 2010
    #include <xnamath.h>
    #define DirectX_NS                 // DirectX SDK requires a blank namespace for several types
#else // Windows SDK
    #include <DirectXMath.h>
    #include <DirectXPackedVector.h>
    #define DirectX_NS DirectX         // Windows SDK requires a DirectX namespace for several types
#endif 


class CCamera
{
public:
    DirectX_NS::XMMATRIX View;

    /// <summary>
    /// Constructor
    /// </summary>
    CCamera();

    /// <summary>
    /// Reset the camera state to initial values
    /// </summary>
    void Reset();

    /// <summary>
    /// Update the view matrix
    /// </summary>
    void Update();

    /// <summary>
    /// Move camera into position
    /// </summary>
    int UpdatePosition();

    /// <summary>
    /// Sets the R value of the camera.
    /// R value represents the distance of the camera from the center
    /// </summary>
    void SetRadius(float r);

    /// <summary>
    /// Sets the Theta value of the camera from around the depth center
    /// Theta represents the angle (in radians) of the camera around the 
    /// center in the x-y plane (circling around players)
    /// </summary>
    void SetTheta(float theta);

    /// <summary>
    /// Sets the Phi value of the camera
    /// Phi represents angle (in radians) of the camera around the center 
    /// in the y-z plane (over the top and below players)
    /// </summary>
    void SetPhi(float phi);

    /// <summary>
    /// Get the camera's up vector
    /// </summary>
    /// <returns>camera's up vector</returns>
    DirectX_NS::XMVECTOR  GetUp() { return m_up; }

    /// <summary>
    /// Get the camera's position vector
    /// </summary>
    /// <returns>camera's position vector</returns>
    DirectX_NS::XMVECTOR  GetEye() { return m_eye; }

    /// <summary>
    /// Sets the center depth of the rendered image
    /// </summary>
    void SetCenterDepth(float depth);

private:
    float r;
    float theta;
    float phi;

    DirectX_NS::XMVECTOR  m_eye;
    DirectX_NS::XMVECTOR  m_at;
    DirectX_NS::XMVECTOR  m_up;
};