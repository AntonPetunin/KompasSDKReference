cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Studs3D1")
set(this_target_name "Studs3D1")

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2024.06.28_14.06.20/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  Studs3D.rc
  ${SQLITE_PATH}/sqlite3.h
  ${SQLITE_PATH}/sqlite3.c
  BaseEvent.h
  BaseEvent.cpp
  DataBase.h
  DataBase.cpp
  PropMen.h
  PropMen.cpp
  Studs3D.h 
  Studs3D.cpp
  Resource.h
  SLIDE.RH
  Studs3D1_res.cpp
)

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
)

set_target_properties(${this_target} PROPERTIES
  OUTPUT_NAME ${this_target_name}
  SUFFIX ".rtw"
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfoWRK) 
  target_include_directories(${this_target} PRIVATE
	${KOMPAS_INCLUDE_DIRECTORIES}
	${COMMON_INCLUDE_DIRECTORIES}
	${KOMPAS_OUTPUT_DIR}/LibToolDis/LibToolDis
	$ENV{KOMPAS_WRK}/Source/
	$ENV{KOMPAS_WRK}/Source/API/KsAPI/KsAPI
	${SQLITE_PATH}
  )

  target_link_directories(${this_target} PRIVATE
	${KOMPAS_OUTPUT_DIR}
	${COMMON_LINK_DIRECTORIES}
  )
else()
  target_include_directories(${this_target} PRIVATE
	${KOMPAS_INCLUDE_DIRECTORIES}
	${COMMON_INCLUDE_DIRECTORIES}

	$ENV{KOMPAS_SDK}/KsAPI/Include
	${SQLITE_PATH}
  )

  target_link_directories(${this_target} PRIVATE
	$ENV{KOMPAS_SDK}/KsAPI/Lib64
	${KOMPAS_OUTPUT_DIR}
	${COMMON_LINK_DIRECTORIES}
  )
endif()

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

copy(${CMAKE_SOURCE_DIR}/../Content/Studs3D1.xml ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Studs3D1.db ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/ElementsData.db ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/Stud.l3d ${OUTPUT_DIRECTORY})
