namespace Microsoft.Samples.Wpf.D3D11Interop
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows;
    using System.Windows.Input;
    using System.Windows.Interop;
    using System.Windows.Media;

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Magnifier Image Settings
        private const double MagImageScale = 1.25; // Scale of image to magnified ellipse
        private const double MagImageOffset = 0.12; // Offset of magnified ellipse within image

        // Unit conversion
        private const float DegreesToRadians = (float)Math.PI / 180;

        // State Management
        private bool magnify = true;
        TimeSpan lastRender;
        bool lastVisible;

        // Magnifier Settings (filled by default slider vlaues)
        private double magSize;
        private double magScale;

        public MainWindow()
        {
            this.InitializeComponent();
            this.host.Loaded += new RoutedEventHandler(this.Host_Loaded);
            this.host.SizeChanged += new SizeChangedEventHandler(this.Host_SizeChanged);
        }
        
        private static bool Init()
        {
            
            bool initSucceeded = NativeMethods.InvokeWithDllProtection(() => NativeMethods.Init()) >= 0;
            
            if (!initSucceeded)
            {
                MessageBox.Show("Failed to initialize.", "WPF D3D Interop", MessageBoxButton.OK, MessageBoxImage.Error);

                if (Application.Current != null)
                {
                    Application.Current.Shutdown();
                }
            }

            return initSucceeded;
        }

        private static void Cleanup()
        {
            NativeMethods.InvokeWithDllProtection(NativeMethods.Cleanup);
        }

        private static int Render(IntPtr resourcePointer, bool isNewSurface)
        {
            return NativeMethods.InvokeWithDllProtection(() => NativeMethods.Render(resourcePointer, isNewSurface));      
        }

        private static int SetCameraRadius(float radius)
        {
            return NativeMethods.InvokeWithDllProtection(() => NativeMethods.SetCameraRadius(radius));
        }

        private static int SetCameraTheta(float theta)
        {
            return NativeMethods.InvokeWithDllProtection(() => NativeMethods.SetCameraTheta(theta));    
        }

        private static int SetCameraPhi(float phi)
        {
            return NativeMethods.InvokeWithDllProtection(() => NativeMethods.SetCameraPhi(phi));
        }

        #region Callbacks
        private void Host_Loaded(object sender, RoutedEventArgs e)
        {
            Init();
            this.InitializeRendering();

            // Setup the Magnifier Size
            MagEllipse.Height = this.magSize;
            MagEllipse.Width = this.magSize;
            Scale.Value = this.magScale;

            // Add mouse over event
            host.MouseMove += this.MagElement_MouseMove;
            ImageHost.MouseMove += this.MagElement_MouseMove;
            MagEllipse.MouseMove += this.MagElement_MouseMove;
            MagImage.MouseMove += this.MagElement_MouseMove;

            host.MouseLeave += this.MagElement_MouseLeave;
            MagEllipse.MouseLeave += this.MagElement_MouseLeave;
            ImageHost.MouseLeave += this.MagElement_MouseLeave;
            MagImage.MouseLeave += this.MagElement_MouseLeave;

            MagBox.Checked += this.MagBox_Checked;
            MagBox.Unchecked += this.MagBox_Unchecked;
        }

        private void Host_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            double dpiScale = 1.0; // default value for 96 dpi

            // determine DPI
            // (as of .NET 4.6.1, this returns the DPI of the primary monitor, if you have several different DPIs)
            var hwndTarget = PresentationSource.FromVisual(this).CompositionTarget as HwndTarget;
            if (hwndTarget != null)
            {
                dpiScale = hwndTarget.TransformToDevice.M11;
            }

            int surfWidth = (int)(host.ActualWidth < 0 ? 0 : Math.Ceiling(host.ActualWidth * dpiScale));
            int surfHeight = (int)(host.ActualHeight < 0 ? 0 : Math.Ceiling(host.ActualHeight * dpiScale));

            // Notify the D3D11Image of the pixel size desired for the DirectX rendering.
            // The D3DRendering component will determine the size of the new surface it is given, at that point.
            InteropImage.SetPixelSize(surfWidth, surfHeight);
                
            // Stop rendering if the D3DImage isn't visible - currently just if width or height is 0
            // TODO: more optimizations possible (scrolled off screen, etc...)
            bool isVisible = (surfWidth != 0 && surfHeight != 0);
            if (lastVisible != isVisible)
            {
                lastVisible = isVisible;
                if (lastVisible)
                {
                    CompositionTarget.Rendering += CompositionTarget_Rendering;
                }
                else
                {
                    CompositionTarget.Rendering -= CompositionTarget_Rendering;
                }
            }
        }

        private void Scale_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            this.magScale = e.NewValue;
        }

        private void Size_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            this.magSize = e.NewValue;

            // Setup the Magnifier Size
            this.MagEllipse.Height = this.magSize;
            this.MagEllipse.Width = this.magSize;
        }

        private void MagBox_Checked(object sender, RoutedEventArgs e)
        {
            this.magnify = true;

            MagCurserToggle1.Cursor = System.Windows.Input.Cursors.None;
            MagCurserToggle2.Cursor = System.Windows.Input.Cursors.None;
            host.Cursor = System.Windows.Input.Cursors.None;
        }

        private void MagBox_Unchecked(object sender, RoutedEventArgs e)
        {
            this.magnify = false;

            MagCurserToggle1.Cursor = System.Windows.Input.Cursors.Arrow;
            MagCurserToggle2.Cursor = System.Windows.Input.Cursors.Arrow;
            host.Cursor = System.Windows.Input.Cursors.Arrow;
        }

        private void MagElement_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.magnify)
            {
                Point point = Mouse.GetPosition(host);

                if (!(point.X < 0 || point.Y < 0 || point.X > host.ActualWidth || point.Y > host.ActualHeight))
                {
                    // Draw the Magnified ellipse on top of image
                    System.Windows.Controls.Canvas.SetTop(this.MagEllipse, point.Y - (this.magSize / 2));
                    System.Windows.Controls.Canvas.SetLeft(this.MagEllipse, point.X - (this.magSize / 2));

                    // Set the magnifier image on top of magnified ellipse 
                    System.Windows.Controls.Canvas.SetTop(this.MagImage, point.Y - (this.magSize * (.5 + MagImageOffset)));
                    System.Windows.Controls.Canvas.SetLeft(this.MagImage, point.X - (this.magSize * (.5 + MagImageOffset)));
                    MagImage.Width = this.magSize * MagImageScale;

                    MagEllipse.Visibility = System.Windows.Visibility.Visible;
                    MagImage.Visibility = System.Windows.Visibility.Visible;

                    double magViewboxSize = this.magSize / this.magScale;
                    MagBrush.Viewbox = new Rect(point.X - (.5 * magViewboxSize), point.Y - (.5 * magViewboxSize), magViewboxSize, magViewboxSize);
                }
                else
                {
                    MagEllipse.Visibility = Visibility.Hidden;
                    MagImage.Visibility = Visibility.Hidden;
                }
            }
        }

        private void MagElement_MouseLeave(object sender, MouseEventArgs e)
        {
            MagEllipse.Visibility = Visibility.Hidden;
            MagImage.Visibility = Visibility.Hidden;
        }

        private void Radius_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetCameraRadius((float)e.NewValue);
        }

        private void Theta_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetCameraTheta((float)e.NewValue * DegreesToRadians);
        }

        private void Phi_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetCameraPhi((float)e.NewValue * DegreesToRadians);
        }
        #endregion Callbacks

        #region Helpers
        private void InitializeRendering()
        {
            InteropImage.WindowOwner = (new System.Windows.Interop.WindowInteropHelper(this)).Handle;
            InteropImage.OnRender = this.DoRender;

            // Set up camera
            SetCameraRadius((float)RadiusSlider.Value);
            SetCameraPhi((float)PhiSlider.Value * DegreesToRadians);
            SetCameraTheta((float)ThetaSlider.Value * DegreesToRadians);

            // Start rendering now!
            InteropImage.RequestRender();
        }

        void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            RenderingEventArgs args = (RenderingEventArgs)e;

            // It's possible for Rendering to call back twice in the same frame 
            // so only render when we haven't already rendered in this frame.
            if (this.lastRender != args.RenderingTime)
            {
                InteropImage.RequestRender();
                this.lastRender = args.RenderingTime;
            }
        }

        private void UninitializeRendering()
        {
            Cleanup();

            CompositionTarget.Rendering -= this.CompositionTarget_Rendering;
        }
        #endregion Helpers

        private void DoRender(IntPtr surface, bool isNewSurface)
        {
            Render(surface, isNewSurface);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            this.UninitializeRendering();

            host.MouseMove -= this.MagElement_MouseMove;
            ImageHost.MouseMove -= this.MagElement_MouseMove;
            MagEllipse.MouseMove -= this.MagElement_MouseMove;
            MagImage.MouseMove -= this.MagElement_MouseMove;

            host.MouseLeave -= this.MagElement_MouseLeave;
            MagEllipse.MouseLeave -= this.MagElement_MouseLeave;
            ImageHost.MouseLeave -= this.MagElement_MouseLeave;
            MagImage.MouseLeave -= this.MagElement_MouseLeave;

            MagBox.Checked -= this.MagBox_Checked;
            MagBox.Unchecked -= this.MagBox_Unchecked;
        }

        private static class NativeMethods
        {
            /// <summary>
            /// Variable used to track whether the missing dependency dialog has been displayed,
            /// used to prevent multiple notifications of the same failure.
            /// </summary>
            private static bool errorHasDisplayed;

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int Init();

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void Cleanup();

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int Render(IntPtr resourcePointer, bool isNewSurface);

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int SetCameraRadius(float radius);

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int SetCameraTheta(float theta);

            [DllImport("D3DVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int SetCameraPhi(float phi);

            /// <summary>
            /// Method used to invoke an Action that will catch DllNotFoundExceptions and display a warning dialog.
            /// </summary>
            /// <param name="action">The Action to invoke.</param>
            public static void InvokeWithDllProtection(Action action)
            {
                InvokeWithDllProtection(
                    () => 
                    { 
                        action.Invoke();
                        return 0;
                    }); 
            }

            /// <summary>
            /// Method used to invoke A Func that will catch DllNotFoundExceptions and display a warning dialog.
            /// </summary>
            /// <param name="func">The Func to invoke.</param>
            /// <returns>The return value of func, or default(T) if a DllNotFoundException was caught.</returns>
            /// <typeparam name="T">The return type of the func.</typeparam>
            public static T InvokeWithDllProtection<T>(Func<T> func)
            {
                try
                {
                    return func.Invoke();
                }
                catch (DllNotFoundException e)
                {
                    if (!errorHasDisplayed)
                    {
                        MessageBox.Show("This sample requires:\nManual build of the D3DVisualization project, which requires installation of Windows 10 SDK or DirectX SDK.\n" +
                                        "Installation of the DirectX runtime on non-build machines.\n\n"+
                                        "Detailed exception message: " + e.Message, "WPF D3D11 Interop",
                                        MessageBoxButton.OK, MessageBoxImage.Error);
                        errorHasDisplayed = true;

                        if (Application.Current != null)
                        {
                            Application.Current.Shutdown();                            
                        }
                    }
                }

                return default(T);
            }
        }
    }
}
