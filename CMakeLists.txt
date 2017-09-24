cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

if (NOT EXISTS ${CMAKE_CACHEFILE_DIR}/cmake)
  execute_process(
    COMMAND git clone https://github.com/uael/cmake-modules.git cmake
    RESULT_VARIABLE result OUTPUT_QUIET
    WORKING_DIRECTORY ${CMAKE_CACHEFILE_DIR}
  )
endif ()

set(CMAKE_MODULE_PATH "${CMAKE_CACHEFILE_DIR}/cmake")
include(ProjectDependency)

project(ufs C)
project_dependency(uds GIT_REPOSITORY https://github.com/uael/uds.git)
project_dependency(unt GIT_REPOSITORY https://github.com/uael/unt.git)

set(ufs_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ufs_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ufs_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB_RECURSE ufs_SRCS ${ufs_SRCS} ${ufs_SRC_DIR}/*.c)
file(GLOB_RECURSE ufs_HDRS ${ufs_HDRS} ${ufs_INC_DIR}/ufs/*.h)
set(ufs_HDR ${ufs_INC_DIR}/ufs.h)

add_library(ufs ${ufs_SRCS} ${ufs_HDRS} ${ufs_HDR})
target_include_directories(ufs PUBLIC ${ufs_INC_DIR})
target_link_libraries(ufs PUBLIC uds unt)

get_directory_property(ufs_PARENT PARENT_DIRECTORY)
if (NOT ${ufs_PARENT})
  set(ufs_DEVEL TRUE)
elseif (NOT ${ufs_DEVEL})
  set(ufs_DEVEL FALSE)
endif ()
if (${ufs_DEVEL} AND EXISTS ${ufs_TEST_DIR})
  project_dependency(cute GIT_REPOSITORY https://github.com/uael/cute.git)
  if (NOT ufs_PARENT)
    enable_testing()
  endif ()

  file(GLOB ufs_TEST_SRCS ${ufs_TEST_SRCS} ${ufs_TEST_DIR}/*.c)
  foreach (ufs_TEST_SRC ${ufs_TEST_SRCS})
    get_filename_component(ufs_TEST_NAME ${ufs_TEST_SRC} NAME_WE)
    add_executable(test_${ufs_TEST_NAME} ${ufs_TEST_SRC})
    add_dependencies(test_${ufs_TEST_NAME} ufs cute)
    target_link_libraries(test_${ufs_TEST_NAME} ufs cute)
    add_test(${ufs_TEST_NAME} test_${ufs_TEST_NAME})
  endforeach ()
endif ()

install(FILES ${ufs_HDRS}
  DESTINATION include/ufs)
install(FILES ${ufs_HDR}
  DESTINATION include)