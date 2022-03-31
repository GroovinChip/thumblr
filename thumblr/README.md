# thumblr
A federated Flutter plugin for generating video thumbnails on desktop platforms

## Supported platforms
- [x] macOS
- [ ] Windows
- [ ] Linux

## Usage

### macOS Setup
You will need to add an entitlement for either read-only access:
```
<key>com.apple.security.files.user-selected.read-only</key>
<true/>
```
or read-write access:
```
<key>com.apple.security.files.user-selected.read-write</key>
<true/>
```
depending on your use case. It may also be necessary to turn off the `app-sandbox` entitlement.

### Examples:

#### Generate a thumbnail for a local video file
```dart
final thumbnailBytes = await thumblr.generateThumbnail(
  filePath: 'path/to/video.mp4',
);

...

Center(
  child: Image.memory(
    thumbnailBytes,
    height: 350,
    width: 350,
  ),
);
```