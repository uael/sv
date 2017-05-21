add_executable(semver semver.c)
target_link_libraries(semver ${PROJECT_NAME})
add_test(semver semver)

add_executable(comp comp.c)
target_link_libraries(comp ${PROJECT_NAME})
add_test(comp comp)

add_executable(range range.c)
target_link_libraries(range ${PROJECT_NAME})
add_test(range range)

add_executable(match match.c)
target_link_libraries(match ${PROJECT_NAME})
add_test(match match)

add_executable(utils utils.c)
target_link_libraries(utils ${PROJECT_NAME})
add_test(utils utils)
