#include "D3D11Image.h"

namespace Microsoft {
    namespace Wpf {
        namespace Interop {
            namespace DirectX {

                static D3D11Image::D3D11Image()
                {
                    OnRenderProperty = DependencyProperty::Register("OnRender",
                        Action<IntPtr>::typeid,
                        D3D11Image::typeid,
                        gcnew UIPropertyMetadata(nullptr, gcnew PropertyChangedCallback(&RenderChanged)));

                    WindowOwnerProperty = DependencyProperty::Register("WindowOwner",
                        IntPtr::typeid,
                        D3D11Image::typeid,
                        gcnew UIPropertyMetadata(IntPtr::Zero, gcnew PropertyChangedCallback(&HWNDOwnerChanged)));
                }

                D3D11Image::D3D11Image()
                {
                }

                D3D11Image::~D3D11Image()
                {
                    if (this->_helper != nullptr)
                    {
                        this->_helper->~SurfaceQueueInteropHelper();
                        this->_helper = nullptr;
                    }
                }

                Freezable^ D3D11Image::CreateInstanceCore()
                {
                    return gcnew D3D11Image();
                }

                void D3D11Image::HWNDOwnerChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs args)
                {
                    D3D11Image^ image = dynamic_cast<D3D11Image^>(sender);

                    if (image != nullptr)
                    {
                        if (image->_helper != nullptr)
                        {
                            image->_helper->HWND = static_cast<IntPtr>(args.NewValue);
                        }
                    }
                }

                void D3D11Image::RenderChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs args)
                {
                    D3D11Image^ image = dynamic_cast<D3D11Image^>(sender);

                    if (image != nullptr)
                    {
                        if (image->_helper != nullptr)
                        {
                            image->_helper->RenderD2D = static_cast<Action<IntPtr>^>(args.NewValue);
                        }
                    }
                }

                Action<IntPtr>^ D3D11Image::get_OnRender()
                {
                    return static_cast<Action<IntPtr>^>(GetValue(OnRenderProperty));
                }

                void D3D11Image::set_OnRender(Action<IntPtr>^ value)
                {
                    SetValue(OnRenderProperty, value);
                }

                IntPtr D3D11Image::get_WindowOwner()
                {
                    return static_cast<IntPtr>(GetValue(WindowOwnerProperty));
                }

                void D3D11Image::set_WindowOwner(IntPtr value)
                {
                    SetValue(WindowOwnerProperty, value);
                }

                void D3D11Image::EnsureHelper()
                {
                    if (this->_helper == nullptr)
                    {
                        this->_helper = gcnew SurfaceQueueInteropHelper();
                        this->_helper->HWND = this->get_WindowOwner();
                        this->_helper->D3DImage = this;
                        this->_helper->RenderD2D = this->get_OnRender();
                    }
                }

                void D3D11Image::RequestRender()
                {
                    this->EnsureHelper();

                    // Don't bother with a call if there's no callback registered.
                    if (nullptr != this->get_OnRender())
                    {
                        this->_helper->RequestRenderD2D();
                    }
                }

                void D3D11Image::SetPixelSize(int pixelWidth, int pixelHeight)
                {
                    this->EnsureHelper();
                    this->_helper->SetPixelSize(static_cast<UInt32>(pixelWidth), static_cast<UInt32>(pixelHeight));
                }
            }
        }
    }
}