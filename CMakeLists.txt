########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 2.8)
project(gras CXX C)
enable_testing()

set(GRAS_VERSION "0.0.0")
set(GRAS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GRAS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

function(GRAS_CHECK_SUBMODULE directory)
    if(NOT EXISTS ${GRAS_SOURCE_DIR}/modules/${directory}/CMakeLists.txt)
        message(FATAL_ERROR "
submodule ${directory} does not exist!!
Did you remember to run the following commands?
    git submodule init
    git submodule update
")
    endif()
endfunction(GRAS_CHECK_SUBMODULE)

GRAS_CHECK_SUBMODULE(Theron)
GRAS_CHECK_SUBMODULE(Apology)
GRAS_CHECK_SUBMODULE(PMC)
GRAS_CHECK_SUBMODULE(gr36)
GRAS_CHECK_SUBMODULE(grex)

list(APPEND CMAKE_MODULE_PATH ${GRAS_SOURCE_DIR}/modules/PMC/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${GRAS_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${GRAS_BINARY_DIR}/cmake/Modules)
include(GRASCommon)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DGRAS_DEBUG)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
    add_definitions(-Wunused)
    add_definitions(-fvisibility=hidden)
    add_definitions(-fvisibility-inlines-hidden)
endif()

if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x0501) #minimum version required is windows xp
    add_definitions(-DNOMINMAX) #disables stupidity and enables std::min and std::max
    add_definitions(-DBOOST_ALL_DYN_LINK) #setup boost auto-linking in msvc
endif(MSVC)

set(GRAS_ROOT "${CMAKE_INSTALL_PREFIX}")

########################################################################
# Component names for install rules
########################################################################
if (NOT DEFINED GRAS_COMP_DEVEL)
    set(GRAS_COMP_DEVEL "gras_devel")
endif()
if (NOT DEFINED GRAS_COMP_RUNTIME)
    set(GRAS_COMP_RUNTIME "gras_runtime")
endif()
if (NOT DEFINED GRAS_COMP_PYTHON)
    set(GRAS_COMP_PYTHON "gras_python")
endif()

set(PMC_COMP_DEVEL ${GRAS_COMP_DEVEL})
set(PMC_COMP_RUNTIME ${GRAS_COMP_RUNTIME})
set(PMC_COMP_PYTHON ${GRAS_COMP_PYTHON})

########################################################################
# Paths to public headers
########################################################################
list(APPEND GRAS_INCLUDE_DIRS ${GRAS_SOURCE_DIR}/modules/PMC/include)
list(APPEND GRAS_INCLUDE_DIRS ${GRAS_SOURCE_DIR}/include)

########################################################################
# Paths for python
########################################################################
list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/python)
list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/python/gras)
list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/python/gras/${CMAKE_BUILD_TYPE})

list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/modules/PMC/python)
list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/modules/PMC/python/PMC)
list(APPEND GRAS_PYTHON_DIRS ${GRAS_BINARY_DIR}/modules/PMC/python/PMC/${CMAKE_BUILD_TYPE})

########################################################################
# setup helpful submodule vars
########################################################################
list(APPEND GRAS_LIBRARIES gras pmc) #for submodule linking

#append gras deps for test code:
list(APPEND GR_TEST_TARGET_DEPS ${GRAS_LIBRARIES})
list(APPEND GR_TEST_PYTHON_DIRS ${GRAS_PYTHON_DIRS})

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(lib/)
add_subdirectory(include/gras)
add_subdirectory(modules/PMC)
add_subdirectory(python/gras)
add_subdirectory(tests)
add_subdirectory(query)
add_subdirectory(cmake/Modules)

########################################################################
# add gnuradio as sub-project
########################################################################
if(NOT GRDIR)
    set(GRDIR modules/gr36)
endif()
set(CMAKE_SOURCE_DIR ${GRAS_SOURCE_DIR}/${GRDIR})
set(CMAKE_BINARY_DIR ${GRAS_BINARY_DIR}/${GRDIR})

set(GRAS_FOUND TRUE)
#GRAS_INCLUDE_DIRS, GRAS_LIBRARIES set above
set(PMC_FOUND TRUE)
set(PMC_INCLUDE_DIRS ${GRAS_SOURCE_DIR}/modules/PMC/include)
set(PMC_LIBRARIES pmc)

#Theron isnt affected by boost thread issues.
#So we allow the gr black listed versions.
OPTION(ENABLE_BAD_BOOST "Enable known bad versions of Boost" ON)

