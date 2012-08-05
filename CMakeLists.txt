CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(FONTRENDERER CXX)

SET(PACKAGE_STRING fontrenderer)
SET(PACKAGE_BUGREPORT font@kaaser.at)
SET(PACKAGE_VERSION 1.0)

SET(libs frlib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Find and include FreeType
FIND_PACKAGE(Freetype REQUIRED)
IF(FREETYPE_FOUND)
	SET(libs ${libs} freetype)
ENDIF(FREETYPE_FOUND)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})

# Find and include ICU
find_path(ICU_INCLUDE_DIR NAMES unicode/unistr.h)
mark_as_advanced(ICU_INCLUDE_DIR)

find_library(ICU_LIBRARY NAMES icuuc cygicuuc cygicuuc32)
mark_as_advanced(ICU_LIBRARY)

if(ICU_INCLUDE_DIR AND ICU_LIBRARY)
	SET(ICU_FOUND 1)
	SET(libs ${libs} ${ICU_LIBRARY})
	ADD_DEFINITIONS(-DUSE_UTF8)
	INCLUDE_DIRECTORIES(${ICU_INCLUDE_DIR})

	MESSAGE(STATUS "Found ICU header files in ${ICU_INCLUDE_DIR}")
	MESSAGE(STATUS "Found ICU libraries: ${ICU_LIBRARY}")
	MESSAGE(STATUS "Enabling UTF8 support.")
ELSE()
	MESSAGE( STATUS "ICU was not found: Disabling UTF8 support.")
ENDIF()


ADD_SUBDIRECTORY(src)
ADD_EXECUTABLE(fontrenderer src/fontrenderer.cpp)

TARGET_LINK_LIBRARIES(fontrenderer frlib ${libs})
