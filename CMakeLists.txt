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

# # Files for dictionary definition
set(LINKDEF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h)
set(DICT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/TAdaptiveGrid3.h)

# Generate the dictionary
ROOT_GENERATE_DICTIONARY(G__TAdaptiveGrid3 ${DICT_HEADER_FILES} MODULE TAdaptiveGrid3 LINKDEF ${LINKDEF_FILE} )

# create library
set(LIB_SOURCES src/TAdaptiveGrid3.cpp)
# add_library(TAdaptiveGrid3 SHARED ${LIB_SOURCES} G__TAdaptiveGrid3.cxx)
add_library(TAdaptiveGrid3 SHARED ${LIB_SOURCES})
target_link_libraries(TAdaptiveGrid3 ROOT::Core ROOT::MathCore ROOT::Matrix ROOT::Physics)



# install library
install(TARGETS TAdaptiveGrid3 DESTINATION lib)
install(FILES include/TAdaptiveGrid3.h DESTINATION ${CMAKE_BINARY_DIR}/include)

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
