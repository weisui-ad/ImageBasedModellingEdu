project(class8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

include_directories("../..")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Glew REQUIRED)
find_package(GLUT REQUIRED)

message("${GLEW_LIBRARIES}")
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(task8_1_3d_shapes task8_1_3d_shapes.cc)
target_link_libraries(task8_1_3d_shapes ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} glfw )

add_executable(task8_2_VBO  task8_2_VBO.cc)
target_link_libraries(task8_2_VBO ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY}  ${GLUT_LIBRARY}  glfw glut)

#add_executable(task8_3_trackball  task8_3_trackball.cc)
#target_link_libraries(task8_3_trackball ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} glfw)
#
#add_executable(task8_4_texturing  task8_4_texturing.cc)
#target_link_libraries(task8_4_texturing ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} glfw)

