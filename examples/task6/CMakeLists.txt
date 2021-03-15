project(class6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

include_directories("../..")


set(SURFACE_RECONSTRUCTION_SOURCES
        task6-1_surface_reconstruction.cc)

set(MESH_CLEAN_SOURCES
        task6-2_meshclean.cc)

add_executable(task6-1_surface_reconstruction ${SURFACE_RECONSTRUCTION_SOURCES})
target_link_libraries(task6-1_surface_reconstruction mvs util core surface)

add_executable(task6-2_meshclean ${MESH_CLEAN_SOURCES})
target_link_libraries(task6-2_meshclean mvs util core surface)