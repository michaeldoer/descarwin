PROJECT(ratp C)

ADD_DEFINITIONS(-DSCHED)

FIND_PACKAGE(X11 REQUIRED)

INCLUDE_DIRECTORIES(
  ${X11_INCLUDE_DIR}
  )

SET(SOURCES
  ratp.c
  )

LINK_LIBRARIES(${X11_LIBRARIES})

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
ADD_DEPENDENCIES(${PROJECT_NAME} yahsp_library-common yahsp_library-dae)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} yahsp_library)

SET_TARGET_PROPERTIES(
  ${PROJECT_NAME}
  PROPERTIES
  COMPILE_FLAGS "${OpenMP_C_FLAGS}"
  LINK_FLAGS "${OpenMP_C_FLAGS}"
  )
