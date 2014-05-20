# Copyright (c) 2013 Esteban Tovagliari

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

#   Find OpenVDB headers and libraries.
#
#  This module defines
#  OpenVDB_INCLUDE_DIRS - where to find openvdb includes.
#  OpenVDB_INCLUDE_PYTHON_DIRS - where to find openvdb includes.
#  OpenVDB_LIBRARIES    - List of libraries when using openvdb.
#  OpenVDB_PYTHON_LIBRARIES    - List of python libraries when using openvdb.
#  OpenVDB_FOUND        - True if openvdb found.

MESSAGE("Loaded OpenVDB Module")
# Look for the header file.
FIND_PATH( OPENVDB_INCLUDE_DIR NAMES openvdb/openvdb.h)

# Look for the python header file.
FIND_PATH( OPENVDB_INCLUDE_PYTHON_DIR NAMES pyopenvdb.h)

# Look for the library.
FIND_LIBRARY( OPENVDB_LIBRARY NAMES openvdb)
FIND_LIBRARY( OPENVDB_PYTHON_LIBRARY NAMES pyopenvdb.so)

# handle the QUIETLY and REQUIRED arguments and set OPENVDB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( OPENVDB DEFAULT_MSG OPENVDB_LIBRARY OPENVDB_PYTHON_LIBRARY OPENVDB_INCLUDE_DIR OPENVDB_INCLUDE_PYTHON_DIR)

# compilation flags
SET( OPENVDB_COMPILE_FLAGS)

# Copy the results to the output variables.
IF( OPENVDB_FOUND)
    SET( OPENVDB_LIBRARIES ${OPENVDB_LIBRARY})
SET( OPENVDB_PYTHON_LIBRARIES ${OPENVDB_PYTHON_LIBRARY})
    SET( OPENVDB_INCLUDE_DIRS ${OPENVDB_INCLUDE_DIR})
    SET( OPENVDB_INCLUDE_PYTHON_DIRS ${OPENVDB_INCLUDE_PYTHON_DIR})
ELSE()
    SET( OPENVDB_LIBRARIES)
    SET( OPENVDB_PYTHON_LIBRARIES)
    SET( OPENVDB_INCLUDE_DIRS)
    SET( OPENVDB_INCLUDE_PYTHON_DIRS)
ENDIF()

MARK_AS_ADVANCED( OPENVDB_INCLUDE_DIR OPENVDB_INCLUDE_PYTHON_DIR OPENVDB_LIBRARY OPENVDB_PYTHON_LIBRARY)
