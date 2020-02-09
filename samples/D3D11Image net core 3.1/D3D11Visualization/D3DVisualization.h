//------------------------------------------------------------------------------
// <copyright file="D3DVizualization.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <d3d11.h>

#ifdef DIRECTX_SDK  // requires DirectX SDK June 2010
    #include <xnamath.h>
    #define DirectX_NS                 // DirectX SDK requires a blank namespace for several types
#else // Windows SDK
    #include <DirectXMath.h>
    #include <DirectXPackedVector.h>
    #define DirectX_NS DirectX         // Windows SDK requires a DirectX namespace for several types
#endif 

#include "OrbitCamera.h"
#include "DX11Utils.h"
#include "resource.h"


extern "C" {
    __declspec(dllexport) HRESULT __cdecl Init();
}

extern "C" {
    __declspec(dllexport) void __cdecl Cleanup();
}

extern "C" {
    __declspec(dllexport) HRESULT __cdecl Render(void * pResource, bool isNewSurface);
}

extern "C" {
    __declspec(dllexport) HRESULT __cdecl SetCameraRadius(float r);
}

extern "C" {
    __declspec(dllexport) HRESULT __cdecl SetCameraTheta(float theta);
}

extern "C" {
    __declspec(dllexport) HRESULT __cdecl SetCameraPhi(float phi);
}

class CCube
{

public:
    /// <summary>
    /// Constructor
    /// </summary>
    CCube();

    /// <summary>
    /// Destructor
    /// </summary>
    ~CCube();

    /// <summary>
    /// Create Direct3D device and swap chain
    /// </summary>
    /// <returns>S_OK for success, or failure code</returns>
    HRESULT                             InitDevice();

    /// <summary>
    /// Renders a frame
    /// </summary>
    /// <returns>S_OK for success, or failure code</returns>
    HRESULT                             Render(void * pResource, bool isNewSurface);


    /// <summary>
    /// Method for retrieving the camera
    /// </summary>
    /// <returns>Pointer to the camera</returns>
    CCamera*                            GetCamera();

    // Special function definitions to ensure alignment between c# and c++ 
    void* operator new(size_t size)
    {
        return _aligned_malloc(size, 16);
    }

    void operator delete(void *p)
    {
        _aligned_free(p);
    }

private:

    HRESULT InitRenderTarget(void * pResource);
    void SetUpViewport();

    // 3d camera
    CCamera                             m_camera;

    HINSTANCE                           m_hInst;
    D3D_DRIVER_TYPE                     m_driverType;
    D3D_FEATURE_LEVEL                   m_featureLevel;
    
    ID3D11Device*                       m_pd3dDevice;
    ID3D11DeviceContext*                m_pImmediateContext;
    IDXGISwapChain*                     m_pSwapChain = NULL;
    ID3D11RenderTargetView*             m_pRenderTargetView = NULL;
    ID3D11InputLayout*                  m_pVertexLayout;
    ID3D11Buffer*                       m_pVertexBuffer;
    ID3D11Buffer*                       m_pIndexBuffer = NULL;
    ID3D11Buffer*                       m_pConstantBuffer = NULL;

    DirectX_NS::XMMATRIX                 m_World;
    DirectX_NS::XMMATRIX                 m_View;
    DirectX_NS::XMMATRIX                 m_Projection;
   
    ID3D11VertexShader*                 m_pVertexShader;
    ID3D11PixelShader*                  m_pPixelShader;

    // Initial window resolution
    UINT                                 m_Width;
    UINT                                 m_Height;

    /// <summary>
    /// Compile and set layout for shaders
    /// </summary>
    /// <returns>S_OK for success, or failure code</returns>
    HRESULT                             LoadShaders();
};