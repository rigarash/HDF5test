#  Copyright Ryo IGARASHI <rigarash@issp.u-tokyo.ac.jp> 2012.
#  Distributed under the Boost Software License, Version 1.0.
#      (See http://www.boost.org/LICENSE_1_0.txt)

# The minimum version is stricter than ALPS itself,
# in order to load the correct FindBoost.cmake when testing.
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(diag-parapack NONE)

######################################################################
# find ALPS Library
######################################################################
find_package(ALPS REQUIRED PATHS ${ALPS_ROOT_DIR} $ENV{ALPS_HOME} NO_SYSTEM_ENVIRONMENT_PATH)
message(STATUS "Found ALPS: ${ALPS_ROOT_DIR} (revision: ${ALPS_VERSION})")
include(${ALPS_USE_FILE})

######################################################################
# applications
######################################################################
# enable C and C++ compilers
enable_language(CXX)

# HDF5test
add_executable(HDF5test HDF5test.cc)
target_link_libraries(HDF5test ${ALPS_LIBRARIES})
