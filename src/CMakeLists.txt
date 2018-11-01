include_directories(
	${GLEW_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	../include
)

add_executable(${EXE_NAME}
	misc.cpp
	myClock.cpp
	myWindow.cpp
	waveFunc.cpp
)

target_link_libraries(${EXE_NAME}
	${GLEW_LIBRARIES}
	${GLFW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_target_properties(${EXE_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${EXE_NAME} DESTINATION ${INSTALL_DIR})
install(FILES ${SHADERS} DESTINATION ${INSTALL_DIR})
