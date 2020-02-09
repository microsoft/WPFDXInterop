//------------------------------------------------------------------------------
// <copyright file="DX11Utils.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <d3d11.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

/// <summary>
/// Helper for compiling shaders with D3DX11
/// </summary>
/// <param name="szFileName">full path to shader to compile</param>
/// <param name="szEntryPoint">entry point of shader</param>
/// <param name="szShaderModel">shader model to compile for</param>
/// <param name="ppBlobOut">holds result of compilation</param>
/// <returns>S_OK for success, or failure code</returns>
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D10Blob** ppBlobOut );