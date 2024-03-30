cmake_minimum_required(VERSION 3.20)
project(example)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(linklist_test tests/linklist_test.c src/container/linklist.c)
set_target_properties(linklist_test PROPERTIES
                C_STANDARD 99
                C_STANDARD_REQUIRED ON
                C_EXTENSIONS OFF
            )
target_compile_definitions(linklist_test PRIVATE -DUNITEST_MAIN)
target_include_directories(linklist_test PRIVATE /home/hui/c_std/include)
