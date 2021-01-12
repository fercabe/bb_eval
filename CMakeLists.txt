cmake_minimum_required(VERSION 3.10.2)
project(bb_eval)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Bring the headers into the project
include_directories(include)
 
# Manually add the sources 
set(SOURCES src/bb_eval.cpp) 
  
# Create the executable file
add_executable(bb_eval ${SOURCES})

# Install actions. Use "sudo make install" to apply
install(TARGETS bb_eval DESTINATION /usr/local/bin)

