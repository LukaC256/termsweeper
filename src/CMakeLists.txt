add_library(TReadline INTERFACE IMPORTED)
target_link_libraries(TReadline INTERFACE readline)


add_executable(termsweeper main.cpp
			map.cpp)
target_link_libraries(termsweeper TReadline)

install(TARGETS termsweeper RUNTIME DESTINATION bin)
