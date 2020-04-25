cmake_minimum_required(VERSION 3.2)
if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(bulk VERSION 0.0.1)
endif()
#add_subdirectory(docs)
add_executable(bulk main.cpp)
target_include_directories(bulk PRIVATE ${PROJECT_SOURCE_DIR})
set_target_properties(bulk PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
 #   ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
 #   LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
 #   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic
        #-Werror
    )
endif()
install(TARGETS bulk RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
find_package(GTest REQUIRED)
find_package(Threads)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()
add_executable(gtest_hw7 tests/gtests.cpp)
target_link_libraries(gtest_hw7
    ${GTEST_LIBRARIES}
   )
target_link_libraries(gtest_hw7
${CMAKE_THREAD_LIBS_INIT}
)
set_property(TARGET gtest_hw7 PROPERTY CXX_STANDARD 14)
