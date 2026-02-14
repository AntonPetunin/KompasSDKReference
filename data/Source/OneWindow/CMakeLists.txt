cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(this_target "OneWindow")
set(this_target_name "OneWindow")

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.11.02_14.14.12/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "OneWindow.rc"
  "OneWindow_pch.h"
  "OneWindow_res.cpp"
  "OneWindow.cpp"
  "OneWindow.h"
  "LibObj.cpp"
  "LibObj.h"
  "GL/KHR/khrplatform.h" 
  "GL/glew.c"
  "GL/gltypes.hpp"
  "GL/glew.h"
  "Events/OneWindowEvents.cpp"
  "Events/OneWindowEvents.h"
  "CPropMen.cpp"
  "CPropMen.h"
  "Resource.h"
)

if (WIN32)

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
	"GL/wglew.h"
   )

else ()

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
	"GL/glxew.h" 
   )

endif()

set_target_properties(${this_target} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME OneWindow
  SUFFIX ".rtw"
  FOLDER "Samples"
)

target_compile_definitions(${this_target} PRIVATE
  ${COMMON_PROJECT_MACROS}
  "GLEW_BUILD"
)

# 3rd party libs
target_link_libraries(${this_target} PRIVATE
)

# Kompas references
target_link_libraries(${this_target} PRIVATE
  "ksAPI"
)

# Project references
target_link_libraries(${this_target} PRIVATE
	
)

target_compile_features(${this_target} PRIVATE
  ${CXX_STD_VERSION}
)

target_include_directories(${this_target} PRIVATE
  ${KOMPAS_INCLUDE_DIRECTORIES}
  ${COMMON_INCLUDE_DIRECTORIES}
  $ENV{KOMPAS_SDK}/lib
  $ENV{KOMPAS_SDK}/Include
)

target_link_directories(${this_target} PRIVATE
  ${KOMPAS_LINK_DIRECTORIES}
  ${COMMON_LINK_DIRECTORIES}
  ${KOMPAS_SDK}/lib64
)

target_compile_options(${this_target} PRIVATE
  ${DEBUG_INFORMATION_FORMAT}
  ${EXCEPTION_HANDLING_MODEL}
  ${COMMON_COMPILE_OPTIONS}
  ${WARNINGS_LEVEL}
  ${WARNINGS_AS_ERRORS}
  ${ADDITIONAL_COMPILE_OPTIONS_CXX}
)

target_link_options(${this_target} PRIVATE
  ${SUBSYSTEM}
)

#target_precompile_headers(${this_target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:OneWindow_pch.h>")

copy(${CMAKE_SOURCE_DIR}/../Content/OneWindow.xml ${OUTPUT_DIRECTORY})
