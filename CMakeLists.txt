cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(TAdaptiveGrid3)

set(CMAKE_CXX_STANDARD 17)

if(DEFINED ENV{ROOTSYS})
    message("Found ROOTSYS at $ENV{ROOTSYS}")
else()
    message("ROOTSYS not found.  Searching in /cern.")
    message("  => This will match the first ROOT install it finds, which might not be what you want!")
    list(APPEND CMAKE_PREFIX_PATH /cern)
endif()

find_package(ROOT 6.16 CONFIG REQUIRED) 

# ROOT's CMake utilities
include(${ROOT_USE_FILE})

# Include directories
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set output paths for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib)


MESSAGE("-- INCLUDING CUSTOM CMAKE TOOLS")
# import colors and dictionary builder
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/colors.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/root_tools.cmake)
MESSAGE("-- DONE INCLUDING CUSTOM CMAKE TOOLS")

# # Files for dictionary definition
# set(LINKDEF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h)
# set(DICT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/TAdaptiveGrid3.h)

set(ALL_CLASSES
    TAdaptiveGrid3)

set(DICT_CLASSES
    TAdaptiveGrid3)

string(REGEX REPLACE "([^;]+)" "src/\\1.cpp" DICT_SOURCES "${DICT_CLASSES}") 
string(REGEX REPLACE "([^;]+)" "\\1.h"       DICT_HEADERS "${DICT_CLASSES}") 
string(REGEX REPLACE "([^;]+)" "src/\\1.cpp" ALL_SOURCES  "${ALL_CLASSES}")  
string(REGEX REPLACE "([^;]+)" "\\1.h"       ALL_HEADERS  "${ALL_CLASSES}")  

# Generate the dictionary
root_dictionary_builder(TARGET_NAME TAdaptiveGrid3DictGenerated
                        CLASS_LIST ${DICT_CLASSES}
                        HEADER_LIST ${DICT_HEADERS})

# create library
set(LIB_SOURCES src/TAdaptiveGrid3.cpp)
add_library(TAdaptiveGrid3 SHARED ${LIB_SOURCES} TAdaptiveGrid3DictGenerated)
target_include_directories(TAdaptiveGrid3 PUBLIC include)
target_link_libraries(TAdaptiveGrid3 ROOT::Core ROOT::MathCore ROOT::Matrix ROOT::Physics)



# install library
install(TARGETS TAdaptiveGrid3 DESTINATION lib)
install(FILES ${DICT_HEADER_FILES} ${LINKDEF_FILE} DESTINATION ${CMAKE_BINARY_DIR}/include)
# install(FILES ${DICT_HEADER_FILES} DESTINATION ${CMAKE_BINARY_DIR}/include)

# testing
function(add_test_executable target)
    add_executable(${target} test/${target}.cpp)
    target_link_libraries(${target} TAdaptiveGrid3)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    add_test(NAME ${target} COMMAND ${target})
endfunction()
enable_testing()

# hello world
add_test_executable(hello_world)
add_test_executable(construct_tree)
