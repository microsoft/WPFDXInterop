// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>

#include "SurfaceQueue.h"

#if DIRECTX_SDK
#include <d3dx9.h>
#else
#include "d3d9.h"
#endif

#include <D3D10_1.h>
//#include <d3dx10.h>


#define IFC(x) { hr = (x); if (FAILED(hr)) { goto Cleanup; }}
#define ReleaseInterface(x) { if (NULL != x) { x->Release(); x = NULL; }}