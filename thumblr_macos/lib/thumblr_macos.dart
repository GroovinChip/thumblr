import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
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
  Future<ui.Image> generateThumbnail({
    required String filePath,
    double position = 0.0,
  }) async {
    final result = (await _channel.invokeMapMethod<String, dynamic>(
      'generateThumbnail',
      {
        'filePath': filePath,
        'position': position.round(),
      },
    ))!;
    final data = base64Decode(result['data']);
    final completer = Completer<ui.Image>();
    ui.decodeImageFromList(data, completer.complete);
    return completer.future;
  }
}
