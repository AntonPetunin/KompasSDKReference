cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(this_target "IPropertyTree_Sample")
set(this_target_name "IPropertyTree_Sample")

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.09.10_19.31.21/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "IPropertyTree_Sample.rc"
  "BaseEvent.cpp"
  "BaseEvent.h"
  "IPropertyTree_res.cpp"
  "IPropertyTree_Sample.cpp"
  "IPropertyTree_Sample.h"
  "PropMan.cpp"
  "PropMan.h"
  "PropUserEvent.cpp"
  "PropUserEvent.h"
  "Resource.h"
)

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
  )

set_target_properties(${this_target} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME IPropertyTree_Sample
  SUFFIX ".rtw"
  FOLDER "Samples"
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


target_include_directories(${this_target} PRIVATE
  ${KOMPAS_INCLUDE_DIRECTORIES}
  ${COMMON_INCLUDE_DIRECTORIES}
)

target_link_directories(${this_target} PRIVATE
  ${KOMPAS_LINK_DIRECTORIES}
  ${COMMON_LINK_DIRECTORIES}
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

copy(${CMAKE_SOURCE_DIR}/../Content/IPropertyTree_Sample.xml ${OUTPUT_DIRECTORY})