using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using SharpDX.Direct2D1;

namespace SharpDx2D
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TimeSpan lastRenderTime;
        private RenderTarget renderTarget = null;
        private SharpDX.DirectWrite.TextLayout textLayout;
        private SharpDX.Direct2D1.Brush brush;

        public MainWindow()
        {
            InitializeComponent();
            Loaded += MainWindow_Loaded;
            SizeChanged += MainWindow_SizeChanged;
        }

        private void MainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Grid g = (Grid)Content;
            if (g.ActualWidth > 0 && g.ActualHeight > 0)
            {
                InteropImage.SetPixelSize((int)g.ActualWidth, (int)g.ActualHeight);
            }
            else
            {
                Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Background, (Action<object, SizeChangedEventArgs>)MainWindow_SizeChanged, null, null);
            }
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            SharpDX.DirectWrite.Factory factory = new SharpDX.DirectWrite.Factory(SharpDX.DirectWrite.FactoryType.Isolated);
            SharpDX.DirectWrite.TextFormat textFormat = new SharpDX.DirectWrite.TextFormat(factory, "Arial", 30);
            textLayout = new SharpDX.DirectWrite.TextLayout(factory, "Test", textFormat, 0, 0);
            MainWindow_SizeChanged(null, null);
            InteropImage.WindowOwner = (new System.Windows.Interop.WindowInteropHelper(this)).Handle;
            InteropImage.OnRender = OnRender;
            CompositionTarget.Rendering += CompositionTarget_Rendering;
        }

        private void OnRender(IntPtr handle, bool isNewSurface)
        {
            if (isNewSurface)
            {
                if (brush != null)
                {
                    brush.Dispose();
                    brush = null;
                }

                if (renderTarget != null)
                {
                    renderTarget.Dispose();
                    renderTarget = null;
                }

                SharpDX.ComObject comObject = new SharpDX.ComObject(handle);
                SharpDX.DXGI.Resource resource = comObject.QueryInterface<SharpDX.DXGI.Resource>();
                SharpDX.Direct3D10.Texture2D texture = resource.QueryInterface<SharpDX.Direct3D10.Texture2D>();
                using (var surface = texture.QueryInterface<SharpDX.DXGI.Surface>())
                {
                    var properties = new RenderTargetProperties();
                    properties.DpiX = 96;
                    properties.DpiY = 96;
                    properties.MinLevel = FeatureLevel.Level_DEFAULT;
                    properties.PixelFormat = new SharpDX.Direct2D1.PixelFormat(SharpDX.DXGI.Format.Unknown, AlphaMode.Premultiplied);
                    properties.Type = RenderTargetType.Default;
                    properties.Usage = RenderTargetUsage.None;

                    renderTarget = new RenderTarget(new Factory(), surface, properties);
                }
            }

            if (brush == null)
            {
                brush = new SharpDX.Direct2D1.SolidColorBrush(renderTarget, new SharpDX.Color4(0.2f, 0.2f, 0.2f, 0.5f));
            }

            renderTarget.BeginDraw();
            renderTarget.DrawTextLayout(new SharpDX.Vector2(50, 50), textLayout, brush);
            renderTarget.EndDraw();
        }

        private void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            RenderingEventArgs args = (RenderingEventArgs)e;

            // It's possible for Rendering to call back twice in the same frame
            // so only render when we haven't already rendered in this frame.
            if (lastRenderTime != args.RenderingTime)
            {
                InteropImage.RequestRender();
                lastRenderTime = args.RenderingTime;
            }
        }
    }
}
