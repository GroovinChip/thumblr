import 'dart:async';

import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';
export 'package:thumblr_platform_interface/src/platform_interface/thumbnail.dart';

/// Generates a thumbnail for a given video file.
///
/// [filePath] is the path to the video file.
/// [position] specifies the time position in seconds at which the thumbnail should be generated.
/// Defaults to `0.0` seconds.
Future<Thumbnail> generateThumbnail({
  required String filePath,
  double position = 0.0,
}) async {
  return ThumblrPlatform.instance.generateThumbnail(
    filePath: filePath,
    position: position,
  );
}
