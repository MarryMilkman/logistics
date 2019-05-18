#include "DotPolyline.hpp"

#include "geometry/Geometry.hpp"
#include "ObjPolygon.hpp"
#include "Plast.hpp"
#include "TetraTreePolygons.hpp"

DotPolyline::DotPolyline() {
	this->dot = Dot();
	this->current_time = "unknown";
	this->isIntersect = false;
	this->position = RelativePosition::Outside;

	this->currentArea = 0;
	this->previousArea = 0;
	this->nextArea = 0;

	this->distance = 0;
}

DotPolyline::DotPolyline(Dot dot, std::string current_time) {
	this->dot = dot;
	this->current_time = current_time;
	this->isIntersect = false;
	this->position = RelativePosition::Outside;

	this->currentArea = 0;
	this->previousArea = 0;
	this->nextArea = 0;

	this->distance = 0;
}

DotPolyline::DotPolyline(DotPolyline const & ref) {
	*this = ref;
}

DotPolyline::~DotPolyline() {
}

DotPolyline		&DotPolyline::operator=(DotPolyline const & ref) {
	this->dot = ref.dot;
	this->current_time = ref.current_time;
	this->isIntersect = ref.isIntersect;
	this->position = ref.position;
	this->listOf_contactAreas = ref.listOf_contactAreas;

	this->currentArea = ref.currentArea;
	this->previousArea = ref.previousArea;
	this->nextArea = ref.nextArea;
	this->id = ref.id;
	return *this;
}




// MARK: - init location of dot polyline
// 1) initLocation
// 2) initLocation_plast
// 3) _initLocation_tt_polygons

	// check most possible polygon, witch can be DotPolyline
	// if no exist - check parent of most poss polygon
void			DotPolyline::initLocation(ObjPolygon *mostPossiblePolygon) {
	Plast				*currentPlast = Plast::getPlast(mostPossiblePolygon);
	RelativePosition	answer;

	if (!mostPossiblePolygon) {
		answer = this->initLocation_plast(currentPlast);
		if (answer == RelativePosition::Outside)
			this->listOf_contactAreas.insert(this->listOf_contactAreas.end(), 0);
		this->position = answer;
		return ;
	}
	answer = Geometry::whereIs_dotInPolygon(this->dot, mostPossiblePolygon);
	if (answer == RelativePosition::Outside || answer == RelativePosition::Border) {
		answer = this->initLocation_plast(currentPlast);
		if (answer == RelativePosition::Outside)
			this->initLocation(mostPossiblePolygon->parent);
		else
			this->position = answer;
	}
	if (answer == RelativePosition::Inside) {
		this->listOf_contactAreas.insert(this->listOf_contactAreas.end(), mostPossiblePolygon);
		if (mostPossiblePolygon->plast)
			this->initLocation_plast(mostPossiblePolygon->plast);
		else
			this->position = answer;
	}
}

	// check plast
RelativePosition	DotPolyline::initLocation_plast(Plast *plast) {
	if (!plast)
		return RelativePosition::Outside;

	TetraTreePolygons			*tt_polygons = plast->_tt_polygons;

	return this->_initLocation_tt_polygons(tt_polygons);
}

	// check tt_polygons: check self polygon, then check next tt_polygon in right direction
RelativePosition	DotPolyline::_initLocation_tt_polygons(TetraTreePolygons *tt_polygons) {
	if (!tt_polygons)
		return RelativePosition::Outside;

	std::vector<TetraTreePolygons *>	list_direction;
	std::vector<ObjPolygon *>			new_listOf_contactAreas;
	ObjPolygon							*check_polygon = tt_polygons->polygon;
	RelativePosition					answer;


	answer = Geometry::whereIs_dotInPolygon(this->dot, check_polygon);
	if (answer == RelativePosition::Border) {
		this->listOf_contactAreas.insert(this->listOf_contactAreas.end(), check_polygon);
	}
	if (answer == RelativePosition::Inside) {
		this->listOf_contactAreas = std::vector<ObjPolygon *>();
		this->listOf_contactAreas.insert(this->listOf_contactAreas.end(), check_polygon);
		this->initLocation_plast(check_polygon->plast);
		return answer;
	}
	list_direction = TetraTreePolygons::createSortListFromChildOf_tt_polygons_withDestination(tt_polygons, this->dot);
	for (TetraTreePolygons *check_tt_polygons : list_direction) {
		RelativePosition	local_answer;

		local_answer = this->_initLocation_tt_polygons(check_tt_polygons);
		if (local_answer == RelativePosition::Inside)
			return RelativePosition::Inside;
		if (local_answer == RelativePosition::Border)
			answer = RelativePosition::Border;
	}
	return answer;
}
