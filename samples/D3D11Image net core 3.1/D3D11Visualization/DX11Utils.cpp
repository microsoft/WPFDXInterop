//------------------------------------------------------------------------------
// <copyright file="DX11Utils.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "DX11Utils.h"

#ifdef DIRECTX_SDK  // requires DirectX SDK June 2010
    #include <d3dx11.h>
#else // Windows SDK
    #include <d3d11.h>
    #include <D3Dcompiler.h>
#endif 

/// <summary>
/// Helper for compiling shaders with D3DX11
/// </summary>
/// <param name="szFileName">full path to shader to compile</param>
/// <param name="szEntryPoint">entry point of shader</param>
/// <param name="szShaderModel">shader model to compile for</param>
/// <param name="ppBlobOut">holds result of compilation</param>
/// <returns>S_OK for success, or failure code</returns>
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D10Blob** ppBlobOut)
{
    HRESULT hr = S_OK;

    ID3D10Blob* pErrorBlob = NULL;
    
#ifdef DIRECTX_SDK  // requires DirectX SDK June 2010
    hr = D3DX11CompileFromFileW( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        0, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
#else // Windows SDK
    hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
        0, 0, ppBlobOut, &pErrorBlob);
#endif 


    if ( FAILED(hr) )
    {
        if (NULL != pErrorBlob)
        {
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE(pErrorBlob);

    return hr;
}