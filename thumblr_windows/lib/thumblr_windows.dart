import 'dart:async';
import 'dart:typed_data';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart' show debugPrint, visibleForTesting;
import 'package:flutter/services.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

const MethodChannel _channel = MethodChannel('dev.groovinchip.thumblr_windows');

/// An implementation of [ThumblrPlatform] for Windows.
class ThumblrWindows extends ThumblrPlatform {
  /// The MethodChannel that is being used by this implementation of the plugin.
  @visibleForTesting
  MethodChannel get channel => _channel;

  /// Registers the Windows implementation.
  static void registerWith() {
    ThumblrPlatform.instance = ThumblrWindows();
  }

  @override
  Future<ui.Image> generateThumbnail({
    required String filePath,
    required double position,
  }) async {
    final result = (await _channel.invokeMapMethod<String, dynamic>('generateThumbnail', {
      'filePath': filePath,
      'position': position,
    }))!;
    final width = result['width'] as int;
    final height = result['height'] as int;
    final depth = result['depth'] as int;
    final data = result['data'] as Uint8List;
    assert(depth == 32, '$depth bit not supported');
    debugPrint(
        'generateThumbnail returned ${width}x${height}x${depth}bit with ${data.lengthInBytes} bytes');
    final completer = Completer<ui.Image>();
    ui.decodeImageFromPixels(data, width, height, ui.PixelFormat.bgra8888, completer.complete);
    return completer.future;
  }
}
