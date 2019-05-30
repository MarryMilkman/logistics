cmake_minimum_required(VERSION 3.10.2)

project(MyProject)

if (NOT WIN32)

	set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/lib/postgresql/10/bin")

	find_package(Boost 1.65 REQUIRED)

	link_directories( ${Boost_LIBRARY_DIRS})

	include_directories(
		${boost_INCLUDE_DIRS}
	)

else ()

endif ()

# PostgreSQL
find_package(PostgreSQL REQUIRED)

link_libraries(${PostgreSQL_LIBRARIES})

#JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(json)


include_directories(
	inc
	json/include
	${PostgreSQL_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SRC
		src/main.cpp
		src/LTime.cpp
		
		src/LogisticsController/LogisticsController.cpp
		src/LogisticsController/LogisticsController_data.cpp
		src/LogisticsController/LogisticsController_logics.cpp

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

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")
	set(SRC_SERVER src/server/LServer_winsock.cpp)
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_thread -lboost_system -lpthread")
	set(SRC_SERVER src/server/LServer_boost_asio.cpp)

	add_executable(test_client trash/test_client.cpp)
endif ()


add_executable(log ${SRC} ${SRC_SERVER})