project(class5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

include_directories("../..")

set(DMRECON_SINGLE_VIEW_SOURCES
        task4-1_dmrecon_single_view.cc)
add_executable(task4-1_dmrecon_single_view ${DMRECON_SINGLE_VIEW_SOURCES})
target_link_libraries(task4-1_dmrecon_single_view mvs util core)


set(DMRECON_MULTIPLE_VIEWs_SOURCES
        task4-2_dmrecon_multi_views.cc)
add_executable(task4-2_dmrecon_multi_views ${DMRECON_MULTIPLE_VIEWs_SOURCES})
target_link_libraries(task4-2_dmrecon_multi_views mvs util core)


set(SCENE2PSET_SINGLE_VIEW_SOURCES
        task4-1_scene2pset_single_view.cc)
add_executable(task4-1_scene2pset_single_view ${SCENE2PSET_SINGLE_VIEW_SOURCES})
target_link_libraries(task4-1_scene2pset_single_view mvs util core)


set(SCENE2PSET_MULTI_VIEWS_SOURCES
        task4-2_scene2pset_multi_views.cc)
add_executable(task4-2_scene2pset_multi_views ${SCENE2PSET_MULTI_VIEWS_SOURCES})
target_link_libraries(task4-2_scene2pset_multi_views mvs util core)