cmake_minimum_required(VERSION 4.0.1 FATAL_ERROR)

set(CORE_PROJECT_NAME SimpleEngineCore)

add_library(${CORE_PROJECT_NAME} STATIC
	src/SimpleEngineCore/Utils/test.cpp
	includes/SimpleEngineCore/Utils/test.hpp
)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_23)
