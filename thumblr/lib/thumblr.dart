import 'dart:async';
import 'dart:typed_data';

import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

/// Generates a thumbnail for a given video file.
Future<Uint8List> generateThumbnail({required String filePath}) {
  return ThumblrPlatform.instance.generateThumbnail(filePath: filePath);
}
