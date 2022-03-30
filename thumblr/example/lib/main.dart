import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:thumblr/thumblr.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  Uint8List? thumbnail;

  @override
  void initState() {
    super.initState();
    getThumbnail();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> getThumbnail() async {
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    Uint8List? _thumb;
    try {
      _thumb = await generateThumbnail(
        filePath:
        '/Users/groovinchip/Documents/stream_chat_flutter desktop feature clips/gallery nav controls.mp4',
      );
    } on PlatformException catch (e) {
      debugPrint('Failed to generate thumbnail: ${e.message}');
    } catch (e) {
      debugPrint('Failed to generate thumbnail: ${e.toString()}');
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() => thumbnail = _thumb);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: thumbnail != null
              ? Image.memory(
            thumbnail!,
            height: 300,
            width: 300,
          )
              : const SizedBox.shrink(),
        ),
      ),
    );
  }
}
