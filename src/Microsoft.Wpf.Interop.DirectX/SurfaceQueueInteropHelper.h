// SurfaceQueueInteropHelper.h

#include "stdafx.h"

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;

namespace Microsoft {
    namespace Windows {
        namespace Media {

            /// A helper class which enables several versions of DirectX to share the same rendering surface.
            public ref class SurfaceQueueInteropHelper : IDisposable
            {
            private:
                Action<IntPtr, bool>^ m_renderD2D;
                D3DImage^ m_d3dImage;
                DependencyPropertyChangedEventHandler^ m_frontBufferAvailableChanged;
                UINT m_pixelWidth, m_pixelHeight;
                HWND m_hwnd;

                IDirect3D9Ex*           m_pD3D9;
                IDirect3DDevice9Ex*     m_pD3D9Device;

                ID3D10Device1*          m_D3D10Device;

                ISurfaceQueue*			m_ABQueue;
                ISurfaceQueue*			m_BAQueue;

                ISurfaceConsumer*		m_ABConsumer;
                ISurfaceProducer*		m_BAProducer;
                ISurfaceConsumer*		m_BAConsumer;
                ISurfaceProducer*		m_ABProducer;

                bool m_isD3DInitialized;
                bool m_areSurfacesInitialized;
                bool m_shouldSkipRender;

                // Could hypothetically add additional types
                enum struct QueueRenderMode
                {
                    None = 0,
                    RenderDXGI = 1
                };

                HRESULT InitD3D10();

                void RenderToDXGI(IntPtr pdxgiSurface, bool isNewSurface);

                void CleanupD3D10();

                HRESULT InitD3D9();

                void CleanupD3D9();

                void CleanupSurfaces();

                void CleanupD3D();

                HRESULT InitD3D();

                HRESULT InitSurfaces();

                bool Initialize();

                // If fShouldRenderD3D10 is true, this method performs the callout to RenderD3D10.
                // In any case, this method always initializes m_d3dImage which incurrs no cost if this results in no change.
                void QueueHelper(QueueRenderMode renderMode);

            public:

                /// The action delegate called when a render is required.
                property Action<IntPtr, bool>^ SurfaceQueueInteropHelper::RenderD2D
                {
                    void set(Action<IntPtr, bool>^ value) { m_renderD2D = value; }
                }

                /// Gets or sets the associated D3DImage object that is working in conjunction with this helper.
                property D3DImage^ SurfaceQueueInteropHelper::D3DImage
                {
                    System::Windows::Interop::D3DImage^ get()
                    {
                        return m_d3dImage;
                    }

                    void set(System::Windows::Interop::D3DImage^ d3dImage)
                    {
                        if (d3dImage != m_d3dImage)
                        {
                            if (nullptr != m_d3dImage)
                            {
                                m_d3dImage->SetBackBuffer(System::Windows::Interop::D3DResourceType::IDirect3DSurface9, (IntPtr)nullptr);
                            }

                            m_d3dImage = d3dImage;

                            // TODO: Force a rerender...?
                        }
                    }
                }

                /// Gets the desired pixel width for the surface.
                property unsigned int SurfaceQueueInteropHelper::PixelWidth
                {
                    unsigned int get()
                    {
                        return m_pixelWidth;
                    }
                }

                /// Gets the desired pixel height for the surface.
                property unsigned int SurfaceQueueInteropHelper::PixelHeight
                {
                    unsigned int get()
                    {
                        return m_pixelHeight;
                    }
                }

                /// Enables user of this component to set the desired pixel size for the surface.
                void SetPixelSize(unsigned int pixelWidth, unsigned int pixelHeight);

                /// Gets or sets the HWND used by the helper while creating DirectX devices.
                property IntPtr SurfaceQueueInteropHelper::HWND
                {
                    IntPtr get() { return (IntPtr)(void*)m_hwnd; }
                    void set(IntPtr hwnd) { m_hwnd = (::HWND)(void*)hwnd; }
                }

                /// Requests render to happen.
                void RequestRenderD2D();

                !SurfaceQueueInteropHelper();

                ~SurfaceQueueInteropHelper();
            };

        }
    }
}
