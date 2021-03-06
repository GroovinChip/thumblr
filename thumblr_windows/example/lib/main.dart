import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _position = ValueNotifier<double>(0.0);
  Thumbnail? _thumbnail;

  @override
  void initState() {
    super.initState();
    getThumbnail();
  }

  void _onSeekPosition(double value) {
    _position.value = value;
  }

  void _onSeekEnd(double value) {
    _position.value = value;
    getThumbnail();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> getThumbnail() async {
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    Thumbnail? result;
    try {
      result = await ThumblrPlatform.instance.generateThumbnail(
        filePath: 'C:\\Users\\groovinchip\\Videos\\gallery nav controls.mp4',
        position: _position.value,
      );
    } on PlatformException catch (e) {
      debugPrint('Failed to generate thumbnail: ${e.message}');
    } catch (e) {
      debugPrint('Failed to generate thumbnail: ${e.toString()}');
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (mounted) {
      setState(() => _thumbnail = result);
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      home: Material(
        child: Column(
          children: [
            Expanded(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Center(
                  child: _thumbnail?.image != null
                      ? RawImage(image: _thumbnail!.image)
                      : const SizedBox.shrink(),
                ),
              ),
            ),
            ValueListenableBuilder(
              valueListenable: _position,
              builder: (BuildContext context, double value, Widget? child) {
                return Slider(
                  value: value,
                  onChanged: _onSeekPosition,
                  onChangeEnd: _onSeekEnd,
                );
              },
            ),
          ],
        ),
      ),
    );
  }
}
