#include "LogisticsController.hpp"

#include "ObjPolyline.hpp"

#include "Plast.hpp"


LogisticsController::LogisticsController() {
	this->_plast = new Plast();
	this->_polyline = new ObjPolyline();
}

LogisticsController::~LogisticsController() {
	std::cerr << "LogisticsController::~LogisticsController()\n";
	if (this->_polyline)
		delete this->_polyline;
	if (this->_plast)
		delete this->_plast;
}

LogisticsController	&LogisticsController::getInstance() {
	static LogisticsController	controller;

	return controller;
}