#include "include/thumblr_windows/thumblr_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// Media Foundation 
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <propvarutil.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <string>
#include <stdio.h>

namespace
{
  template <class T> void SafeRelease(T** ppT)
  {
    if (*ppT) {
      (*ppT)->Release();
      *ppT = NULL;
    }
  }

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

  ThumblrWindowsPlugin::ThumblrWindowsPlugin() {
  
      // Initialize Media Foundation.
      HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
      if (SUCCEEDED(hr)) {
          hr = MFStartup(MF_VERSION);
      }
  }

  ThumblrWindowsPlugin::~ThumblrWindowsPlugin() {
      MFShutdown();
      CoUninitialize();
  }

  HRESULT CanSeekMedia(IMFSourceReader* pReader, BOOL* pbCanSeek)
  {
      HRESULT hr = S_OK;
      ULONG flags = 0;
      PROPVARIANT var;
      PropVariantInit(&var);

      if (pReader == NULL) {
          return MF_E_NOT_INITIALIZED;
      }

      *pbCanSeek = FALSE;
      hr = pReader->GetPresentationAttribute((DWORD)MF_SOURCE_READER_MEDIASOURCE,
          MF_SOURCE_READER_MEDIASOURCE_CHARACTERISTICS, &var);
      if (SUCCEEDED(hr)) {
          hr = PropVariantToUInt32(var, &flags);
      }
      if (SUCCEEDED(hr)) {
          // If the source has slow seeking, we will treat it as
          // not supporting seeking. 
          if ((flags & MFMEDIASOURCE_CAN_SEEK) && !(flags & MFMEDIASOURCE_HAS_SLOW_SEEK)) {
              *pbCanSeek = TRUE;
          }
      }

      return hr;
  }

  HRESULT GetMediaDuration(IMFSourceReader* pReader, LONGLONG* pDuration)
  {
      PROPVARIANT var;
      PropVariantInit(&var);

      HRESULT hr = S_OK;
      if (pReader == NULL) {
          return MF_E_NOT_INITIALIZED;
      }

      hr = pReader->GetPresentationAttribute((DWORD)MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var);
      if (SUCCEEDED(hr)) {
          assert(var.vt == VT_UI8);
          *pDuration = var.hVal.QuadPart;
      }

      return hr;
  }

  HRESULT GetThumbnail(std::wstring file, std::vector<unsigned char> &data, 
      unsigned int *width, unsigned int *height, LONGLONG* duration, double position)
  {
      HRESULT hr = S_OK;
      IMFAttributes* pAttributes = NULL;
      IMFSourceReader* m_pReader = NULL;
      IMFMediaType* pType = NULL;
      GUID subtype = { 0 };
      BOOL bCanSeek = FALSE;
      IMFSample* pSample = NULL;
      DWORD dwFlags = 0;
      IMFMediaBuffer* pBuffer = NULL;
      BYTE* pBitmapData = NULL;    // Bitmap data
      DWORD cbBitmapData = 0;      // Size of data, in bytes
      
      // Configure the source reader to perform video processing.
      //
      // This includes:
      //   - YUV to RGB-32
      //   - Software deinterlace
      hr = MFCreateAttributes(&pAttributes, 1);
      if (SUCCEEDED(hr)){
          hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
      }
      
      // Create the source reader from the URL.
      if (SUCCEEDED(hr)){
          hr = MFCreateSourceReaderFromURL(file.c_str(), pAttributes, &m_pReader);
      }
      
      // Configure the source reader to give us progressive RGB32 frames.
      // The source reader will load the decoder if needed.
      hr = MFCreateMediaType(&pType);
      if (SUCCEEDED(hr)) {
          hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
      }
      if (SUCCEEDED(hr)) {
          hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
      }
      if (SUCCEEDED(hr)) {
          hr = m_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pType);
      }
      SafeRelease(&pType);

      // Ensure the stream is selected.
      if (SUCCEEDED(hr)) {
          hr = m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
      }
      
      // Get the media type from the stream.
      hr = m_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pType);
      if (FAILED(hr)) { goto done; }

      // Make sure it is a video format.
      hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);
      if (subtype != MFVideoFormat_RGB32) {
          hr = E_UNEXPECTED;
          goto done;
      }

      // Get the width and height
      hr = MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, width, height);
      if (FAILED(hr)) { goto done; }

      // Can we seek the media?
      hr = CanSeekMedia(m_pReader, &bCanSeek);
      if (FAILED(hr)) { goto done; }

      // Determine the duration of the media
      //LONGLONG duration;
      hr = GetMediaDuration(m_pReader, duration);
      if (FAILED(hr)) { goto done; }
      LONGLONG offset = (LONGLONG)(*duration * position);

      // Seek to position
      if (bCanSeek && (offset > 0))  {
          PROPVARIANT var;
          PropVariantInit(&var);
          var.vt = VT_I8;
          var.hVal.QuadPart = offset;
          hr = m_pReader->SetCurrentPosition(GUID_NULL, var);
          if (FAILED(hr)) { goto done; }
      }

      hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, &dwFlags, NULL, &pSample);
      if (FAILED(hr)) { goto done; }

      hr = pSample->ConvertToContiguousBuffer(&pBuffer);
      if (FAILED(hr)) { goto done; }

      hr = pBuffer->Lock(&pBitmapData, NULL, &cbBitmapData);

      if (FAILED(hr)) { goto done; }

      data.resize(cbBitmapData);
      std::memcpy(&data[0], pBitmapData, cbBitmapData);
      
done:
      if (pBitmapData && pBuffer)  {
          pBuffer->Unlock();
      }
      SafeRelease(&pBuffer);
      SafeRelease(&pSample);
      SafeRelease(&pType);
      SafeRelease(&m_pReader);

      return hr;
  }

  std::wstring ConvertAnsiToWide(const std::string& str)
  {
      int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0);
      std::wstring wstr(count, 0);
      MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &wstr[0], count);
      return wstr;
  }

  void ThumblrWindowsPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("generateThumbnail") == 0) {
      const auto* arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
      auto* path = std::get_if<std::string>(&(arguments->find(flutter::EncodableValue("filePath"))->second));
      auto* position = std::get_if<double>(&(arguments->find(flutter::EncodableValue("position"))->second));
      if (!arguments || !path || !position) {
        result->Error("invalid_args", "Expected map with filePath and position.");
        return;
      }
      
      std::wstring wstr = ConvertAnsiToWide(*path);
      std::vector<unsigned char> pixels = std::vector<unsigned char>();
      unsigned int width = 0, height = 0, depth = 32;
      LONGLONG duration;
      HRESULT hr = GetThumbnail(wstr, pixels, &width, &height, &duration, *position);
      if (FAILED(hr)) {
        std::ostringstream out;
        out << "failed width error: 0x" << std::uppercase << 
            std::setfill('0') << std::setw(8) << std::hex << (int)hr;
        result->Error("failed", out.str());
      }
      else {
        result->Success(flutter::EncodableMap{
          { "width" , static_cast<int64_t>(width)  },
          { "height", static_cast<int64_t>(height) },
          { "depth" , static_cast<int64_t>(depth)  },
          { "videoDuration" , static_cast<int64_t>(duration)  },
          { "data"  , pixels },
        });
      }
    } else {
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
