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

                public ref class D3D11Image :
                    public D3DImage
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

                    property Action<IntPtr>^ OnRender
                    {
                        Action<IntPtr>^ get()
                        {
                            return static_cast<Action<IntPtr>^>(GetValue(OnRenderProperty));
                        }

                        void set(Action<IntPtr>^ value)
                        {
                            SetValue(OnRenderProperty, value);
                        }
                    }

                    property IntPtr WindowOwner
                    {
                        IntPtr get()
                        {
                            return static_cast<IntPtr>(GetValue(WindowOwnerProperty));
                        }

                        void set(IntPtr value)
                        {
                            SetValue(WindowOwnerProperty, value);
                        }
                    }

                    void RequestRender();
                    
                    void SetPixelSize(int pixelWidth, int pixelHeight);
                };

            }
        }
    }
}