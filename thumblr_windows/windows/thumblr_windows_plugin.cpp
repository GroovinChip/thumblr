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
    size.cx = 1280;
    size.cy = 720;

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

  std::vector<unsigned char> ToPixels(HBITMAP hBitmap, BITMAPINFOHEADER *bmi)
  {
    BITMAP bmp;

    HDC hDC = CreateCompatibleDC(NULL);
    HGDIOBJ oldBitmap = SelectObject(hDC, hBitmap);
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    SelectObject(hDC, oldBitmap);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = -bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    size_t uSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    std::vector<unsigned char> uData = std::vector<unsigned char>(uSize);
    uData.resize(uSize);

    GetDIBits(hDC, hBitmap, 0, bi.biHeight, &uData[0], (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    std::memcpy(bmi, &bi, bi.biSize);

    DeleteDC(hDC);

    return uData;
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
      BITMAPINFOHEADER bmInfo = { 0 };
      std::vector<unsigned char> pixels = ToPixels(hBitmap, &bmInfo);
      DeleteObject(hBitmap);
      
      result->Success(flutter::EncodableMap{
        { "width" , bmInfo.biWidth },
        { "height", -bmInfo.biHeight },
        { "depth" , bmInfo.biBitCount },
        { "data"  , pixels },
      });


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
