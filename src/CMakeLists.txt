CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(FONTRENDERER CXX)

FIND_PACKAGE(Freetype REQUIRED)

INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})

ADD_LIBRARY(frlib line.cpp primitive.cpp renderer.cpp vector.cpp primitives/bezier.cpp primitives/segment.cpp)


