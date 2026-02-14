cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Step10")
set(this_target_name "Step10")

project(${this_target})

set(${this_target}_SOURCE_FILES
  "Step10.rc"
  "Step10.cpp"
  "Step10_res.cpp"
)

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.10.22_16.06.24/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

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

copy(${CMAKE_SOURCE_DIR}/../Content/Step10.xml ${OUTPUT_DIRECTORY})
