import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:thumblr_windows/thumblr_windows.dart';

void main() {
  const MethodChannel channel = MethodChannel('thumblr_windows');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await ThumblrWindows.platformVersion, '42');
  });
}
