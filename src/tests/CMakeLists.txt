set(TEST_BLOOM test_bloom.cpp)
add_executable(test_bloom ${TEST_BLOOM})
target_link_libraries(test_bloom LibreSSL::TLS)

set(TEST_HRW test_hrw.cpp)
add_executable(test_hrw ${TEST_HRW})
target_link_libraries(test_hrw LibreSSL::TLS)

