cmake_minimum_required(VERSION 3.18)

set(VAMP_SDK_PATH "" CACHE PATH "The path to the Vamp plugin SDK")
if(NOT EXISTS ${VAMP_SDK_PATH})
  message(FATAL_ERROR "The path to the Vamp plugin SDK is not valid: " ${VAMP_PLUGIN_SDK_PATH})
endif()

project(IrcamVampExtension VERSION 0.0.2 LANGUAGES C CXX)

# Host Section

set(VAMP_HOST_SDK_DIRECTORY "${VAMP_SDK_PATH}/vamp-hostsdk")
set(VAMP_HOST_SOURCES_DIRECTORY "${VAMP_SDK_PATH}/src/vamp-hostsdk")

file(GLOB VAMP_HOST_SDK_HEADERS ${VAMP_HOST_SDK_DIRECTORY}/*h)
file(GLOB VAMP_HOST_SDK_SOURCES ${VAMP_HOST_SOURCES_DIRECTORY}/*h  ${VAMP_HOST_SOURCES_DIRECTORY}/*cpp ${VAMP_HOST_SOURCES_DIRECTORY}/*c)
source_group("Vamp/Host" FILES ${VAMP_HOST_SDK_HEADERS} ${VAMP_HOST_SDK_SOURCES})
add_library(vamp_host STATIC ${VAMP_HOST_SDK_HEADERS} ${VAMP_HOST_SDK_SOURCES})
target_compile_definitions(vamp_host PUBLIC _VAMP_NO_HOST_NAMESPACE=1)
target_compile_definitions(vamp_host PUBLIC 
  IVE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  IVE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  IVE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)
if(WIN32)
  target_compile_definitions(vamp_host PRIVATE _USE_MATH_DEFINES)
  target_compile_options(vamp_host PRIVATE /wd4996 /wd4267)
elseif(APPLE)
  set_target_properties(vamp_host PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "./")
  target_compile_options(vamp_host PRIVATE -Wno-conversion)
endif()
target_include_directories(vamp_host PUBLIC ${VAMP_SDK_PATH})

set(IVE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
file(GLOB_RECURSE IVE_SOURCE ${IVE_SOURCE_DIR}/ive.h ${IVE_SOURCE_DIR}/IvePluginWrapper.hpp ${IVE_SOURCE_DIR}/IvePluginWrapper.cpp ${IVE_SOURCE_DIR}/IvePluginExtension.hpp)
source_group("Source" FILES ${IVE_SOURCE})

add_library(ive_host_static STATIC ${IVE_SOURCE})
set_target_properties(ive_host_static PROPERTIES OUTPUT_NAME "ive_host")
set_target_properties(ive_host_static PROPERTIES CXX_STANDARD 20)
set_target_properties(ive_host_static PROPERTIES CXX_STANDARD_REQUIRED ON)
target_include_directories(ive_host_static PUBLIC ${VAMP_SDK_PATH})
target_include_directories(ive_host_static INTERFACE ${IVE_SOURCE_DIR})
target_link_libraries(ive_host_static PUBLIC vamp_host)
target_compile_definitions(ive_host_static PUBLIC IVE_LIB_IMPORT)

# Plugin Section

set(VAMP_PLUGIN_SDK_DIRECTORY "${VAMP_SDK_PATH}/vamp-sdk")
set(VAMP_PLUGIN_SOURCES_DIRECTORY "${VAMP_SDK_PATH}/src/vamp-sdk")

file(GLOB VAMP_PLUGIN_SDK_HEADERS ${VAMP_PLUGIN_SDK_DIRECTORY}/*h)
file(GLOB VAMP_PLUGIN_SDK_SOURCES ${VAMP_PLUGIN_SOURCES_DIRECTORY}/PluginAdapter.cpp ${VAMP_PLUGIN_SOURCES_DIRECTORY}/RealTime.cpp  ${VAMP_PLUGIN_SOURCES_DIRECTORY}/FFT.cpp)
source_group("Vamp/Plugin" FILES ${VAMP_PLUGIN_SOURCES})
add_library(vamp_plugin STATIC ${VAMP_PLUGIN_SDK_HEADERS} ${VAMP_PLUGIN_SDK_SOURCES})
set_target_properties(vamp_plugin PROPERTIES CXX_STANDARD 20)
set_target_properties(vamp_plugin PROPERTIES CXX_STANDARD_REQUIRED ON)
target_compile_definitions(vamp_plugin PUBLIC 
  IVE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  IVE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  IVE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)
if(WIN32)
  target_compile_definitions(vamp_plugin PRIVATE _USE_MATH_DEFINES)
  target_compile_options(vamp_plugin PRIVATE /wd4996 /wd4267)
elseif(APPLE)
  set_target_properties(vamp_plugin PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "./")
  target_compile_options(vamp_plugin PRIVATE -Wno-conversion)
elseif(UNIX)
  set_property(TARGET vamp_plugin PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()
target_include_directories(vamp_plugin PUBLIC ${VAMP_SDK_PATH})

set(IVE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
file(GLOB_RECURSE IVE_SOURCE ${IVE_SOURCE_DIR}/ive.h ${IVE_SOURCE_DIR}/IvePluginAdapter.hpp ${IVE_SOURCE_DIR}/IvePluginAdapter.cpp ${IVE_SOURCE_DIR}/IvePluginExtension.hpp)
source_group("Source" FILES ${IVE_SOURCE})

add_library(ive_plugin_static STATIC ${IVE_SOURCE})
set_target_properties(ive_plugin_static PROPERTIES OUTPUT_NAME "ive_plugin")
set_target_properties(ive_plugin_static PROPERTIES CXX_STANDARD 20)
set_target_properties(ive_plugin_static PROPERTIES CXX_STANDARD_REQUIRED ON)
target_include_directories(ive_plugin_static PUBLIC ${VAMP_SDK_PATH})
target_include_directories(ive_plugin_static INTERFACE ${IVE_SOURCE_DIR})
target_link_libraries(ive_plugin_static PUBLIC vamp_plugin)
target_compile_definitions(ive_plugin_static PUBLIC IVE_LIB_EXPORT)
if(UNIX AND NOT APPLE)
    set_target_properties(ive_plugin_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Helper Section

function(vamp_prepare_plugin_target target)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20)
  set_target_properties(${target} PROPERTIES PREFIX "")
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Release)
  target_link_libraries(${target} PRIVATE vamp_plugin)
  if(WIN32)
    target_compile_definitions(${target} PRIVATE _USE_MATH_DEFINES)
    target_compile_options(${target} PRIVATE /wd4996 /wd4267)
    set_target_properties(${target} PROPERTIES LINK_FLAGS "/export:vampGetPluginDescriptor")
  elseif(UNIX AND NOT APPLE)
    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endfunction(vamp_prepare_plugin_target)

function(ive_prepare_plugin_target target)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20)
  set_target_properties(${target} PROPERTIES PREFIX "")
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
  set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Release)
  target_link_libraries(${target} PRIVATE ive_plugin_static)
  if(WIN32)
    target_compile_definitions(${target} PRIVATE _USE_MATH_DEFINES)
    target_compile_options(${target} PRIVATE /wd4996 /wd4267)
    set_target_properties(${target} PROPERTIES LINK_FLAGS "/export:vampGetPluginDescriptor")
  elseif(UNIX AND NOT APPLE)
    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endfunction(ive_prepare_plugin_target)
