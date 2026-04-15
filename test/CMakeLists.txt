function(add_sv_test name)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} ${PROJECT_NAME})
  if (WIN32)
    set_target_properties(${name} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
  endif ()
  add_test(${name} ${name})
endfunction()

foreach (name version comp range match utils usage semvers)
  add_sv_test(${name})
endforeach ()
