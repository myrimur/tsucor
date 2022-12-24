cmake_minimum_required(VERSION 3.15)

set(LIB_NAME tsucor)
set(PROJECT_NAME tsucor_example)

project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too much problem for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing. 
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan)
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.  
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
set(ENABLE_TSan OFF)
#! MemorySanitizer detects uninitialized memory reads 
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)

# Assembler options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_ASM_FLAGS "-masm=intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_ASM_FLAGS "-masm=intel -Wa,-msyntax=intel,-mnaked-reg")
endif()

#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

add_library(${LIB_NAME} SHARED src/coro.cpp src/pass.s)
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${LIB_NAME} PUBLIC inc)

target_link_libraries(${PROJECT_NAME} ${LIB_NAME})

install(PROGRAMS
		$<TARGET_FILE:${LIB_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION lib)

install(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME} ${LIB_NAME})

# Include CMake setup
include(cmake/main-config.cmake)
