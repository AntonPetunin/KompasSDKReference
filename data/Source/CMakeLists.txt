cmake_minimum_required(VERSION 3.29.5 FATAL_ERROR)

if (NOT TESTS)
project(Samples C CXX)

message(STATUS "Output dir: ${OUTPUT_DIRECTORY}")

# Build type
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)

# Platform
set(PLATFORM "x64" CACHE STRING "Build platform")

# Kompas targets
include(${CMAKE_SOURCE_DIR}/CMake/CompileOptions.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${OUTPUT_DIRECTORY})
set(CMAKE_PDB_OUTPUT_DIRECTORY      ${OUTPUT_DIRECTORY})

# Visual Studio output dir workaround
# https://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIRECTORY})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIRECTORY})
  set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG}     ${OUTPUT_DIRECTORY})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# Kompas binary output dir
message(STATUS "Kompas output external dir: ${KOMPAS_OUTPUT_DIR}")
message(STATUS "RC flags: ${CMAKE_RC_FLAGS}")

set(SQLITE_PATH ${CMAKE_SOURCE_DIR}/ThirdPart/sqlite)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfoWRK)
	set(KOMPAS_INCLUDE_DIRECTORIES
	  ${KOMPAS_OUTPUT_DIR}/LibToolDis/LibToolDis
	  $ENV{KOMPAS_WRK}/Source/API/KsAPI/KsAPI
	  ${SQLITE_PATH}
	)

	set(KOMPAS_LINK_DIRECTORIES
	  ${KOMPAS_OUTPUT_DIR}
	)
else()
	set(KOMPAS_INCLUDE_DIRECTORIES
	  $ENV{KOMPAS_SDK}/KsAPI/Include
	  ${SQLITE_PATH}
	)

	set(KOMPAS_LINK_DIRECTORIES
	  $ENV{KOMPAS_SDK}/KsAPI/Include
    $ENV{KOMPAS_SDK}/KsAPI/Lib64
    ${KOMPAS_OUTPUT_DIR}
	)
endif()

message(STATUS "Kompas API include dir: ${KOMPAS_INCLUDE_DIRECTORIES}")
message(STATUS "Kompas API libs dir: ${KOMPAS_LINK_DIRECTORIES}")

set(COMMON_INCLUDE_DIRECTORIES
  "."
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

set(COMMON_LINK_DIRECTORIES
  "."  
  ${OUTPUT_DIRECTORY}
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
endif()

set(BUILD_VERSION_MACROS
  "BuildVersionMajor=24"
  "BuildVersionMinor=0"
  "BuildVersionPatch=0"
  "BuildVersionBuild=0"
)

set(COMMON_PROJECT_MACROS
  ${COMMON_MACROS}
  ${BUILD_VERSION_MACROS}
)

macro(copy src dst)
  if (TESTS)
    message (STATUS "Копирование ${src} в ${dst} не выполняется (проект автотестов).")  
  else()
    file(COPY ${src} DESTINATION ${dst})
  endif()
endmacro()

copy(${CMAKE_SOURCE_DIR}/../Content/Resources ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/AsconIconsSDK-Accent.otf ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/AsconIconsSDK-Base.otf ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/AsconIconsSDK-Color1.otf ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/AsconIconsSDK-Color2.otf ${OUTPUT_DIRECTORY})
copy(${CMAKE_SOURCE_DIR}/../Content/AsconIconsSDK-Color3.otf ${OUTPUT_DIRECTORY})

add_subdirectory(Cube)

#OneWindow
add_subdirectory(OneWindow)

#Gayka1
add_subdirectory(Gayka1)

#MacroObj
add_subdirectory(MacroObj)

#Steps
add_subdirectory(Steps)

#IPropertyTree_Sample
add_subdirectory(IPropertyTree_Sample)

#UserSymbolWithLeader
add_subdirectory(UserSymbolWithLeader)

#Events
add_subdirectory(Events)

#MyConverter
add_subdirectory(MyConverter)

#ConverterTst
add_subdirectory(ConverterTst)
