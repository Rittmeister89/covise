MACRO(USE_GSOAP)
  IF ((NOT GSOAP_FOUND) AND (${ARGC} LESS 1))
    USING_MESSAGE("Skipping because of missing GSOAP")
    RETURN()
  ENDIF((NOT GSOAP_FOUND) AND (${ARGC} LESS 1))
  IF(NOT GSOAP_USED AND GSOAP_FOUND)
    SET(GSOAP_USED TRUE)
    ADD_DEFINITIONS(-DHAVE_GSOAP)
    INCLUDE_DIRECTORIES(${GSOAP_INCLUDE_DIR})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${GSOAP_LIBRARY})
  ENDIF()
ENDMACRO(USE_GSOAP)