set(CMAKE_PROJECT_NAME gnuradio) #for submodule vars

set(GR_MOAR_LIBRARIES ${GRAS_LIBRARIES})

add_subdirectory(${GRDIR})

########################################################################
# add GRAS to gnuradio cpack registry
# this must come after the submodule
########################################################################
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(GrPackage)

CPACK_SET(CPACK_COMPONENT_GROUP_GRAS_DESCRIPTION "GNU Radio Advanced Scheduler")

CPACK_COMPONENT("${GRAS_COMP_RUNTIME}"
    GROUP        "GRAS"
    DISPLAY_NAME "Runtime"
    DESCRIPTION  "Runtime"
)

CPACK_COMPONENT("${GRAS_COMP_DEVEL}"
    GROUP        "GRAS"
    DISPLAY_NAME "Development"
    DESCRIPTION  "C++ headers and import libraries"
)

CPACK_COMPONENT("${GRAS_COMP_PYTHON}"
    GROUP        "GRAS"
    DISPLAY_NAME "Python"
    DESCRIPTION  "Python modules for runtime"
    DEPENDS      "${GRAS_COMP_RUNTIME}"
)

########################################################################
# GREX as sub-project
########################################################################
list(APPEND CMAKE_MODULE_PATH ${GRAS_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${GRAS_BINARY_DIR}/cmake/Modules)
set(GRAS_ROOT ${GRAS_SOURCE_DIR})
list(APPEND GRAS_TEST_ENVIRONS "GRAS_ROOT=${GRAS_SOURCE_DIR}")
set(CMAKE_SOURCE_DIR ${GRAS_SOURCE_DIR}/modules/grex)
set(CMAKE_BINARY_DIR ${GRAS_BINARY_DIR}/modules/grex)

#call include to force local include precedence
include_directories(${GRAS_INCLUDE_DIRS})
include_directories(${PMC_INCLUDE_DIRS})

set(VOLK_FOUND ${ENABLE_VOLK})
set(VOLK_INCLUDE_DIRS
    ${GRAS_SOURCE_DIR}/${GRDIR}/volk/include
    ${GRAS_BINARY_DIR}/${GRDIR}/volk/include
)
if(MSVC)
    #add compatibility includes for stdint types
    list(APPEND VOLK_INCLUDE_DIRS ${GRAS_SOURCE_DIR}/${GRDIR}/volk/cmake/msvc)
endif()
set(VOLK_LIBRARIES volk)
if(ENABLE_VOLK)
    list(APPEND GR_TEST_TARGET_DEPS volk)
endif(ENABLE_VOLK)

#packet stuffs uses gr-digtal:
if(ENABLE_GR_DIGITAL)
    list(APPEND GR_TEST_PYTHON_DIRS
        ${GRAS_BINARY_DIR}/${GRDIR}/gnuradio-runtime/python #3.7
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-digital/python/digital #3.7
        ${GRAS_SOURCE_DIR}/${GRDIR}/gruel/src/python #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gruel/src/swig #3.6
        ${GRAS_SOURCE_DIR}/${GRDIR}/gnuradio-core/src/python #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gnuradio-core/src/lib/swig #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-digital/python #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-digital/swig #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-filter/python #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-filter/swig #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-analog/python #3.6
        ${GRAS_BINARY_DIR}/${GRDIR}/gr-analog/swig #3.6
    )
    list(APPEND GR_TEST_TARGET_DEPS gnuradio-digital gnuradio-filter gnuradio-fft gnuradio-analog)
endif(ENABLE_GR_DIGITAL)

list(APPEND GRAS_TEST_TARGET_DEPS "${GR_TEST_TARGET_DEPS}")
list(APPEND GRAS_TEST_LIBRARY_DIRS "${GR_TEST_LIBRARY_DIRS}")
list(APPEND GRAS_TEST_PYTHON_DIRS "${GR_TEST_PYTHON_DIRS}")

add_subdirectory(modules/grex)

########################################################################
# add GREX to gnuradio cpack registry
# this must come after the submodule
########################################################################
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(GrPackage)

CPACK_SET(CPACK_COMPONENT_GROUP_GREXTRAS_DESCRIPTION "GREX")

CPACK_COMPONENT("grex"
    GROUP        "GREX"
    DISPLAY_NAME "Runtime"
    DESCRIPTION  "Runtime"
    DEPENDS      "${GRAS_RUNTIME}"
)
