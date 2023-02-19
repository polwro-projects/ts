cmake_minimum_required(VERSION 3.13)
project("TSP")

# UNIX-specific definitions
if(UNIX)
	# Add debug symbols
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

	# Add important things for Linux
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(SOURCES
	"src/main.cpp"
	"src/utils/tokenizer.cpp"
	"src/io/basereader.cpp"
	"src/tsp/algorithm/algorithm.cpp"
	"src/application.cpp"
	"src/tsp/algorithm/ts.cpp"
	"src/utils/os/memory.cpp"
)

add_executable(TSP ${SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
	include
)

# Force the compiler to use C++20
if(CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()