project(class8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

include_directories("../..")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories($(GLEW_INCLUDE_DIRS))
endif()

find_package(GLUT REQUIRED)

message("${GLEW_LIBRARIES}")
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(task8_1_3d_shapes task8_1_3d_shapes.cc)
target_link_libraries(task8_1_3d_shapes ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARY} -lGLU -lstdc++)

message("okok")
message("${GLEW_LIBRARIES}")
add_executable(task8_2_VBO  task8_2_VBO.cc)
target_link_libraries(task8_2_VBO ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY}  ${GLUT_LIBRARY} -lGLU -lstdc++)

add_executable(task8_3_texturing  task8_3_texturing.cc)
target_link_libraries(task8_3_texturing ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} -lGLU -lstdc++)

