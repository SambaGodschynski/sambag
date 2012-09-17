add_executable(unit_test sambag_tests.cpp)
target_link_libraries (unit_test  ${UNITTEST_LIBS})
target_link_libraries (unit_test  ${CPPUNIT_LIBRARY_DIRS}/${LIBCPPUNIT})
#target_link_libraries (unit_test  sambag_com_tests)
