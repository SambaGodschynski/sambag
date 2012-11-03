SET ( SAMBAG_TESTSOURCES
	sambag/xml/tests/XML2ObjTest.cpp
)

#unit tests
SET( UNIT_TESTSOURCE testApps/sambag_tests.cpp ${SAMBAG_TESTSOURCES})
add_executable(unit_tests ${UNIT_TESTSOURCE})
target_link_libraries (unit_tests ${SAMBAG_CLIBS})


