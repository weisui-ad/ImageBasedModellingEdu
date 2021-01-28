project(class3)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

include_directories("../..")

# test triangle
set(TRIANGLE_FILE
        task2-1_test_triangle.cc )
add_executable(task2-1_test_triangle ${TRIANGLE_FILE})
target_link_libraries(task2-1_test_triangle sfm util core)

#test p3p
set(P3P_FILE
        task2-2_test_p3p_kneip.cc)
add_executable(task2-2_test_p3p_kneip ${P3P_FILE})
target_link_libraries(task2-2_test_p3p_kneip sfm util core features)

set(P3P_FILE
        task2-2_test_p3p_ransac.cc)
add_executable(task2-2_test_p3p_ransac ${P3P_FILE})
target_link_libraries(task2-2_test_p3p_ransac sfm util core features)


#test lm optimize
set(LM_OPTIMIZE_FILES
        task2-3_test_lm_optimize.cc)
add_executable(task2-3_test_lm_optimize ${LM_OPTIMIZE_FILES})
target_link_libraries(task2-3_test_lm_optimize sfm util core features)

#test jacobian
set(JACOBIAN_FILE task2-4_test_jacobian.cc)
add_executable(task2-4_test_jacobian ${JACOBIAN_FILE})
target_link_libraries(task2-4_test_jacobian sfm util core)

# test bundle adjustment
set(BA_FILES
        task2-5_test_bundle_adjustment.cc)
add_executable(task2-5_test_ba ${BA_FILES})
target_link_libraries(task2-5_test_ba sfm util core features)

