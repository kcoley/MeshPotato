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

#  Find Intel TBB headers and libraries.
#
#  This module defines
#  TBB_INCLUDE_DIRS - where to find tbb includes.
#  TBB_LIBRARIES    - List of libraries when using tbb.
#  TBB_FOUND        - True if tbb found.

# Look for the header file.
FIND_PATH( TBB_INCLUDE_DIR NAMES tbb/parallel_for.h)

# Look for the library.
FIND_LIBRARY( TBB_LIBRARY NAMES tbb)

# handle the QUIETLY and REQUIRED arguments and set TBB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( TBB DEFAULT_MSG TBB_LIBRARY TBB_INCLUDE_DIR)

# Copy the results to the output variables.
IF( TBB_FOUND)
    SET( TBB_LIBRARIES ${TBB_LIBRARY})
    SET( TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR})
ELSE()
    SET( TBB_LIBRARIES)
    SET( TBB_INCLUDE_DIRS)
ENDIF()

MARK_AS_ADVANCED( TBB_INCLUDE_DIR TBB_LIBRARY TBB_MALLOC_LIBRARY)
