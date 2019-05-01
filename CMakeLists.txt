cmake_minimum_required(VERSION 3.13.4)

project(MyProject)

# PostgreSQL
find_package(PostgreSQL REQUIRED)
#target_include_directories(MyTarget PRIVATE ${PostgreSQL_INCLUDE_DIRS})
link_libraries(${PostgreSQL_LIBRARIES})

include_directories(inc json/include ${PostgreSQL_INCLUDE_DIRS})

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(json)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC
		src/main.cpp
		src/LogisticsController.cpp
		src/Plast.cpp
		src/ObjPolygon.cpp
		src/ObjPolyline.cpp
		src/DotPolyline.cpp
		src/TetraTreePolygons.cpp
		src/geometry/Dot.cpp
		src/geometry/Geometry.cpp
		src/geometry/Line.cpp
	)

add_executable(log ${SRC})