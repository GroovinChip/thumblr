import 'dart:async';
import 'dart:convert';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart' show visibleForTesting;
import 'package:flutter/services.dart';
import 'package:thumblr_platform_interface/src/platform_interface/thumblr_platform_interface.dart';
import 'package:thumblr_platform_interface/src/platform_interface/thumbnail.dart';

const MethodChannel _channel = MethodChannel('dev.groovinchip.thumblr');

/// An implementation of [ThumblrPlatform] that uses method channels.
class MethodChannelThumblr extends ThumblrPlatform {
  /// The MethodChannel that is being used by this implementation of the plugin.
  @visibleForTesting
  MethodChannel get channel => _channel;

  @override
  Future<Thumbnail> generateThumbnail({
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
    final videoLength = result['videoLength'];
    final imageData = base64.decode(result['data']);
    final completer = Completer<ui.Image>();
    ui.decodeImageFromList(imageData, completer.complete);
    final image = await completer.future;
    return Thumbnail(
      image: image,
      videoDuration: videoLength,
    );
  }
}
