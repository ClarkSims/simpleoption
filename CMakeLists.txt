cmake_minimum_required(VERSION 3.10)
project(simpleoption VERSION 1.0.0 DESCRIPTION "functions for European and American option calculations")

set(CMAKE_INSTALL_LIBDIR lib) 
include(GNUInstallDirs)

set(CMAKE_BUILD_TYPE RelWithDebInfo)


set(SIMPLEOPTION_PUBLIC_INCLUDE_DIRECTORIES      include/ )

file(GLOB_RECURSE SIMPLEOPTION_INCLUDE_FILES "include/simpleoption/*.h")

file(GLOB_RECURSE SIMPLEOPTION_SOURCE_FILES "src/*.cpp")

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED ${SIMPLEOPTION_INCLUDE_FILES} ${SIMPLEOPTION_SOURCE_FILES} )

target_include_directories(${PROJECT_NAME} PUBLIC ${SIMPLEOPTION_PUBLIC_INCLUDE_DIRECTORIES})

set_target_properties(simpleoption PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simpleoption PROPERTIES SOVERSION 1)

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/simpleoption
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")

configure_file(simpleoption.pc.in simpleoption.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/simpleoption.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

add_subdirectory(test)
add_subdirectory(demo)
