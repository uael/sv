include(CMakeParseArguments)

project(sv C)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/cmake/properties.cmake)
  message(FATAL_ERROR "cmake/properties.cmake file should exists.")
endif ()
include(${CMAKE_CURRENT_LIST_DIR}/cmake/properties.cmake)

include(GNUInstallDirs) # include this *AFTER* PROJECT(), otherwise paths are wrong.

get_directory_property(${PROJECT_NAME}_PARENT PARENT_DIRECTORY)
if (NOT ${PROJECT_NAME}_PARENT)
  set(${PROJECT_NAME}_DEVEL TRUE)
endif ()

if (NOT COMMAND project_dependency)
  function(project_dependency DEP)
    cmake_parse_arguments(DL_ARGS "TEST;LINK" "" "" ${ARGN})
    if (DL_ARGS_TEST)
      set(${PROJECT_NAME}_TEST_DEPS ${${PROJECT_NAME}_TEST_DEPS} ${DEP} PARENT_SCOPE)
    else ()
      set(${PROJECT_NAME}_DEPS ${${PROJECT_NAME}_DEPS} ${DEP} PARENT_SCOPE)
    endif ()
    set(${DEP}_LINK ${DL_ARGS_LINK} PARENT_SCOPE)
    if (NOT DL_ARGS_TEST OR ${PROJECT_NAME}_DEVEL)
      set(DL_ARGS_BUILD_DIR "${CMAKE_BINARY_DIR}/vendor/${DEP}")
      set(DL_ARGS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/${DEP}")
      file(WRITE "${DL_ARGS_BUILD_DIR}/CMakeLists.txt"
        "cmake_minimum_required(VERSION 3.5 FATAL_ERROR)\n"
        "project(vendor-${DEP} NONE)\n"
        "include(ExternalProject)\n"
        "ExternalProject_Add(vendor-${DEP}\n"
        "${DL_ARGS_UNPARSED_ARGUMENTS}\nSOURCE_DIR \"${DL_ARGS_SOURCE_DIR}\"\n"
        "CONFIGURE_COMMAND \"\"\nBUILD_COMMAND \"\"\nINSTALL_COMMAND \"\"\nTEST_COMMAND \"\")")
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
      add_subdirectory(${DL_ARGS_SOURCE_DIR})
    endif ()
  endfunction()
endif ()

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake)
endif ()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
  set(CLANG TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
  set(GCC TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Intel")
  set(ICC TRUE)
elseif (NOT MSVC)
  message(FATAL_ERROR "Unknown compiler")
endif ()

set(CMAKE_C_STANDARD 99)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(COVERAGE "Turn on COVERAGE support" OFF)
if (COVERAGE AND NOT MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif ()

set(${PROJECT_NAME}_HEADERS)
set(${PROJECT_NAME}_SOURCES)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(${PROJECT_NAME}_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_INCLUDE_DIR}/semver.h)
if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR})
  file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/*.h)
endif ()
if (EXISTS ${${PROJECT_NAME}_SOURCE_DIR})
  file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_SOURCE_DIR}/*.c)
endif ()

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_HEADERS}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${${PROJECT_NAME}_MAJOR}.${${PROJECT_NAME}_MINOR}.${${PROJECT_NAME}_PATCH}")
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${${PROJECT_NAME}_MAJOR})

target_compile_definitions(${PROJECT_NAME} PRIVATE SV_COMPILE=1)
if (${BUILD_SHARED_LIBS})
  target_compile_definitions(${PROJECT_NAME} PRIVATE SV_BUILD_DYNAMIC_LINK=1)
endif ()

if (${PROJECT_NAME}_DEPS)
  foreach (DEP ${${PROJECT_NAME}_DEPS})
    add_dependencies(${PROJECT_NAME} ${DEP})
    if (${DEP}_LINK)
      target_link_libraries(${PROJECT_NAME} ${DEP})
    endif ()
  endforeach ()
endif ()

if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR})
  target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR})
endif ()
if (EXISTS ${${PROJECT_NAME}_SOURCE_DIR})
  target_include_directories(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SOURCE_DIR})
endif ()

if (MSVC)
  if (${PROJECT_NAME}_MSVC_COMPILE_OPTIONS)
    target_compile_options(${PROJECT_NAME}
      PRIVATE /Oy
      PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>
      PRIVATE ${${PROJECT_NAME}_MSVC_COMPILE_OPTIONS})
  else ()
    target_compile_options(${PROJECT_NAME}
      PRIVATE /Oy
      PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
  endif ()
else ()
  if (${PROJECT_NAME}_COMPILE_OPTIONS)
    target_compile_options(${PROJECT_NAME}
      PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer
      PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>
      PRIVATE ${${PROJECT_NAME}_COMPILE_OPTIONS})
  else ()
    target_compile_options(${PROJECT_NAME}
      PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer
      PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
  endif ()
endif ()

if (MSVC)
  set(CMAKE_FLAGS
    CMAKE_C_FLAGS CMAKE_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
  foreach (CMAKE_FLAG ${CMAKE_FLAGS})
    string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
    string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
  endforeach ()
endif ()

if (${PROJECT_NAME}_DEVEL)
  if (EXISTS ${${PROJECT_NAME}_TEST_DIR})
    enable_testing()

    file(GLOB ctest_SOURCES ${ctest_SOURCES} ${${PROJECT_NAME}_TEST_DIR}/*.c)
    foreach (ctest_SRC ${ctest_SOURCES})
      get_filename_component(ctest_NAME ${ctest_SRC} NAME_WE)
      add_executable(test_${ctest_NAME} ${ctest_SRC})
      add_dependencies(test_${ctest_NAME} ${PROJECT_NAME})
      target_link_libraries(test_${ctest_NAME} ${PROJECT_NAME})
      if (${PROJECT_NAME}_TEST_DEPS)
        foreach (DEP ${${PROJECT_NAME}_TEST_DEPS})
          add_dependencies(test_${ctest_NAME} ${DEP})
          if (${DEP}_LINK)
            target_link_libraries(test_${ctest_NAME} ${DEP})
          endif ()
        endforeach ()
      endif ()
      add_test(${ctest_NAME} test_${ctest_NAME})
    endforeach ()
  endif ()
else ()
  set(${PROJECT_NAME}_HEADERS ${PROJECT_NAME}_HEADERS PARENT_SCOPE)
  set(${PROJECT_NAME}_SOURCES ${PROJECT_NAME}_SOURCES PARENT_SCOPE)
  set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_NAME}_INCLUDE_DIR PARENT_SCOPE)
  set(${PROJECT_NAME}_SOURCE_DIR ${PROJECT_NAME}_SOURCE_DIR PARENT_SCOPE)
  set(${PROJECT_NAME}_TEST_DIR ${PROJECT_NAME}_TEST_DIR PARENT_SCOPE)
endif ()

block()
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(bindir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
set(datarootdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR})
set(includedir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
set(infodir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INFODIR})
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(libexecdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR})
set(localstatedir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR})
set(mandir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MANDIR})
set(sbindir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SBINDIR})
set(sharedstatedir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SHAREDSTATEDIR})
set(sysconfdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR})
set(PACKAGE_NAME ${PROJECT_NAME})
set(LIBSV_PKG_CONFIG_VERSION
    ${${PROJECT_NAME}_MAJOR}.${${PROJECT_NAME}_MINOR}.${${PROJECT_NAME}_PATCH})
configure_file("scripts/libsv.pc.in" "scripts/libsv.pc" @ONLY)
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/scripts/libsv.pc"
  COMPONENT Devel
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endblock()

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}.h)
  install(FILES ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}_SUMMARY")
set(CPACK_PACKAGE_VENDOR "${PROJECT_NAME}_VENDOR")
set(CPACK_PACKAGE_CONTACT "${PROJECT_NAME}_CONTACT")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_LIST_DIR}/${${PROJECT_NAME}_README}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/${${PROJECT_NAME}_LICENSE}")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
if (WIN32 AND NOT UNIX)
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
  set(CPACK_NSIS_CONTACT "${PROJECT_NAME}_VENDOR_CONTACT")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_GENERATOR "NSIS;ZIP")
else ()
  set(CPACK_GENERATOR "TGZ;RPM;DEB")
endif ()
include(CPack)
