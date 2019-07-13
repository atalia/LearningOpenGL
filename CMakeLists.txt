cmake_minimum_required(VERSION 3.14)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "CMAKE_CURRENT_BINARY_DIR" ${CMAKE_CURRENT_BINARY_DIR})


# 添加头文件
set(GLEW_H /usr/local/Cellar/glew/2.1.0/include)
set(GLFW_H /usr/local/Cellar/glfw/3.3/include)
set(GLM_H include/glm)
set(OPENCV_H /usr/local/Cellar/opencv/4.1.0_2/include/opencv4/opencv2)
set(OPENCV4_H /usr/local/Cellar/opencv/4.1.0_2/include/opencv4)
include_directories(${GLEW_H} ${GLFW_H} ${GLM_H} ${OPENCV4_H} ${OPENCV_H})

# 添加目标链接
set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3/lib/libglfw.3.dylib)
set(OPENCV_LINK /usr/local/Cellar/opencv/4.1.0_2/lib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# 执行编译命令
#set(SOURCE_FILES OpenGL/Hello\ Window/hellowindow.cpp)
#set(SOURCE_FILES OpenGL/Transformation/Transform.cpp OpenGL/Transformation/shader.cpp OpenGL/Transformation/shader.h)
set(SOURCE_DIR OpenGL/Transformation)
aux_source_directory(${SOURCE_DIR} SOURCE_LIST)
add_executable(LearningOpenGL ${SOURCE_LIST})

if (APPLE)
    target_link_libraries(LearningOpenGL "-framework OpenGL")
    find_package(OpenCV)
    target_link_libraries(LearningOpenGL ${OpenCV_LIBS})
endif()


