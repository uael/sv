add_executable(semver semver.c)
target_link_libraries(semver ${PROJECT_NAME})
add_test(semver semver)

add_executable(comparison comparison.c)
target_link_libraries(comparison ${PROJECT_NAME})
add_test(comparison comparison)

add_executable(comp comp.c)
target_link_libraries(comp ${PROJECT_NAME})
add_test(comp comp)
