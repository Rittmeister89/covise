MACRO(USE_FMOD)
  FIND_PACKAGE(FMOD)
  IF ((NOT FMOD_FOUND) AND (${ARGC} LESS 1))
    USING_MESSAGE("Skipping because of missing FMOD")
    RETURN()
  ENDIF((NOT FMOD_FOUND) AND (${ARGC} LESS 1))
  IF(NOT FMOD_USED AND FMOD_FOUND)
    SET(FMOD_USED TRUE)
    INCLUDE_DIRECTORIES(${FMOD_INCLUDE_DIR})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${FMOD_LIBRARIES})
  ENDIF()
ENDMACRO(USE_FMOD)
