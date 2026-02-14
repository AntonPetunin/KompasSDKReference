cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Step4")
set(this_target_name "Step4")

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.07.15_15.50.37/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "Step4.rc"
  "APIDraw.cpp"
  "APIDraw.h"
  "CPropMen.cpp"
  "CPropMen.h"
  "DataBase.cpp"
  "DataBase.h"
  "Doc2DDispatcher.cpp"
  "Doc2DDispatcher.h"
  "Resource.h"
  "SLIDE.RH"
  "Step4.h"
  "Step4.cpp"
  "Step4_pch.h"
  "Step4_res.cpp"
  "Events/Process2DEvent.cpp"
  "Events/Process2DEvent.h"
  "Events/PropertyManagerEvent.cpp"
  "Events/PropertyManagerEvent.h"
  "${SQLITE_PATH}/sqlite3.c"
  "${SQLITE_PATH}/sqlite3.h"
)

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
)

set_target_properties(${this_target} PROPERTIES
  OUTPUT_NAME ${this_target_name}
  SUFFIX ".rtw"
)

target_include_directories(${this_target} PRIVATE
  ${KOMPAS_INCLUDE_DIRECTORIES}
  ${COMMON_INCLUDE_DIRECTORIES}
  ${SQLITE_PATH}
)

target_link_directories(${this_target} PRIVATE
  ${KOMPAS_LINK_DIRECTORIES}
  ${COMMON_LINK_DIRECTORIES}
)

target_compile_definitions(${this_target} PRIVATE
  ${COMMON_PROJECT_MACROS}
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

target_precompile_headers(${this_target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Step4_pch.h>")

copy(${CMAKE_SOURCE_DIR}/../Content/Step4.xml ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Step4.db ${OUTPUT_DIRECTORY})
