# - Find ALVAR
# Find the ALVAR includes and library
#
#  ALVAR_INCLUDE_DIR - Where to find ALVAR includes
#  ALVAR_LIBRARIES   - List of libraries when using ALVAR
#  ALVAR_FOUND       - True if ALVAR was found

IF(ALVAR_INCLUDE_DIR)
  SET(ALVAR_FIND_QUIETLY TRUE)
ENDIF(ALVAR_INCLUDE_DIR)

FIND_PATH(ALVAR_INCLUDE_DIR "Tracker.h"
  PATHS
  $ENV{ALVAR_HOME}/include
  $ENV{EXTERNLIBS}/alvar/include
  ~/Library/Frameworks/include
  /Library/Frameworks/include
  /usr/local/include
  /usr/include
  /sw/include # Fink
  /opt/local/include # DarwinPorts
  /opt/csw/include # Blastwave
  /opt/include
  DOC "ALVAR - Headers"
)

SET(ALVAR_NAMES alvar200)
SET(ALVAR_DBG_NAMES alvar200d)
SET(ALVARPLATFORM_NAMES alvarplatform200)
SET(ALVARPLATFORM_DBG_NAMES alvarplatform200d)

FIND_LIBRARY(ALVAR_LIBRARY NAMES ${ALVAR_NAMES}
  PATHS
  $ENV{ALVAR_HOME}
  $ENV{EXTERNLIBS}/alvar
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64
  DOC "ALVAR - Library"
)

FIND_LIBRARY(ALVARPLATFORM_LIBRARY NAMES ${ALVARPLATFORM_NAMES}
  PATHS
  $ENV{ALVAR_HOME}
  $ENV{EXTERNLIBS}/alvar
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64
  DOC "ALVARPLATFORM - Library"
)

INCLUDE(FindPackageHandleStandardArgs)

IF(MSVC)
  # VisualStudio needs a debug version
  FIND_LIBRARY(ALVAR_LIBRARY_DEBUG NAMES ${ALVAR_DBG_NAMES}
    PATHS
    $ENV{ALVAR_HOME}/lib
    $ENV{EXTERNLIBS}/alvar/lib
    $ENV{EXTERNLIBS}/alvar/bin
    DOC "ALVAR - Library (Debug)"
  )
  FIND_LIBRARY(ALVARPLATFORM_LIBRARY_DEBUG NAMES ${ALVARPLATFORM_DBG_NAMES}
    PATHS
    $ENV{ALVAR_HOME}/lib
    $ENV{EXTERNLIBS}/alvar/lib
    $ENV{EXTERNLIBS}/alvar/bin
    DOC "ALVARPLATFORM - Library (Debug)"
  )
  
  
  IF(ALVAR_LIBRARY_DEBUG AND ALVAR_LIBRARY)
    SET(ALVAR_LIBRARIES optimized ${ALVAR_LIBRARY} debug ${ALVAR_LIBRARY_DEBUG} optimized ${ALVARPLATFORM_LIBRARY} debug ${ALVARPLATFORM_LIBRARY_DEBUG})
  ENDIF(ALVAR_LIBRARY_DEBUG AND ALVAR_LIBRARY)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALVAR DEFAULT_MSG ALVAR_LIBRARY ALVAR_LIBRARY_DEBUG ALVARPLATFORM_LIBRARY ALVARPLATFORM_LIBRARY_DEBUG ALVAR_INCLUDE_DIR)

  MARK_AS_ADVANCED(ALVAR_LIBRARY ALVAR_LIBRARY_DEBUG ALVARPLATFORM_LIBRARY ALVARPLATFORM_LIBRARY_DEBUG ALVAR_INCLUDE_DIR)
  
ELSE(MSVC)
  # rest of the world
  SET(ALVAR_LIBRARIES ${ALVAR_LIBRARY} ${ALVARPLATFORM_LIBRARY})

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALVAR DEFAULT_MSG ALVAR_LIBRARY ALVARPLATFORM_LIBRARY ALVAR_INCLUDE_DIR)
  
  MARK_AS_ADVANCED(ALVAR_LIBRARY ALVARPLATFORM_LIBRARY ALVAR_INCLUDE_DIR)
  
ENDIF(MSVC)

IF(ALVAR_FOUND)
  SET(ALVAR_INCLUDE_DIRS ${ALVAR_INCLUDE_DIR})
ENDIF(ALVAR_FOUND)