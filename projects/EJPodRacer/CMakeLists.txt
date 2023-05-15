#--------------------------------------------------------------------------
# spacegame project
#--------------------------------------------------------------------------

PROJECT(EJPodRacer)
FILE(GLOB project_headers code/*.h)
FILE(GLOB project_sources code/*.cc)

SET(files_project ${project_headers} ${project_sources})
SET(files_proto)
#flat_compile(
#	test.fbs
#	# add more flatbuffer files here
#) 

SOURCE_GROUP("EJPodRacer" FILES ${files_project})

ADD_EXECUTABLE(EJPodRacer ${files_project} ${files_proto})
target_include_directories(EJPodRacer PRIVATE "${CMAKE_BINARY_DIR}/generated/flat")

TARGET_LINK_LIBRARIES(EJPodRacer core render)
ADD_DEPENDENCIES(EJPodRacer core render)

IF(MSVC)
    set_property(TARGET EJPodRacer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF()