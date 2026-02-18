cmake_minimum_required(VERSION 3.13)
project(winplus VERSION 1.0)

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
    "${winux_SRC_PATH}/encoding.cpp"
    "${winux_SRC_PATH}/md5.cpp"
    "${winux_SRC_PATH}/sha1.c"
    "${winux_SRC_PATH}/sha2.c"
)

# Targets
add_library(winplus SHARED ${winplus_SRCS} ${winplus_HEADERS} ${winux_SRCS} ${winux_HEADERS})
target_include_directories(
    winplus 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparties/winux/include>$<INSTALL_INTERFACE:include>
)
target_compile_definitions(winplus PUBLIC WINPLUS_DLL_USE PRIVATE WINPLUS_DLL_EXPORTS)

add_library(winplus_a STATIC ${winplus_SRCS} ${winplus_HEADERS} ${winux_SRCS} ${winux_HEADERS})
target_include_directories(
    winplus_a 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparties/winux/include>$<INSTALL_INTERFACE:include>
)

set_target_properties(winplus winplus_a PROPERTIES PUBLIC_HEADER "${winplus_HEADERS}")

# Export targets for the build tree
#export(TARGETS winplus winplus_a NAMESPACE WinPlus:: FILE WinPlusTargets.cmake)

include(GNUInstallDirs)
# Install
install(
    TARGETS winplus winplus_a
    EXPORT WinPlusTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
    PUBLIC_HEADER DESTINATION include
)
# Generate <PackageName>Targets.cmake for build-tree
export(EXPORT WinPlusTargets NAMESPACE WinPlus::)
# Register build-tree-dir of <PackageName> to User Registry(HKCU/Software/Kitware/CMake/Packages)
export(PACKAGE WinPlus)

# Generate <PackageName>Targets.cmake for install-tree
install(
    EXPORT WinPlusTargets NAMESPACE WinPlus::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
)

include(CMakePackageConfigHelpers)
# Generate <PackageName>Config.cmake
configure_package_config_file(
    "Config.cmake.in"
    "WinPlusConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# Generate <PackageName>ConfigVersion.cmake
include(WriteBasicConfigVersionFile)
write_basic_package_version_file(
    "WinPlusConfigVersion.cmake"
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/WinPlusConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/WinPlusConfigVersion.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
)

include(CPack)
