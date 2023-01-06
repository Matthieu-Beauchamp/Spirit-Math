# TODO: support installing
# https://www.foonathan.net/2016/03/cmake-install/ 

############################################################
# cmake config
############################################################

cmake_minimum_required(VERSION 3.13)

project(Spirit-Math)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

############################################################
# C++ config
############################################################
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS ${SPIRIT_COMPILE_FLAGS})


############################################################
# Access other spirit modules
############################################################
if (NOT SPIRIT_SUPER_PROJECT)
    # https://cmake.org/cmake/help/latest/module/FetchContent.html
    include(FetchContent)
    
    FetchContent_Declare(
        Spirit-Base
        GIT_REPOSITORY https://github.com/Matthieu-Beauchamp/Spirit-Base.git
        GIT_TAG        origin/master
    )

    if (SPIRIT_MATH_BUILD_TESTS)
        set(SPIRIT_BASE_BUILD_CATCH TRUE)
    endif()
    
    if (SPIRIT_MATH_BUILD_BENCHMARKS)
        set(SPIRIT_BASE_BUILD_CELERO TRUE)
    endif()
    
    FetchContent_MakeAvailable(Spirit-Base) # ...
endif()


############################################################
# Spirit configuration options
############################################################

spirit_define_option(
        BUILD_SHARED_LIBS
        FALSE BOOL
        "Choose if Spirit's dependencies should be linked statically
        or dynamically (On Windows, DLLs will be put with the executables automatically)"
)

spirit_define_option(
        SPIRIT_MATH_BUILD_TESTS
        FALSE BOOL
        "Select if tests should be built"
)

spirit_define_option(
        SPIRIT_MATH_BUILD_DOCS
        FALSE BOOL
        "Select if docs should be built"
)

spirit_define_option(
        SPIRIT_MATH_BUILD_EXAMPLES
        FALSE BOOL
        "Select if examples should be built"
)

spirit_define_option(
        SPIRIT_MATH_BUILD_BENCHMARKS
        FALSE BOOL
        "Select if examples should be built"
)


############################################################
# Paths
############################################################
set(SPIRIT_MATH_ROOT ${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/built)
set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/lib)

set(SPIRIT_MATH_INCLUDE_DIR ${SPIRIT_MATH_ROOT}/include)
set(SPIRIT_MATH_SRC_DIR ${SPIRIT_MATH_ROOT}/src/SPIRIT)


############################################################
# Build
############################################################
spirit_update_submodules(${SPIRIT_MATH_ROOT})

add_library(spirit-math "")

target_link_libraries(spirit-math spirit-base)

target_include_directories(spirit-math PUBLIC ${SPIRIT_MATH_INCLUDE_DIR})


# Dependencies #############################################

# xsimd
add_subdirectory(ext_libs/xsimd)
target_link_libraries(spirit-math xsimd)

# eigen
set(EIGEN_BUILD_DOC OFF CACHE INTERNAL "Don't litter with HTML" FORCE)
add_subdirectory(ext_libs/eigen)
target_link_libraries(spirit-math eigen)


# Build Spirit Module #############################################

# TODO: Include headers as sources (helps for meta information)
set(SPIRIT_MATH_COMPONENTS 
    Component
)

foreach (COMPONENT ${SPIRIT_MATH_COMPONENTS})
    add_subdirectory("${SPIRIT_MATH_SRC_DIR}/${COMPONENT}")
endforeach ()


if (SPIRIT_MATH_BUILD_TESTS)
    add_subdirectory("${SPIRIT_MATH_ROOT}/test")
endif ()

if (SPIRIT_MATH_BUILD_DOCS)
    add_subdirectory("${SPIRIT_MATH_ROOT}/docs/")
endif ()

if (SPIRIT_MATH_BUILD_EXAMPLES)
    add_subdirectory("${SPIRIT_MATH_ROOT}/examples/")
endif ()

if (SPIRIT_MATH_BUILD_BENCHMARKS)
    add_subdirectory("${SPIRIT_MATH_ROOT}/benchmarks/")
endif ()