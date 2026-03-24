cmake_minimum_required(VERSION 3.10)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Run: mkdir build && cd build && cmake ..")
endif()

project(simpleoption_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(DEMOS
    demo_binomial_greeks_option
    demo_binomial_greeks_option_calc_alpha
    demo_bisection
    demo_bisection_secant
    demo_grid_search
    demo_inverse_cumulative_normal_distribution
    demo_newton_raphson
    demo_option_tenor
    demo_secant
)

foreach(DEMO ${DEMOS})
    add_executable(${DEMO} src/${DEMO}.cpp)
    if(TARGET simpleoption)
        target_link_libraries(${DEMO} simpleoption)
    else()
        target_link_libraries(${DEMO}
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/libsimpleoption.so
        )
    endif()
endforeach()
