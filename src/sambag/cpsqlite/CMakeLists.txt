add_subdirectory(tests)
SET (sambag_cpsqlite
	DataBase.cpp
)
add_library(sambag_cpsqlite ${sambag_cpsqlite})
