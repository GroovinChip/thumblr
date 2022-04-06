cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "thumblr_linux")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "thumblr_linux_plugin")

add_subdirectory(FFmpeg)

# find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
# find_library(AVCODEC_LIBRARY avcodec)

# find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
# find_library(AVFORMAT_LIBRARY avformat)

# find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
# find_library(AVUTIL_LIBRARY avutil)

# find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
# find_library(AVDEVICE_LIBRARY avdevice)

# find_path(AVDEVICE_INCLUDE_DIR libswscale/swscale.h)
# find_library(AVDEVICE_LIBRARY avdevice)

add_library(${PLUGIN_NAME} SHARED
  "thumblr_linux_plugin.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK FFmpgeg)

# List of absolute paths to libraries that should be bundled with the plugin
set(thumblr_linux_bundled_libraries
  ""
  PARENT_SCOPE
)
