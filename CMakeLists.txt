###############
# (C) 2014 by Matteo Pasotti <matteo.pasotti@gmail.com>
# License: GPLv3
###############
SET( VERSION_MAJOR "1")
SET( VERSION_MINOR "3" )
SET( VERSION_PATCH "0" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${GIT_SHA1_VERSION}" )

cmake_minimum_required(VERSION 2.8.11)

project(yainstall)

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
message( STATUS ${CMAKE_MODULE_PATH} )

# Debug, RelWithDebInfo, Release
# set(CMAKE_BUILD_TYPE Debug)

# Find includes in corresponding build directories
# usefull for MOC
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtCore library - Not Really Needed as Qt5Widgets already requires it!
find_package(Qt5Core REQUIRED)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
# Find the Qt5Network library
find_package(Qt5Network REQUIRED)
# Find the Qt5WebKitWidgets library
find_package(Qt5WebKitWidgets REQUIRED)
# Find the Qt5LinguistTools library (libqt5help-devel)
find_package(Qt5LinguistTools REQUIRED)

find_package( ZLIB REQUIRED )
find_package( LibZip REQUIRED )

if( LIBZIP_FOUND )
    # include zip.h
    include_directories( ${LIBZIP_INCLUDE_DIR} )
    # include zipconf.h
    include_directories( ${LIBZIP_CONF_INCLUDE_DIR} )
else( LIBZIP_FOUND )
    message( FATAL_ERROR "LibZip not found" )
endif( LIBZIP_FOUND )

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
# NOTE: using qt5_use_modules makes include_directories droppable
# include_directories(${Qt5Widgets_INCLUDE_DIRS})
# include_directories(${Qt5Network_INCLUDE_DIRS})
# include_directories(${Qt5WebKitWidgets_INCLUDE_DIRS})

# DETECT YALIB ON WIN32
if(WIN32)
    find_path(YALIB_INCLUDE_DIR NAMES yalib.h
              HINTS ${PC_YALIB_INCLUDEDIR} ${PC_YALIB_INCLUDE_DIRS}
              PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../yalib/src/ )

    find_library(YALIB_LIBRARY NAMES libyalib yalib
                PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../yalib-build/src/
                HINTS ${PC_YALIB_LIBDIR} ${PC_YALIB_LIBRARY_DIRS} )

    set(YALIB_LIBRARIES ${YALIB_LIBRARY} )
    set(YALIB_INCLUDE_DIRS ${YALIB_INCLUDE_DIR} )

    message(STATUS "Found YaLib library at: ${YALIB_LIBRARIES}")
    include_directories( ${YALIB_INCLUDE_DIRS} )
    message( STATUS "Include YaLib: ${YALIB_INCLUDE_DIRS}" )
endif(WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++0x")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message( STATUS "Building on MacOSX" )
    set( MACOSX ON )
else()
    unset( MACOSX )
endif()

add_definitions(-DVERSION=${VERSION})

file(GLOB YI_SOURCES *.cpp)
file(GLOB YI_HEADERS *.h)
file(GLOB YI_RESOURCES *.qrc)
file(GLOB YI_FORMS *.ui)
file(GLOB TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/languages/*.ts)
file(GLOB CSS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/css/*.css)
file(GLOB JS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/js/*.js)

QT5_WRAP_UI(UIS_HDRS ${YI_FORMS})

qt5_add_resources(RC_ADDED ${YI_RESOURCES})

qt5_add_translation( QM_FILES ${TS_FILES} )

add_executable(yainstall ${YI_SOURCES} ${UIS_HDRS} ${RC_ADDED} ${QM_FILES})

qt5_use_modules( yainstall Widgets LinguistTools Network WebKitWidgets)

target_link_libraries( yainstall ${LIBZIP_LIBRARY} )

if(UNIX)
    # NOTE: using qt5_use_modules target_link_libraries can refer just to the other libraries
    # target_link_libraries(yainstall yalib Qt5::Widgets Qt5::Network Qt5::WebKitWidgets z)
    if(MACOSX)
        include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../yalib-build/src")
        message( STATUS "Including ${CMAKE_CURRENT_SOURCE_DIR}/../yalib-build/src" )
        #Determine Yalib library
        FIND_LIBRARY(YALIB_LIBRARY NAMES yalib
             PATH_SUFFIXES lib
             PATHS $CMAKE_CURRENT_SOURCE_DIR/../yalib-build/src                 # Extension to localize MinGW root
             DOC "The file that corresponds to the base il library."
        )
        if(YALIB_LIBRARY)
            message(STATUS "Found YaLib library at: ${YALIB_LIBRARY}")
            target_link_libraries(yainstall ${YALIB_LIBRARY} z)
        else(YALIB_LIBRARY)
            message( STATUS "YaLib Not Found")
        endif(YALIB_LIBRARY)
    else(MACOSX)
        target_link_libraries(yainstall yalib z)
    endif()
elseif(WIN32)
    if(YALIB_LIBRARIES)
        target_link_libraries(yainstall ${YALIB_LIBRARIES} z)
    else(YALIB_LIBRARIES)
        message( FATAL_ERROR "YaLib Not Found")
    endif(YALIB_LIBRARIES)
endif()

# setup version and soversion
set_target_properties( yainstall PROPERTIES VERSION ${VERSION})

install( TARGETS yainstall DESTINATION bin )
install( FILES ${QM_FILES} DESTINATION share/yainstall/languages )
install( FILES ${CSS_FILES} DESTINATION share/yainstall/css )
install( FILES ${JS_FILES} DESTINATION share/yainstall/js )
