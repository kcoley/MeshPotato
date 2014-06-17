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

#  Find Alembic library
#
#  This module defines
#  ALEMBIC_INCLUDE_DIRS, where to find samplerate.h, Set when
#                        ALEMBIC_INCLUDE_DIR is found.
#  ALEMBIC_LIBRARIES, libraries to link against to use Samplerate.
#  ALEMBIC_ROOT_DIR, The base directory to search for Samplerate.
#                    This can also be an environment variable.
#  ALEMBIC_FOUND, If false, do not try to use Samplerate.
#

# If ALEMBIC_ROOT_DIR was defined in the environment, use it.
IF( NOT ALEMBIC_ROOT_DIR AND NOT $ENV{ALEMBIC_ROOT_DIR} STREQUAL "")
    SET( ALEMBIC_ROOT_DIR $ENV{ALEMBIC_ROOT_DIR})
ENDIF()

SET( _alembic_SEARCH_DIRS
      ${ALEMBIC_ROOT_DIR}
      /usr/local
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
    )

FIND_PATH( ALEMBIC_INCLUDE_DIR NAMES Alembic/Abc/All.h HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES include)

#libAbcWFObjConvert.so
#libAlembicAbcOpenGL.so

#libAlembicAbc.so
#libAlembicAbcCollection.so
#libAlembicAbcCoreAbstract.so
#libAlembicAbcCoreHDF5.so
#libAlembicAbcGeom.so
#libAlembicAbcMaterial.so
#libAlembicUtil.so
#libAlembicAbcCoreFactory.so

#libAlembicOgawa.so
#libAlembicAbcCoreOgawa.so


FIND_LIBRARY( ALEMBIC_ABC_LIBRARY                   NAMES AlembicAbc HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCCOLLECTION_LIBRARY         NAMES AlembicAbcCollection HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCCORE_ABS_LIBRARY           NAMES AlembicAbcCoreAbstract HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCCORE_HDF5_LIBRARY          NAMES AlembicAbcCoreHDF5 HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCCORE_OGAWA_LIBRARY         NAMES AlembicAbcCoreOgawa HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_OGAWA_LIBRARY                 NAMES AlembicOgawa HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCCORE_ABS_FACTORY_LIBRARY   NAMES AlembicAbcCoreFactory HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCGEOM_LIBRARY               NAMES AlembicAbcGeom HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCMATERIAL_LIBRARY           NAMES AlembicAbcMaterial HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)
FIND_LIBRARY( ALEMBIC_ABCUTIL_LIBRARY               NAMES AlembicUtil HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)

FIND_LIBRARY( ALEMBIC_ABCGL_LIBRARY                 NAMES AlembicAbcOpenGL HINTS ${_alembic_SEARCH_DIRS} PATH_SUFFIXES lib64 lib)

# handle the QUIETLY and REQUIRED arguments and set ALEMBIC_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( ALEMBIC DEFAULT_MSG ALEMBIC_ABC_LIBRARY ALEMBIC_INCLUDE_DIR)

IF( ALEMBIC_FOUND)
    SET( ALEMBIC_LIBRARIES  ${ALEMBIC_ABC_LIBRARY}
                            ${ALEMBIC_ABCCOLLECTION_LIBRARY}
                            ${ALEMBIC_ABCCORE_ABS_LIBRARY}
                            ${ALEMBIC_ABCCORE_HDF5_LIBRARY}
                            ${ALEMBIC_ABCCORE_OGAWA_LIBRARY}
                            ${ALEMBIC_OGAWA_LIBRARY}
                            ${ALEMBIC_ABCCORE_ABS_FACTORY_LIBRARY}
                            ${ALEMBIC_ABCGEOM_LIBRARY}
                            ${ALEMBIC_ABCMATERIAL_LIBRARY}
                            ${ALEMBIC_ABCUTIL_LIBRARY}
                            )

    SET( ALEMBIC_INCLUDE_DIRS ${ALEMBIC_INCLUDE_DIR})
ENDIF()

MARK_AS_ADVANCED(   ALEMBIC_INCLUDE_DIR
                    ALEMBIC_ABC_LIBRARY
                    ALEMBIC_ABCCOLLECTION_LIBRARY
                    ALEMBIC_ABCCORE_ABS_LIBRARY
                    ALEMBIC_ABCCORE_HDF5_LIBRARY
                    ALEMBIC_ABCGEOM_LIBRARY
                    ALEMBIC_ABCMATERIAL_LIBRARY
                    ALEMBIC_ABCUTIL_LIBRARY
                    )
