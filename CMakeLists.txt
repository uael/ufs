cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

include(CMakeParseArguments)
if (NOT COMMAND project_dependency)
  function(project_dependency DEP)
    get_property(loaded GLOBAL PROPERTY ${DEP}_LOADED)
    if (NOT loaded)
      cmake_parse_arguments(DL_ARGS "" "" "" ${ARGN})
      set(DL_ARGS_BUILD_DIR "${CMAKE_BINARY_DIR}/vendor/${DEP}")
      set(DL_ARGS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/${DEP}")
      file(WRITE "${DL_ARGS_BUILD_DIR}/CMakeLists.txt"
        "cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)\n"
        "project(vendor-${DEP} NONE)\n"
        "include(ExternalProject)\n"
        "ExternalProject_Add(vendor-${DEP}\n"
        "${DL_ARGS_UNPARSED_ARGUMENTS}\nSOURCE_DIR \"${DL_ARGS_SOURCE_DIR}\"\n"
        "CONFIGURE_COMMAND \"\"\nBUILD_COMMAND \"\"\nINSTALL_COMMAND \"\"\n"
        "TEST_COMMAND \"\"\nUPDATE_COMMAND \"\"\nPATCH_COMMAND \"\")")
      execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        -D "CMAKE_MAKE_PROGRAM:FILE=${CMAKE_MAKE_PROGRAM}" .
        RESULT_VARIABLE result OUTPUT_QUIET
        WORKING_DIRECTORY "${DL_ARGS_BUILD_DIR}")
      if (result)
        message(FATAL_ERROR "Config step for ${DEP} failed: ${result}")
      endif ()
      execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result OUTPUT_QUIET
        WORKING_DIRECTORY "${DL_ARGS_BUILD_DIR}")
      if (result)
        message(FATAL_ERROR "Build step for ${DEP} failed: ${result}")
      endif ()
      add_subdirectory(${DL_ARGS_SOURCE_DIR} ${DL_ARGS_BUILD_DIR}/build)
      set_property(GLOBAL PROPERTY ${DEP}_LOADED TRUE)
    endif ()
  endfunction()
endif ()

project(ufs C)
project_dependency(uds GIT_REPOSITORY https://github.com/uael/uds.git)

set(ufs_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ufs_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ufs_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB_RECURSE ufs_SRCS ${ufs_SRCS} ${ufs_SRC_DIR}/*.c)
file(GLOB_RECURSE ufs_HDRS ${ufs_HDRS} ${ufs_INC_DIR}/ufs/*.h)
set(ufs_HDR ${ufs_INC_DIR}/ufs.h)

add_library(ufs ${ufs_SRCS} ${ufs_HDRS} ${ufs_HDR})
target_include_directories(ufs PUBLIC ${ufs_INC_DIR})
target_link_libraries(ufs PUBLIC uds)

get_directory_property(ufs_PARENT PARENT_DIRECTORY)
if (NOT ${ufs_PARENT})
  set(ufs_DEVEL TRUE)
elseif (NOT ${ufs_DEVEL})
  set(ufs_DEVEL FALSE)
endif ()
if (${ufs_DEVEL} AND EXISTS ${ufs_TEST_DIR})
  if (NOT ufs_PARENT)
    enable_testing()
  endif ()

  file(GLOB ufs_TEST_SRCS ${ufs_TEST_SRCS} ${ufs_TEST_DIR}/*.c)
  foreach (ufs_TEST_SRC ${ufs_TEST_SRCS})
    get_filename_component(ufs_TEST_NAME ${ufs_TEST_SRC} NAME_WE)
    add_executable(test_${ufs_TEST_NAME} ${ufs_TEST_SRC})
    add_dependencies(test_${ufs_TEST_NAME} ufs)
    target_link_libraries(test_${ufs_TEST_NAME} ufs)
    add_test(${ufs_TEST_NAME} test_${ufs_TEST_NAME})
  endforeach ()
endif ()

install(FILES ${ufs_HDRS}
  DESTINATION include/ufs)
install(FILES ${ufs_HDR}
  DESTINATION include)