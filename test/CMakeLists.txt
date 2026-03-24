cmake_minimum_required(VERSION 3.10)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Run: mkdir build && cd build && cmake ..")
endif()

project(simpleoption_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(TESTS
    test1.americanoption
    test1.simpleoption
    test2.americanoption
    test3.simpleoption
    test4.americanoption
    test4.simpleoption
    test.allgreeks
    test.allopt
    test.allvol
    test.american_option_approximation
    test.binomial_greeks
    test_cdf
)

foreach(TEST ${TESTS})
    add_executable(${TEST} src/${TEST}.cpp)
    if(TARGET simpleoption)
        target_link_libraries(${TEST} simpleoption)
    else()
        target_link_libraries(${TEST}
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/libsimpleoption.so
        )
    endif()
endforeach()
