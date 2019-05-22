cmake_minimum_required(VERSION 3.10.2)

project(MyProject)

#set(Boost_USE_RELEASE_LIBS ON)

#set(Boost_USE_MULTITHREAD ON)

set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/lib/postgresql/10/bin")

find_package(PostgreSQL REQUIRED)

link_libraries(${PostgreSQL_LIBRARIES})

# add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

# set(BOOST_LIBRARY_DIR "/usr/lib/x86_64-linux-gnu/")

find_package(Boost 1.65 REQUIRED)


include_directories(
	inc 
	json/include 
	${PostgreSQL_INCLUDE_DIRS}
	${boost_INCLUDE_DIRS}
)

 link_directories( ${Boost_LIBRARY_DIRS})

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(json)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_thread -lboost_system -lpthread")

# set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lboost_thread -lboost_system -lpthread")

#-lws2_32

set(SRC
		src/main.cpp
		src/LServer.cpp
		src/LogisticsController.cpp
		src/Plast.cpp
		src/ObjPolygon.cpp
		src/ObjPolyline.cpp
		src/DotPolyline.cpp
		src/TetraTreePolygons.cpp
		src/geometry/Dot.cpp
		src/geometry/Geometry.cpp
		src/geometry/Line.cpp

		src/split.cpp
	)

add_executable(log ${SRC})

add_executable(test_client trash/test_client.cpp)




