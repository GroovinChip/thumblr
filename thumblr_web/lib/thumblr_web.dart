import 'dart:async';
import 'dart:convert';
import 'dart:html';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart' show visibleForTesting;
import 'package:flutter/services.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

const MethodChannel _channel = MethodChannel('dev.groovinchip.thumblr_web');

class ThumblrWeb extends ThumblrPlatform {
  /// The MethodChannel that is being used by this implementation of the plugin.
  @visibleForTesting
  MethodChannel get channel => _channel;

  /// Registers the web implementation.
  static void registerWith(Registrar registrar) {
    ThumblrPlatform.instance = ThumblrWeb();
  }

  @override
  Future<Thumbnail> generateThumbnail({
    required String filePath,
    double position = 0.0,
  }) async {
    final VideoElement video = VideoElement();
    video.src = filePath;
    video.currentTime = position;
    final CanvasElement canvas = CanvasElement(
      height: 300,
      width: 300,
    );
    CanvasRenderingContext2D ctx = canvas.context2D;
    ctx.drawImage(video, 300, 300);
    final imageData = ctx.canvas.toDataUrl();
    final thumbnailData = base64Decode(imageData.split(',').last);
    final completer = Completer<ui.Image>();
    ui.decodeImageFromList(thumbnailData, completer.complete);
    final image = await completer.future;
    return Thumbnail(image: image);
  }
}
