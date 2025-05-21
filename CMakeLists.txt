cmake_minimum_required(VERSION 4.0.1 FATAL_ERROR)

set(PROJECT_NAME SimpleEngine)
project(SimpleEngine)


add_subdirectory(SimpleEngineCore)
add_subdirectory(SimpleEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleEngineEditor)
