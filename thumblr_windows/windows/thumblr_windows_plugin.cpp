#include "include/thumblr_windows/thumblr_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <shlobj_core.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <string>
#include <stdio.h>

namespace
{

  class ThumblrWindowsPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    ThumblrWindowsPlugin();

    virtual ~ThumblrWindowsPlugin();

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  };

  // static
  void ThumblrWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "dev.groovinchip.thumblr_windows",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<ThumblrWindowsPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  ThumblrWindowsPlugin::ThumblrWindowsPlugin() {}

  ThumblrWindowsPlugin::~ThumblrWindowsPlugin() {}

  std::wstring ConvertAnsiToWide(const std::string &str)
  {
    int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &wstr[0], count);
    return wstr;
  }

  HBITMAP GetThumbnail(std::wstring file)
  {
    std::wstring folder, fileName;
    size_t pos = file.find_last_of(L"\\");
    folder = file.substr(0, pos);
    fileName = file.substr(pos + 1);

    IShellFolder *pDesktop = NULL;
    IShellFolder *pSub = NULL;
    IExtractImage *pIExtract = NULL;
    LPITEMIDLIST pidl = NULL;

    HRESULT hr;
    hr = SHGetDesktopFolder(&pDesktop);
    if (FAILED(hr))
      return NULL;
    hr = pDesktop->ParseDisplayName(NULL, NULL, (LPWSTR)folder.c_str(), NULL, &pidl, NULL);
    if (FAILED(hr))
      return NULL;
    hr = pDesktop->BindToObject(pidl, NULL, IID_IShellFolder, (void **)&pSub);
    if (FAILED(hr))
      return NULL;
    hr = pSub->ParseDisplayName(NULL, NULL, (LPWSTR)fileName.c_str(), NULL, &pidl, NULL);
    if (FAILED(hr))
      return NULL;
    hr = pSub->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST *)&pidl, IID_IExtractImage, NULL, (void **)&pIExtract);
    if (FAILED(hr))
      return NULL;

    SIZE size;
    size.cx = 300;
    size.cy = 300;

    DWORD dwFlags = IEIFLAG_ORIGSIZE | IEIFLAG_QUALITY;

    HBITMAP hThumbnail = NULL;

    // Set up the options for the image
    OLECHAR pathBuffer[MAX_PATH];
    hr = pIExtract->GetLocation(pathBuffer, MAX_PATH, NULL, &size, 32, &dwFlags);

    // Get the image
    hr = pIExtract->Extract(&hThumbnail);

    pDesktop->Release();
    pSub->Release();

    return hThumbnail;
  }

  std::vector<unsigned char> ToPixels(HBITMAP BitmapHandle)
  {
    BITMAP Bmp = {0};
    BITMAPINFO Info = {0};
    std::vector<unsigned char> Pixels = std::vector<unsigned char>();

    HDC DC = CreateCompatibleDC(NULL);
    std::memset(&Info, 0, sizeof(BITMAPINFO)); // not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
    GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = Bmp.bmWidth;
    Info.bmiHeader.biHeight = Bmp.bmHeight;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = ((Info.bmiHeader.biWidth * Bmp.bmBitsPixel + 31) / 32) * 4 * Info.bmiHeader.biHeight;

    Pixels.resize(Info.bmiHeader.biSizeImage);
    GetDIBits(DC, BitmapHandle, 0, Info.bmiHeader.biHeight, &Pixels[0], &Info, DIB_RGB_COLORS);
    SelectObject(DC, OldBitmap);

    // height = std::abs(height);
    DeleteDC(DC);
    return Pixels;
  }

  void ThumblrWindowsPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("generateThumbnail") == 0)
    {
      const flutter::EncodableValue *arguments = method_call.arguments();
      std::string path = std::get<std::string>(*arguments);

      std::wstring wstr = ConvertAnsiToWide(path);

      // std::wcout << wstr << std::endl;

      HBITMAP hBitmap = GetThumbnail(wstr);
      std::vector<unsigned char> pixels = ToPixels(hBitmap);
      result->Success(flutter::EncodableValue(pixels));
    }
    else
    {
      result->NotImplemented();
    }
  }

} // namespace

void ThumblrWindowsPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  ThumblrWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
