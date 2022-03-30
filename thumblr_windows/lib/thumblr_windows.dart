
import 'dart:async';

import 'package:flutter/services.dart';

class ThumblrWindows {
  static const MethodChannel _channel = MethodChannel('thumblr_windows');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
