cmake_minimum_required(VERSION 3.1...3.15)

# Set cmake to most recent version
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Mallard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(Magnum REQUIRED
  GL
  MeshTools
  Primitives
  Shaders
  Trade
  TgaImporter
  SceneGraph
  Sdl2Application)

find_package(SDL2)

find_package(EnTT CONFIG REQUIRED)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(mallard
  src/Main.cpp
  src/Ground.cpp
  src/Camera.cpp
  )

target_compile_features(mallard PUBLIC cxx_std_17)

target_link_libraries(mallard PRIVATE
    SDL2
    EnTT
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Trade
    Magnum::TgaImporter
    Magnum::SceneGraph
    Magnum::Shaders)

install(TARGETS mallard DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
