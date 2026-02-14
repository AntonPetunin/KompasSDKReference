cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Gayka1")
set(this_target_name "Gayka1")

if (UNIX)
    add_compile_definitions(SQLITE_OS_UNIX=1)
endif()

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Case_2023.09.28_17.34.24/rtw)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "Gayka1.rc"
	${SQLITE_PATH}/sqlite3.c
	${SQLITE_PATH}/sqlite3.h
	BaseProcess.cpp
	BaseProcess.h
	Bolt.cpp
	Bolt.h
	BoltMainProcess.cpp
	BoltMainProcess.h
	DataBase.cpp
	DataBase.h
	Doc2DDispatcher.cpp
	Doc2DDispatcher.h
	Entry.cpp
	Gayka.cpp
	Gayka.h
	Gayka5915Process.cpp
	Gayka5915Process.h
	HotPoint.cpp
	HotPoint.h
	IHatchUtil.h
	ILeaderProcess.h
	IPlacementProcess.h
	ParameterObject.cpp
	ParameterObject.h
	ParameterObjectProcess.cpp
	ParameterObjectProcess.h
	LeaderBuildProcess.cpp
	LeaderBuildProcess.h
	PropertyManagerObject.cpp
	PropertyManagerObject.h
	ProcessDispatcher.cpp
	ProcessDispatcher.h
	Resource.h
	"Gayka1_pch.h"
	Gayka1_res.cpp
)

add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
 )

set_target_properties(${this_target} PROPERTIES
  SUFFIX ".rtw"
  FOLDER "Samples"
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_include_directories(${this_target} PRIVATE
	${KOMPAS_INCLUDE_DIRECTORIES}
	${COMMON_INCLUDE_DIRECTORIES}
	$ENV{KOMPAS_WRK}/Source/API/API2DCore/API2DCore
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
	${SQLITE_PATH}
  )

  target_link_directories(${this_target} PRIVATE
  	$ENV{KOMPAS_SDK}/KsAPI/Lib64
	${KOMPAS_OUTPUT_DIR}
	${COMMON_LINK_DIRECTORIES}
  )
endif()


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

target_precompile_headers(${this_target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Gayka1_pch.h>")

copy(${CMAKE_SOURCE_DIR}/../Content/Gayka1.xml ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/ElementsData.db ${OUTPUT_DIRECTORY})
