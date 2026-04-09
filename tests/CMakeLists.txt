# Test executable
add_executable(memorc_tests
    test_main.cpp
    test_worker.cpp
    test_worker_pool.cpp
    test_task_queue.cpp
)

find_package(GTest REQUIRED)

find_package(Threads REQUIRED)
enable_testing()
# Link with library
target_link_libraries(memorc_tests PRIVATE 
                    memorc
                    GTest::GTest 
                    GTest::Main)
enable_testing()
# Enable testing
add_test(NAME memorc_tests COMMAND memorc_tests)


#Compile options
target_compile_options(memorc_tests PRIVATE
                    $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
target_link_libraries(memorc_tests PRIVATE
                    $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
# Add coverage if needed
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(memorc_tests PRIVATE --coverage)
    target_link_options(memorc_tests PRIVATE --coverage)
endif()
