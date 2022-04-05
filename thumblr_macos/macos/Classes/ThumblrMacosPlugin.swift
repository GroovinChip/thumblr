import Cocoa
import FlutterMacOS
import AVFoundation

public class ThumblrMacosPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "dev.groovinchip.thumblr_macos", binaryMessenger: registrar.messenger)
    let instance = ThumblrMacosPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "generateThumbnail":
      if let arguments = call.arguments as? Dictionary<String, Any>,
        let path = arguments["filePath"] as? String,
        let position = arguments["position"] as? Int64 {
          do {
            let pathURL = URL(fileURLWithPath: path)
            let asset = AVURLAsset(url: pathURL, options: nil)
            let generator = AVAssetImageGenerator(asset: asset)
            generator.appliesPreferredTrackTransform = true
            let cgImage = try generator.copyCGImage(at: CMTimeMake(value: position, timescale: 1), actualTime: nil)
            let image = NSImage(cgImage: cgImage, size: NSSize(width: cgImage.width, height: cgImage.height))
            let data = image.tiffRepresentation!
            let base64String = data.base64EncodedString()
            let resultMap = ["data": base64String]
            result(resultMap)
          } catch let error {
            print("*** Error generating thumbnail: \(error.localizedDescription)")
          }
      }

    default:
      result(FlutterMethodNotImplemented)
    }
  }
}
