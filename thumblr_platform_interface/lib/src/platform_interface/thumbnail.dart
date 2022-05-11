import 'dart:ui' as ui;

/// A thumbnail generated from a video fileS.
///
/// Contains relevant information about the thumbnail, such as its dimensions
/// and duration.
class Thumbnail {
  const Thumbnail({
    required this.image,
    this.videoDuration,
    this.height,
    this.width,
    this.depth,
  });

  /// The thumbnail image.
  final ui.Image image;

  /// The duration of the video.
  final double? videoDuration;

  /// The height of the thumbnail [image].
  final int? height;

  /// The width of the thumbnail [image].
  final int? width;

  /// The depth of the thumbnail [image].
  final int? depth;
}
