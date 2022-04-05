import 'dart:ui' as ui;

import 'package:plugin_platform_interface/plugin_platform_interface.dart';
import 'package:thumblr_platform_interface/src/method_channel/method_channel_thumblr.dart';

/// {@template thumblrPlatform}
/// The interface that implementations of `thumblr` must implement.
///
/// Platform implementations should extend this class rather than implement
/// it as `thumblr`. Extending this class (using `extends`) ensures that the
/// subclass will get the default implementation, while platform
/// implementations that `implements` this interface will be broken by
/// newly added [ThumblrPlatform] methods.
/// {@endtemplate}
abstract class ThumblrPlatform extends PlatformInterface {
  /// {@macro thumblrPlatform}
  ThumblrPlatform() : super(token: _token);

  static final Object _token = Object();

  /// The default instance of [ThumblrPlatform] to use.
  ///
  /// Defaults to [MethodChannelThumblr].
  static ThumblrPlatform _instance = MethodChannelThumblr();

  /// The default instance of [ThumblrPlatform] to use.
  ///
  /// Defaults to [MethodChannelThumblr].
  static ThumblrPlatform get instance => _instance;

  /// Platform-specific plugins should set this with their own platform-specific
  /// class that extends [ThumblrPlatform] when they register themselves.
  static set instance(ThumblrPlatform instance) {
    PlatformInterface.verify(instance, _token);
    _instance = instance;
  }

  /// Generates a thumbnail for a given video file.
  Future<ui.Image> generateThumbnail({
    required String filePath,
    double position = 0.0,
  }) {
    throw UnimplementedError('generateThumbnail() has not been implemented');
  }
}
