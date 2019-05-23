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
}




// MARK: - init location of dot polyline
// 1) initLocation
// 2) _get_start_polygon
// 3) _initLocation_basePolygon
// 4) _initLocation_tt_polygons


void			DotPolyline::initLocation(ObjPolygon *mostPossiblePolygon) {
	RelativePosition	answer;
	ObjPolygon			*startPolygon = this->_get_start_polygon(mostPossiblePolygon);

	this->_initLocation_basePolygon(startPolygon);
}

	// find first polygon, that contains current dot (move up)
ObjPolygon		*DotPolyline::_get_start_polygon(ObjPolygon *mostPossiblePolygon) {
	if (!mostPossiblePolygon)
		return 0;
	RelativePosition	answer;
	ObjPolygon			*r_polygon = mostPossiblePolygon;

	answer = Geometry::whereIs_dotInPolygon(this->dot, r_polygon);
	while (answer == RelativePosition::Outside && r_polygon) {
		r_polygon = r_polygon->parent;
		answer = Geometry::whereIs_dotInPolygon(this->dot, r_polygon);
	}
	this->position = answer;
	return r_polygon;
}

	// add basePolygon to list contact aryas and check plast (recursive)
void			DotPolyline::_initLocation_basePolygon(ObjPolygon *basePolygon) {
	std::vector<ObjPolygon *>	nestedPolygons;
	Plast						*plast;
	RelativePosition			answer_position;

	if (this->position != RelativePosition::Border)
		this->listOf_contactAreas.clear();
	this->listOf_contactAreas.push_back(basePolygon);
	this->currentArea = basePolygon;
	if (!basePolygon)
		plast = Plast::getPlast(basePolygon);
	else
		plast = basePolygon->plast;
	if (!plast->empty()) {
			// change position of pDot
		this->_initLocation_tt_polygons(plast->_tt_polygons, nestedPolygons);
		for (ObjPolygon *check_polygon : nestedPolygons)
			this->_initLocation_basePolygon(check_polygon);
	}
}

	// recursiv serch contact polygons in tt_poltgons
RelativePosition	DotPolyline::_initLocation_tt_polygons(
						TetraTreePolygons *tt_polygons,
						std::vector<ObjPolygon *>	&nestedPolygons)
{
	if (!tt_polygons || !tt_polygons->polygon)
		return RelativePosition::Outside;

	std::vector<TetraTreePolygons *>	list_direction;
	ObjPolygon							*check_polygon = tt_polygons->polygon;
	RelativePosition					answer;

	answer = Geometry::whereIs_dotInPolygon(this->dot, check_polygon);
	// std::cerr << "Dot " << this->dot.x << ":" << this->dot.y << ", " << check_polygon;
	// std::cerr << "Answer: " << answer << "\n";
	if (answer == RelativePosition::Border) {
		if (this->position != answer)
			this->listOf_contactAreas.clear();
		this->position = RelativePosition::Border;
		nestedPolygons.insert(nestedPolygons.end(), check_polygon);
	}
	if (answer == RelativePosition::Inside) {
		if (this->position != RelativePosition::Border) {
			this->position = RelativePosition::Inside;
			nestedPolygons.clear();
		}
		nestedPolygons.push_back(check_polygon);
		return answer;
	}
	list_direction = TetraTreePolygons::createSortListFromChildOf_tt_polygons_withDestination(tt_polygons, this->dot);
	
	// std::cerr << list_direction.size() << "  : " << tt_polygons->polygon;
	for (TetraTreePolygons *check_tt_polygons : list_direction) {
		RelativePosition	local_answer;

		local_answer = this->_initLocation_tt_polygons(check_tt_polygons, nestedPolygons);
		if (local_answer == RelativePosition::Inside) {
			return RelativePosition::Inside;
		}
	}
	if (this->position == RelativePosition::Border)
		return this->position;
	return RelativePosition::Outside;
}

