import 'dart:async';
import 'dart:convert';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart' show visibleForTesting;
import 'package:flutter/services.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

const MethodChannel _channel = MethodChannel('dev.groovinchip.thumblr_macos');

/// An implementation of [ThumblrPlatform] for macOS.
class ThumblrMacOS extends ThumblrPlatform {
  /// The MethodChannel that is being used by this implementation of the plugin.
  @visibleForTesting
  MethodChannel get channel => _channel;

  /// Registers the macOS implementation.
  static void registerWith() {
    ThumblrPlatform.instance = ThumblrMacOS();
  }

  @override
  Future<ui.Image> generateThumbnail({required String filePath}) async {
    final base64Thumbnail =
        await _channel.invokeMethod('generateThumbnail', filePath);
    final completer = Completer<ui.Image>();
    ui.decodeImageFromList(base64Decode(base64Thumbnail), completer.complete);
    return completer.future;
  }
}
