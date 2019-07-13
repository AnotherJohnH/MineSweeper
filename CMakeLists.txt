
set(source  Source/mines.cpp)
set(binary  mines)
set(app     MineSweeper)
set(version 0.0.1)

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)
include(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${platform_libs})
