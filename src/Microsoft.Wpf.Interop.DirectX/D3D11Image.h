// SurfaceQueueInteropHelper.h

#pragma once

#include "SurfaceQueueInteropHelper.h"


using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;
using namespace Microsoft::Windows::Media;

namespace Microsoft {
    namespace Wpf {
        namespace Interop {
            namespace DirectX {

                ref class D3D11Image :
                    public D3DImage//, IDisposable
                {
                private: 
                    static void RenderChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs args);
                    static void HWNDOwnerChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs args);
                    void EnsureHelper();
                    static D3D11Image();
            
                internal:
                    SurfaceQueueInteropHelper^ _helper;

                protected:
                    Freezable^ CreateInstanceCore() override;

                public:
                    D3D11Image();
                    ~D3D11Image();

                    static DependencyProperty^ OnRenderProperty;
                    static DependencyProperty^ WindowOwnerProperty;

                    Action<IntPtr>^ get_OnRender();
                    void set_OnRender(Action<IntPtr>^ value);

                    IntPtr get_WindowOwner();
                    void set_WindowOwner(IntPtr value);

                    void RequestRender();
                    
                    void SetPixelSize(int pixelWidth, int pixelHeight);
                };

            }
        }
    }
}