#include "Plast.hpp"
#include "TetraTreePolygons.hpp"

#include "LogisticsController.hpp"
#include "ObjPolygon.hpp"

Plast::Plast() {
	this->_tt_polygons = 0;
}

Plast::Plast(TetraTreePolygons *tt_polygons) {
	this->_tt_polygons = tt_polygons;
}

Plast::~Plast() {
	if (this->_tt_polygons) {
		this->_tt_polygons->deleteBranches_and_polygon();
		delete this->_tt_polygons;
	}
}

std::vector<ObjPolygon *>	Plast::addNewPolygon_toPlast(
							ObjPolygon *polygon,
							ObjPolygon *parentPolygon,
							IntersectionType & answer)
{
	std::vector<ObjPolygon *>	answer_FromCheckIntersection_polygons;

	std::cerr << "Plast::addNewPolygon_toPlast\n";
	polygon->parent = parentPolygon;
	if (!this->_tt_polygons) {
		answer = IntersectionType::Intersection_false;
		this->_tt_polygons = new TetraTreePolygons(polygon);
		this->_update_max_min_coord(polygon);
		return std::vector<ObjPolygon *>();
	}
	if (answer == IntersectionType::PolygonInclude)
		answer = IntersectionType::Intersection_false;
	answer_FromCheckIntersection_polygons = LogisticsController::checkIntersection(polygon, this->_tt_polygons, &answer);
	std::cerr << "Answer: " << answer << "\n";
	if (answer == IntersectionType::Intersection_false) {
		this->_tt_polygons->addNewPolygon(polygon);
		this->_update_max_min_coord(polygon);
		return std::vector<ObjPolygon *>();
	}
	if (answer == IntersectionType::PolygonInclude) {
		Plast	*next_plast = answer_FromCheckIntersection_polygons[0]->plast;
		return next_plast->addNewPolygon_toPlast(polygon, answer_FromCheckIntersection_polygons[0], answer);
	}
	if (answer == IntersectionType::PolygonUpper) {
		TetraTreePolygons	*new_tt_polygons = new TetraTreePolygons();
		TetraTreePolygons	*trash_tt_polygon = this->_tt_polygons;

		TetraTreePolygons::setUpdate_tt_polygons_without(this->_tt_polygons, answer_FromCheckIntersection_polygons, new_tt_polygons);
		if (parentPolygon)
			parentPolygon->plast->_tt_polygons = new_tt_polygons;
		else
			this->_tt_polygons = new_tt_polygons;
		for (ObjPolygon *ans_polygon : answer_FromCheckIntersection_polygons)
			ans_polygon->parent = polygon;
		trash_tt_polygon->deleteBranches();
		delete trash_tt_polygon;
		polygon->plast->_tt_polygons = TetraTreePolygons::createNew_tt_polygons_with(answer_FromCheckIntersection_polygons);
		this->_tt_polygons->addNewPolygon(polygon);
		this->_update_max_min_coord(polygon);
		return std::vector<ObjPolygon *>();
	}
	if (answer == IntersectionType::Intersection_true) {
		std::cerr << "Intersection polygons " << polygon->data.id << " and |";
		for (ObjPolygon *polygon_intersect : answer_FromCheckIntersection_polygons)
			std::cerr << polygon_intersect->data.id << "| ";
		std::cerr << "\n";
		return answer_FromCheckIntersection_polygons;
	}
	if (answer == IntersectionType::FullMatch) {
		std::cerr << "Full macth polygons " << polygon->data.id << " and "
					<< answer_FromCheckIntersection_polygons[0]->data.id << "\n"; 
		return answer_FromCheckIntersection_polygons;
	}
	return std::vector<ObjPolygon *>();
}

void		Plast::_update_max_min_coord(ObjPolygon *polygon) {
	if (polygon->data.max_x > this->max_x)
		this->max_x = polygon->data.max_x;
	if (polygon->data.max_y > this->max_y)
		this->max_y = polygon->data.max_y;
	if (polygon->data.min_x < this->min_x)
		this->min_x = polygon->data.min_x;
	if (polygon->data.min_y < this->min_y)
		this->min_y = polygon->data.min_y;
}


	// get plast of polygon (where is locate polygon)
	// if !polygon return LogisticsController plast
Plast			*Plast::getPlast(ObjPolygon *polygon) {
	Plast	*r_plast;

	if (!polygon || (polygon && !polygon->parent)) {
		LogisticsController	&controller = LogisticsController::getInstance();

		r_plast = controller.get_plast();
	}
	else {
		r_plast = polygon->parent->plast;
	}
	return r_plast;
}


Plast			*Plast::getOveralPlast(ObjPolygon *p1, ObjPolygon *p2) {
	LogisticsController		&controller = LogisticsController::getInstance();
	Plast					*zeroPlast = controller.get_plast();
	ObjPolygon				*check_p1;
	ObjPolygon				*check_p2;
	ObjPolygon				*overalPolygon = 0;

	if (!p1 || !p2)
		return zeroPlast;
	check_p1 = p1->parent;
	check_p2 = p2->parent;
	while (check_p1) {
		check_p2 = p2;
		while (check_p2) {
			if (check_p2->parent == check_p1->parent) {
				overalPolygon = check_p2->parent;
				break ;
			}
			check_p2 = check_p2->parent;
		}
		if (overalPolygon)
			break ;
		check_p1 = check_p1->parent;
	}
	if (!overalPolygon)
		return zeroPlast;
	return overalPolygon->plast;
}


bool		Plast::is_dot_can_be_in_plast(Dot dot, Plast *plast) {
	if (!plast)
		return false;

	if (dot.x > plast->max_x || dot.x < plast->min_x || dot.y > plast->max_y || dot.y < plast->min_y)
		return false;
	return true;
}


bool		Plast::empty() {
	if (!this || !this->_tt_polygons)
		return true;
	return false;
}