import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';
import 'dart:ui';

import 'package:flutter/foundation.dart' show visibleForTesting;
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
  Future<Uint8List> generateThumbnail({required String filePath}) async {
    final thumbnailBytes =
        await _channel.invokeMethod('generateThumbnail', filePath);
    return Uint8List.fromList(thumbnailBytes);
  }
}
