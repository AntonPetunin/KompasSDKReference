cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Step4_KsAPI_2D")
set(this_target_name "Step4_KsAPI_2D")

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.09.19_19.43.21/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "Step4_KsAPI_2D.rc"
  "Resource.h"
  "Step4_KsAPI_2D.cpp"
  "Step4_KsAPI_2D.h"
  "Step4_KsAPI_2D_pch.h"
  "Step4_KsAPI_2D_res.cpp"
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

target_precompile_headers(${this_target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Step4_KsAPI_2D_pch.h>")

copy(${CMAKE_SOURCE_DIR}/../Content/Step4_KsAPI_2D.xml ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Конфигурация.pdc ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Конфигурация1.pdc ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Конфигурация2.pdc ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Конфигурация3.pdc ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Чертеж.cdw ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Чертеж2.cdw ${OUTPUT_DIRECTORY})
