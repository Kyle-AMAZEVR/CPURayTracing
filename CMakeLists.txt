cmake_minimum_required(VERSION 3.0.0)
project(CPURayTracing)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)


set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories("inteltbb/tbb/include")
include_directories("inteltbb/pstl/include")
include_directories("inteltbb/pstl/stdlib")

link_directories("inteltbb/tbb/lib/intel64_win/vc_mt")

add_executable(CPURayTracing main.cpp Vector3.h Vector3.cpp)

target_link_libraries(CPURayTracing tbb.lib tbbbind.lib tbbmalloc.lib tbbproxy.lib)

add_custom_command(TARGET CPURayTracing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/inteltbb/redist/intel64_win/tbb/vc_mt/tbb.dll $<TARGET_FILE_DIR:CPURayTracing>/tbb.dll)
add_custom_command(TARGET CPURayTracing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/inteltbb/redist/intel64_win/tbb/vc_mt/tbbbind.dll $<TARGET_FILE_DIR:CPURayTracing>/tbbbind.dll)
add_custom_command(TARGET CPURayTracing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/inteltbb/redist/intel64_win/tbb/vc_mt/tbbmalloc.dll $<TARGET_FILE_DIR:CPURayTracing>/tbbmalloc.dll)
add_custom_command(TARGET CPURayTracing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/inteltbb/redist/intel64_win/tbb/vc_mt/tbbmalloc_proxy.dll $<TARGET_FILE_DIR:CPURayTracing>/tbbmalloc_proxy.dll)



###########################################################
