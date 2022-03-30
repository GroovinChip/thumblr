import 'dart:convert';
import 'dart:typed_data';

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
  Future<Uint8List> generateThumbnail({required String filePath}) async {
    final base64Thumbnail =
        await _channel.invokeMethod('generateThumbnail', filePath);
    return base64Decode(base64Thumbnail);
  }
}