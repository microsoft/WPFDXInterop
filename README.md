# **WPF DirectX Extensions**
WPF DirectX Extensions allow you to easily host DirectX 10 and DirectX 11 content in WPF applications.

Getting Started
-------------------
 **Where to get it**

 - NuGet package
 	- [x86](https://www.nuget.org/packages/Microsoft.Wpf.Interop.DirectX-x86/0.9.0-beta-22856)
	- [x64](https://www.nuget.org/packages/Microsoft.Wpf.Interop.DirectX-x64/0.9.0-beta-22856)
 - [Source Code](https://github.com/Microsoft/WPFDXInterop)

**Resources**

 - [Documentation](https://github.com/Microsoft/WPFDXInterop/wiki)
 - [Samples](/samples)

**More Info**

 - [Report a bug or ask a question](https://github.com/Microsoft/WPFDXInterop/issues)
 - [License](http://opensource.org/licenses/MIT)

Code Example
------------

***XAML***
```
<Window x:Class="WpfApplication5.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
        xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
        xmlns:local="clr-namespace:WpfApplication5"
        mc:Ignorable="d"
        xmlns:DXExtensions="clr-namespace:Microsoft.Wpf.Interop.DirectX;assembly=Microsoft.Wpf.Interop.DirectX"
        Title="MainWindow" Height="350" Width="525">
    <Grid>
        <Image>
            <Image.Source>
                <DXExtensions:D3D11Image  x:Name="InteropImage"/>
            </Image.Source>
        </Image>
    </Grid>
</Window>
```

***C#***

The C# portions of interfacing with a native component that generates the DX visualization is not concise enough to host as an example. We would recommend that you look at sample code [here](https://github.com/Microsoft/WPFDXInterop/blob/master/samples/D3D11Image/WpfD3D11Interop/MainWindow.xaml.cs) to get a detailed understanding of the code required


Using WPF DirectX Extensions
-------------------
The [documentation](https://github.com/Microsoft/WPFDXInterop/wiki) explains how to install Visual Studio, add the WPF DirectX Extension NuGet package to your project, and get started using the API.

Building WPF DirectX Extensions from Source
------------------------------
**What You Need**

 - [Visual Studio 2015](https://www.visualstudio.com/features/wpf-vs)
 - [DirectX SDK](http://www.microsoft.com/en-us/download/details.aspx?id=6812)
 - [Windows SDK](https://dev.windows.com/en-us/downloads/windows-10-sdk)
 
**Build and Create WPF DirectX Extensions NuGet**
 
 - [Clone the Repository](https://github.com/Microsoft/WPFDXInterop)
 - Open Microsoft.Wpf.Interop.DirectX_winsdk or Microsoft.Wpf.Interop.DirectX_dxsdk solution from [Source](/src)  in Visual Studio
 - Change Build Configuration to Release and build for x86 and x64
 - Run BuildNuGetPackage in [scripts](/scripts) to create nuget packages 

