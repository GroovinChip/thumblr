# thumblr_macos

The macOS implementation of the thumblr plugin.

## Usage

This is an endorsed implementation of the thumblr plugin. All you need to do is depend on `thumblr`
in your `pubspec.yaml`. You can then use the `thumblr` API normally. You should not use
`thumblr_macos` APIs directly.

### Entitlements
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