import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:thumblr_macos/thumblr_macos.dart';
import 'package:thumblr_platform_interface/thumblr_platform_interface.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  final ThumblrMacOS plugin = ThumblrMacOS();

  final List<MethodCall> log = <MethodCall>[];

  setUp(() {
    plugin.channel.setMockMethodCallHandler((MethodCall methodCall) async {
      log.add(methodCall);
    });
    log.clear();
  });

  test('Registered instance', () {
    ThumblrMacOS.registerWith();
    expect(ThumblrPlatform.instance, isA<ThumblrMacOS>());
  });

  tearDown(() {
    plugin.channel.setMockMethodCallHandler(null);
    log.clear();
  });
}
