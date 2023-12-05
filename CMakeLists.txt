cmake_minimum_required(VERSION 3.13)
project(winplus)

# Files
file(
    GLOB winplus_HEADERS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*.*"
)
aux_source_directory(src winplus_SRCS)

file(
    GLOB winux_HEADERS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "thirdparties/winux/include/*.*"
)
#aux_source_directory(thirdparties/winux/src winux_SRCS)
set(winux_SRC_PATH "thirdparties/winux/src")
list(
    APPEND winux_SRCS
    "${winux_SRC_PATH}/utilities.cpp"
    "${winux_SRC_PATH}/time.cpp"
    "${winux_SRC_PATH}/smartptr.cpp"
    "${winux_SRC_PATH}/strings.cpp"
    "${winux_SRC_PATH}/json.cpp"
    "${winux_SRC_PATH}/filesys.cpp"
    "${winux_SRC_PATH}/console.cpp"
    "${winux_SRC_PATH}/system.cpp"
)

# Targets
add_library(winplus SHARED ${winplus_SRCS} ${winplus_HEADERS} ${winux_SRCS} ${winux_HEADERS})
target_include_directories(winplus PUBLIC include PRIVATE thirdparties/winux/include)
target_compile_definitions(winplus PUBLIC WINPLUS_DLL_USE PRIVATE WINPLUS_DLL_EXPORTS)

add_library(winplus_a STATIC ${winplus_SRCS} ${winplus_HEADERS} ${winux_SRCS} ${winux_HEADERS})
target_include_directories(winplus_a PUBLIC include PRIVATE thirdparties/winux/include)

set_target_properties(winplus winplus_a PROPERTIES PUBLIC_HEADER "${winplus_HEADERS}")

include(GNUInstallDirs)
# Install
install(
    TARGETS winplus winplus_a
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
    PUBLIC_HEADER DESTINATION include
)

include(CPack)
