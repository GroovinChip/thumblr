cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "thumblr_windows")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "thumblr_windows_plugin")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(MediaFoundation REQUIRED COMPONENTS MFCreateAttributes MFCreateMediaType MFCreateSourceReaderFromURL)
add_library(${PLUGIN_NAME} SHARED
  "thumblr_windows_plugin.cpp"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME}
  PRIVATE
  flutter
  flutter_wrapper_plugin
  ${MediaFoundation_LIBRARIES}
  propsys.lib
  D2d1.lib
)

# List of absolute paths to libraries that should be bundled with the plugin
set(thumblr_windows_bundled_libraries
  ""
  PARENT_SCOPE
)
