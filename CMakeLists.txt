cmake_minimum_required(VERSION 3.20)
project(example)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(lib_log lib/log/log.c)
set_target_properties(lib_log PROPERTIES
																																																																																																																																																                    C_STANDARD 11
																																																																																																																																																		                    C_STANDARD_REQUIRED ON
																																																																																																																																																				                    C_EXTENSIONS OFF
																																																																																																																																																						                )
target_compile_definitions(lib_log PRIVATE )
target_include_directories(lib_log PRIVATE /home/hui/c_std/include)
add_executable(linklist_test tests/linklist_test.c src/container/linklist.c)
set_target_properties(linklist_test PROPERTIES
																																																																																																																																																                    C_STANDARD 11
																																																																																																																																																		                    C_STANDARD_REQUIRED ON
																																																																																																																																																				                    C_EXTENSIONS OFF
																																																																																																																																																						                )
target_compile_definitions(linklist_test PRIVATE -DUNITEST_MAIN)
target_include_directories(linklist_test PRIVATE /home/hui/c_std/include)
add_executable(sort_test tests/sort_test.c)
set_target_properties(sort_test PROPERTIES
																																																																																																																																																                    C_STANDARD 11
																																																																																																																																																		                    C_STANDARD_REQUIRED ON
																																																																																																																																																				                    C_EXTENSIONS OFF
																																																																																																																																																						                )
target_compile_definitions(sort_test PRIVATE -DUNITEST_MAIN)
target_include_directories(sort_test PRIVATE /home/hui/c_std/include)
