import 'dart:async';

import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';
export 'package:thumblr_platform_interface/src/platform_interface/thumbnail.dart';

/// Generates a thumbnail for a given video file.
Future<Thumbnail> generateThumbnail({
  required String filePath,
  double position = 0.0,
}) async {
  return ThumblrPlatform.instance.generateThumbnail(
    filePath: filePath,
    position: position,
  );
}
