# **WPF DirectX Extensions**
WPF DirectX Extensions allow you to easily host DirectX 10 and DirectX 11 content in WPF applications.

Getting Started
-------------------
 **Where to get it**

 - NuGet package
 - [Source Code](https://github.com/Microsoft/WPFDXInterop)

**Resources**

 - [Documentation](https://github.com/Microsoft/WPFDXInterop/wiki)
 - [Samples](/samples)

**More Info**

 - [Report a bug or ask a question](https://github.com/Microsoft/WPFDXInterop/issues)
 - [License](http://opensource.org/licenses/MIT)
 - [FAQ](https://github.com/Microsoft/WPFDXInterop/blob/master/FAQ.md)

Code Example
------------
//TODO

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

