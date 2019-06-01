cmake_minimum_required(VERSION 3.4.1)


function(find_include_dirs out)
    file(GLOB_RECURSE headers ${ARGN})
    foreach (path ${headers})
        get_filename_component(dir ${path} PATH)
        list(APPEND include_dirs ${dir})
    endforeach ()
    list(REMOVE_DUPLICATES include_dirs)
    set(${out} ${include_dirs} PARENT_SCOPE)
endfunction()

find_include_dirs(skia_headers ../../../../vendor/skia/include/*.h)
find_include_dirs(freetype_headers ../../../../vendor/freetype/include/*.h)
include_directories(${skia_headers} ${freetype_headers} ../../../../include ../../../../src)

file(GLOB_RECURSE SOURCE_FILES ../../../../src/*.cpp ../../../../src/*.h ../../../../include/*.h src/*.cpp src/*.h)

function(remove_srcs)
    file(GLOB_RECURSE to_remove ${ARGN})
    if (NOT "${to_remove}" STREQUAL "")
        list(REMOVE_ITEM SOURCE_FILES ${to_remove})
        set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
    endif ()
endfunction()

remove_srcs(../../../../src/platform/ios/*.*)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        skiatext

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SOURCE_FILES} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib  log)
find_library(GLESv2-lib  GLESv2)
find_library(EGL-lib EGL)
find_library(android-lib android)

file(GLOB_RECURSE skia_lib ../../../../vendor/skia/android/*.a)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        skiatext
        ${skia_lib}
        ${GLESv2-lib}
        ${EGL-lib}
        ${android-lib}
        ${log-lib} )
