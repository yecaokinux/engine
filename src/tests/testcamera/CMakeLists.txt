project(testcamera)
set(SRCS
	FrustumEntity.h
	TestCamera.h TestCamera.cpp
)
engine_add_executable(TARGET ${PROJECT_NAME} SRCS ${SRCS} WINDOWED)
target_link_libraries(${PROJECT_NAME} testcore)
