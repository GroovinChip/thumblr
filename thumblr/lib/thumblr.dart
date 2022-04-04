import 'dart:async';
import 'dart:ui' as ui;

import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

/// Generates a thumbnail for a given video file.
Future<ui.Image> generateThumbnail({required String filePath}) {
  return ThumblrPlatform.instance.generateThumbnail(filePath: filePath);
}
