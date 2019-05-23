cmake_minimum_required(VERSION 3.13.4)

project(MyProject)

######

#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)

#set(BOOST_ROOT "C:/all_with_prog/plugins/boost_1_64_0_b2")

#set(BOOST_LIBRARYDIR "C:/all_with_prog/plugins/boost_1_64_0_b2/lib64-msvc-9.0")

#set ( CMAKE_POSTFIX_PATH "${CMAKE_POSTFIX_PATH} -lboost_thread -lboost_system -lpthread" )


#include_directories ( ${CPPNETLIB_INCLUDE_DIRS} )

#target_link_libraries ( MyProject ${CPPNETLIB_LIBRARIES} )
######


# PostgreSQL
find_package(PostgreSQL REQUIRED)

link_libraries(${PostgreSQL_LIBRARIES})

include_directories(inc json/include ${PostgreSQL_INCLUDE_DIRS})

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(json)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lws2_32")


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