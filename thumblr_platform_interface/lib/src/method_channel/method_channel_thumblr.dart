import 'dart:async';
import 'dart:convert';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart' show visibleForTesting;
import 'package:flutter/services.dart';
import 'package:thumblr_platform_interface/src/platform_interface/thumblr_platform_interface.dart';

const MethodChannel _channel = MethodChannel('dev.groovinchip.thumblr');

/// An implementation of [ThumblrPlatform] that uses method channels.
class MethodChannelThumblr extends ThumblrPlatform {
  /// The MethodChannel that is being used by this implementation of the plugin.
  @visibleForTesting
  MethodChannel get channel => _channel;

  @override
  Future<ui.Image> generateThumbnail({
    required String filePath,
    double position = 0.0,
  }) async {
    final base64Thumbnail = await _channel.invokeMethod('generateThumbnail', filePath);
    final imageData = base64.decode(base64Thumbnail);
    final completer = Completer<ui.Image>();
    ui.decodeImageFromList(imageData, completer.complete);
    return completer.future;
  }
}
