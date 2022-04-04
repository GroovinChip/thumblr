import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:thumblr_windows/thumblr_windows.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  final ThumblrWindows plugin = ThumblrWindows();

  final List<MethodCall> log = <MethodCall>[];

  setUp(() {
    plugin.channel.setMockMethodCallHandler((MethodCall methodCall) async {
      log.add(methodCall);
    });
    log.clear();
  });

  test('Registered instance', () {
    ThumblrWindows.registerWith();
    expect(ThumblrPlatform.instance, isA<ThumblrWindows>());
  });

  tearDown(() {
    plugin.channel.setMockMethodCallHandler(null);
    log.clear();
  });
}
