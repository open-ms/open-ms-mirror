project(tests)

set(my_tests
MRMScoring_test
Scoring_test
TestConvert
DiaHelpers_test
)

## add targets for the executables
foreach(i ${my_tests})
  add_executable(${i} ${i}.C)
  if(USE_BOOST_UNIT_TEST)
    target_link_libraries(${i} OpenSwathAlgo boost_unit_test_framework)
  else()
    target_link_libraries(${i} OpenMS)
  endif()

  add_test(${i} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${i})
endforeach(i)

add_dependencies(test_build ${my_tests} )

enable_testing()
