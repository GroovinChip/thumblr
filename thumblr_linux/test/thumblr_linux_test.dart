import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:thumblr_linux/thumblr_linux.dart';

void main() {
  const MethodChannel channel = MethodChannel('thumblr_linux');

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
    expect(await ThumblrLinux.platformVersion, '42');
  });
}
