# thumblr_platform_interface

A common platform interface for the `thumblr` plugin.

This interface allows platform-specific implementations of the `thumblr` plugin, 
as well as the plugin itself, to ensure they are supporting the same interface.

## Usage

To implement a new platform-specific implementation of `thumblr`, 
extend `ThumblrPlatform` with an implementation that performs the platform-specific 
behavior, and when you register your plugin, set the default `ThumblrPlatform` by 
calling `ThumblrPlatform.instance = MyPlatformThumblr()`.