cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(this_target "Events")
set(this_target_name "Events")

if (UNIX)
    add_compile_definitions(SQLITE_OS_UNIX=1)
endif()

project(${this_target})

# Используется для сборки библиотек для тестов
if (TESTS)
  add_compile_definitions(TESTS=${TESTS})
	set(TEST_DIRECTORY ${OUTPUT_DIRECTORY}/Sources/KsAPI/rtw/)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${TEST_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY      ${TEST_DIRECTORY})
	message(STATUS "From ${this_target}: test output dir: ${TEST_DIRECTORY}")
endif()

set(${this_target}_SOURCE_FILES
  "Events.rc"
  "Events.cpp"
  "Resource.h"
  "Events_pch.h"
  "Events_res.cpp"
  "BaseEvent.cpp"
  "BaseEvent.h"
  "ApplicationEvent.h"
  "ApplicationEvent.cpp"
  "LibFunctions.cpp"
  "LibFunctions.h"
  "DocumentEvent.cpp"
  "DocumentEvent.h"
  "Document2DEvent.cpp"
  "Document2DEvent.h"
  "Document3DEvent.cpp"
  "Document3DEvent.h"
  "DocumentFrameEvent.cpp"
  "DocumentFrameEvent.h"
  "DrawingObjectEvent.cpp"
  "DrawingObjectEvent.h"
  "SelectMngEvent.cpp"
  "SelectMngEvent.h"
  "StampEvent.cpp"
  "StampEvent.h"
  "ModelObjectEvent.cpp"
  "ModelObjectEvent.h"
  "SpecificationObjectEvent.cpp"
  "SpecificationObjectEvent.h"
  "SpecificationDocumentEvent.cpp"
  "SpecificationDocumentEvent.h" 
  "SpecificationDescriptionEvent.cpp"
  "SpecificationDescriptionEvent.h" 
  "IniFile.cpp"
  "IniFile.h"
)
  
add_library(${this_target} SHARED
  ${${this_target}_SOURCE_FILES}
 )

set_target_properties(${this_target} PROPERTIES
  OUTPUT_NAME Events
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

target_precompile_headers(${this_target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Events_pch.h>")

if(TESTS)
  message(STATUS "From ${this_target}: Added Snapshot")

  add_library(Snapshot STATIC 
  ${OUTPUT_DIRECTORY}/Sources/Snapshot/Snapshot.h
  ${OUTPUT_DIRECTORY}/Sources/Snapshot/Snapshot.cpp
  ${OUTPUT_DIRECTORY}/Sources/Snapshot/pugixml/pugiconfig.hpp
  ${OUTPUT_DIRECTORY}/Sources/Snapshot/pugixml/pugixml.cpp
  ${OUTPUT_DIRECTORY}/Sources/Snapshot/pugixml/pugixml.hpp
  )

  target_include_directories(Snapshot PRIVATE
  ${KOMPAS_INCLUDE_DIRECTORIES}
  ${COMMON_INCLUDE_DIRECTORIES} 
  )

  target_link_directories(Snapshot PRIVATE
  ${KOMPAS_LINK_DIRECTORIES}
  ${COMMON_LINK_DIRECTORIES} 
  )

  target_compile_features(Snapshot PRIVATE cxx_std_20)
  target_compile_definitions(Snapshot PRIVATE -DSNAPSHOT_EXPORTS)

  target_link_libraries(${this_target} PRIVATE
  "Snapshot"
  )

  include_directories(${OUTPUT_DIRECTORY}/Sources/Snapshot/)
  
endif()

copy(${CMAKE_SOURCE_DIR}/../Content/Events.xml ${OUTPUT_DIRECTORY})
