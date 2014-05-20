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

# Find OpenEXR headers and libraries.
#
#  This module defines
# OPENEXR_INCLUDE_DIRS - where to find OPENEXR includes.
# OPENEXR_LIBRARIES    - List of libraries when using OPENEXR.
# OPENEXR_FOUND        - True if OPENEXR found.

# Look for the header file.
FIND_PATH( OPENEXR_INCLUDE_DIR NAMES OpenEXR/ImfHeader.h)

# Look for the libraries.
FIND_LIBRARY( OPENEXR_IMF_LIBRARY NAMES IlmImf)
FIND_LIBRARY( OPENEXR_THREAD_LIBRARY NAMES IlmThread)

# handle the QUIETLY and REQUIRED arguments and set OPENEXR_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( OPENEXR DEFAULT_MSG  OPENEXR_IMF_LIBRARY
                                                        OPENEXR_THREAD_LIBRARY
                                                        OPENEXR_INCLUDE_DIR)

# Copy the results to the output variables.
IF( OPENEXR_FOUND)
    SET( OPENEXR_LIBRARIES  ${OPENEXR_IMF_LIBRARY} ${OPENEXR_THREAD_LIBRARY})
    SET( OPENEXR_INCLUDE_DIRS ${OPENEXR_INCLUDE_DIR})
ELSE( OPENEXR_FOUND)
    SET( OPENEXR_LIBRARIES)
    SET( OPENEXR_INCLUDE_DIRS)
ENDIF( OPENEXR_FOUND)

MARK_AS_ADVANCED( OPENEXR_IMF_LIBRARY
                  OPENEXR_INCLUDE_DIR)
