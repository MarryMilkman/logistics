#include "LogisticsController.hpp"

#include "ObjPolygon.hpp"
#include "ObjPolyline.hpp"
#include "DotPolyline.hpp"

#include "TetraTreePolygons.hpp"
#include "Plast.hpp"

#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"



// MARK: -  function for check Intersection check_polygon with ather arya in TetraTreePolygons
// 1) checkIntersection (static)
// 2) _intersectionPolygons (static)
// 3) _checkPartsOfOneArya_and_link_otherParts (static)

	// return std::vector<ObjPolygon *> - pull of polygons,
	// which have some relationship with check ObjPolygon (intersection||include||upper||full match)
	// if relationship were not found, then return empty std::vector
std::vector<ObjPolygon *>	LogisticsController::checkIntersection(
										ObjPolygon *check_polygon,
										TetraTreePolygons *check_tt_polygon,
										IntersectionType *answer)
{
	IntersectionType					answ_from_intersection;
	std::vector<ObjPolygon *>			answerPolygons;
	std::vector<TetraTreePolygons *>	sortList_tt_polygons;
	ObjPolygon							*polygon_from_check_tt_polygon = check_tt_polygon->polygon;

	std::cerr << "LogisticsController::checkIntersection\n";
	answ_from_intersection = LogisticsController::_intersectionPolygons(check_polygon, polygon_from_check_tt_polygon);
	if (answ_from_intersection == IntersectionType::FullMatch) {
		*answer = answ_from_intersection;
		std::vector<ObjPolygon *>	r_vector;

		r_vector.insert(r_vector.end(), polygon_from_check_tt_polygon);
		return r_vector;
	}
	if (answ_from_intersection == IntersectionType::PolygonInclude) {
		*answer = answ_from_intersection;
		std::vector<ObjPolygon *>	r_vector;

		r_vector.insert(r_vector.end(), polygon_from_check_tt_polygon);
		return r_vector;
	}
	if (answ_from_intersection == IntersectionType::Intersection_true) {
		*answer = answ_from_intersection;
		answerPolygons.insert(answerPolygons.end(), polygon_from_check_tt_polygon);
	}
	if (answ_from_intersection == IntersectionType::PolygonUpper && *answer != IntersectionType::Intersection_true) {
		*answer = answ_from_intersection;
		answerPolygons.insert(answerPolygons.end(), polygon_from_check_tt_polygon);
	}
	if (*answer != IntersectionType::PolygonUpper && *answer != IntersectionType::Intersection_true
			&& *answer != IntersectionType::PolygonInclude && *answer != IntersectionType::FullMatch)
		*answer = answ_from_intersection;
	// Can be ERROR!
	sortList_tt_polygons = TetraTreePolygons::createSortListFromChildOf_tt_polygons_withDestination(check_tt_polygon, check_polygon->data.center);
	for (TetraTreePolygons *new_check_tt_polygons : sortList_tt_polygons) {
		if (!new_check_tt_polygons)
			continue;
		std::vector<ObjPolygon *>	new_answerPolygons;

		new_answerPolygons = LogisticsController::checkIntersection(check_polygon, new_check_tt_polygons, answer);
		if (*answer == IntersectionType::FullMatch) {
			return new_answerPolygons;
		}
		if (*answer == IntersectionType::Intersection_true) {
			if (answ_from_intersection != IntersectionType::Intersection_true)
				answerPolygons = std::vector<ObjPolygon *>();
			answ_from_intersection = *answer;
			//
			answerPolygons = LogisticsController::_joinList(answerPolygons, new_answerPolygons);
		}
		if (*answer == IntersectionType::PolygonUpper && *answer != IntersectionType::Intersection_true) {
			answ_from_intersection = *answer;
			answerPolygons = LogisticsController::_joinList(answerPolygons, new_answerPolygons);
		}
		if (*answer == IntersectionType::PolygonInclude)
			return new_answerPolygons;
	}
	return answerPolygons;
}

	// _intersectionPolygons - check relationships with tow polygons
	// return answer in relation to check_polygon
IntersectionType		LogisticsController::_intersectionPolygons(
											ObjPolygon *check_polygon,
											ObjPolygon *polygon_from_tt)
{
	IntersectionType	status = IntersectionType::FullMatch;
	std::vector<Dot>	&arr_dot = check_polygon->data.arr_dot;

	std::cerr << "LogisticsController::_intersectionPolygons\n";
	if (!ObjPolygon::is_squareOfPolygonsIntersect(check_polygon, polygon_from_tt)) {
		return IntersectionType::Intersection_false;
	}

	for (Dot dot : arr_dot) {
		
		RelativePosition answer = Geometry::whereIs_dotInPolygon(dot, polygon_from_tt);
		// 0 - out of polygon; 1 - in the border; 2 - inside
		if (answer == RelativePosition::Outside) {
			if (status == IntersectionType::PolygonInclude)
				return IntersectionType::Intersection_true;
			status = IntersectionType::Intersection_false;
		}
		if (answer == RelativePosition::Inside) {
			if (status == IntersectionType::Intersection_false)
				return IntersectionType::Intersection_true;
			status = IntersectionType::PolygonInclude;
		}
	}
	if (status == IntersectionType::FullMatch)
		return status;
	if (status == IntersectionType::PolygonInclude)
		return status;
	bool	is_equal = false;

	ObjPolygon	*dominantPolygon = ObjPolygon::checkDominationSquare(check_polygon, polygon_from_tt, is_equal);
	if (is_equal)
		dominantPolygon = check_polygon;
	if (dominantPolygon != check_polygon)
		return IntersectionType::Intersection_false;
	ObjPolygon	*obedientPolygon = polygon_from_tt;

	Dot		dot = obedientPolygon->data.arr_dot[0];

	if (Geometry::is_dotInPolygon(dot, dominantPolygon))
			return IntersectionType::PolygonUpper;
	return IntersectionType::Intersection_false;
}

	// if Arya inculde many parts, check them parent: if have different - ErrorPartsOfOneArya
IntersectionType				LogisticsController::_checkPartsOfOneArya_and_link_otherParts(
									std::vector<ObjPolygon *> parts_of_one_area)
{
	for (ObjPolygon *polygon : parts_of_one_area) {
		for (ObjPolygon *verified_polygon : parts_of_one_area) {
			if (verified_polygon == polygon)
				continue;
			if (verified_polygon->parent != polygon->parent)
				return IntersectionType::ErrorPartsOfOneArya;
		}
	}
	for (ObjPolygon *polygon : parts_of_one_area) {
		for (ObjPolygon *verified_polygon : parts_of_one_area) {
			if (verified_polygon == polygon)
				continue;
			polygon->otherParts.insert(polygon->otherParts.end(), verified_polygon);
		}
	}
	return IntersectionType::Intersection_false;
}






// MARK: - useful function
// 0) _delete_from_controller_and_return_polygon
// 1) _joinList (static)
// 2) _get_arr_dot_fromPolygon_string
// 3) _add_max_min_center_XY_toPolygonData
// 4) _showSituationWithPolygons
// 5) get_plast
	
	// delete polygons (parts of one arya) from plasts and _all_polygons,
	//  and return list of this polygons
std::vector<ObjPolygon *>		LogisticsController::_delete_from_controller_and_return_polygon(ObjPolygon	*exist_polygon) {
	std::vector<ObjPolygon *>	save_list_polygons;
	std::vector<Plast *>		save_list_plast;

	save_list_polygons.push_back(exist_polygon);
	save_list_plast.push_back(exist_polygon->plast);
	for (ObjPolygon *polygon : exist_polygon->otherParts) {
		save_list_polygons.push_back(polygon);
		save_list_plast.push_back(polygon->plast);
	}

		// delete save_list_polygons from controller
	Plast				*currentPlast;
	TetraTreePolygons	*new_tt_polygons = new TetraTreePolygons();
	TetraTreePolygons	*trash_tt_polygons;

	currentPlast = Plast::getPlast(exist_polygon);

	trash_tt_polygons = currentPlast->_tt_polygons;
	TetraTreePolygons::setUpdate_tt_polygons_without(trash_tt_polygons, save_list_polygons, new_tt_polygons);

	trash_tt_polygons->deleteBranches();
	delete trash_tt_polygons;
	if (!new_tt_polygons->polygon) {
		delete new_tt_polygons;
		new_tt_polygons = 0;
	}
	currentPlast->_tt_polygons = new_tt_polygons;

		// add save_list_plast->tt_polygons to controller
	ObjPolygon	*parent = exist_polygon->parent;
	for (Plast *plast : save_list_plast) {
		plast->_tt_polygons->updateParent(parent);
		if (currentPlast->_tt_polygons)
			currentPlast->_tt_polygons->addNew_tt_polygons(plast->_tt_polygons);
		else
			currentPlast->_tt_polygons = plast->_tt_polygons;
		plast->_tt_polygons = 0;
	}
	return save_list_polygons;
}

	// join 2 std::vector<ObjPolygon *>
std::vector<ObjPolygon *>		LogisticsController::_joinList(std::vector<ObjPolygon *> list_p1, std::vector<ObjPolygon *> list_p2)
{
	std::vector<ObjPolygon *> r_list;

	for (ObjPolygon * p : list_p1)
		r_list.insert(r_list.end(), p);
	for (ObjPolygon * p : list_p2)
		r_list.insert(r_list.end(), p);
	return r_list;
}

	// get array dot from polygonJSON["coord"][0] (std::string)
std::vector<Dot>		LogisticsController::_get_arr_dot_fromPolygon_string(
							std::string str_arr)
{
	std::vector<Dot>	arr_dot;
	JSON				json;
	int					size;
	int					i = -1;

	json = JSON::parse(str_arr);
	size = json.size();
	while (++i < size) {
		Dot		dot(json[i][0].get<double>(), json[i][1].get<double>());

		arr_dot.insert(arr_dot.end(), dot);
	}
	return arr_dot;
}

	// calculate and add max/min coord + center to  t_polygon_data data
void					LogisticsController::_add_max_min_center_XY_toPolygonData(
											std::vector<Dot> &arr_dot,
											t_polygon_data &data)
{

	data.max_x = -10000;
	data.max_y = -10000;
	data.min_x = 10000;
	data.min_y = 10000;
	for (Dot dot : arr_dot) {
		if (dot.x > data.max_x)
			data.max_x = dot.x;
		if (dot.x < data.min_x)
			data.min_x = dot.x;
		if (dot.y > data.max_y)
			data.max_y = dot.y;
		if (dot.y < data.min_y)
			data.min_y = dot.y;
	}
	data.center.x = (data.max_x + data.min_x ) / 2;
	data.center.y = (data.max_y + data.min_y ) / 2;
}

Plast					*LogisticsController::get_plast() {
	return this->_plast;
}

ResultStatus			LogisticsController::get_status() {
	return this->_status;
}

JSON					LogisticsController::get_result() {
	return this->_result;
}

	// show some sam info about polygons in controller
void							LogisticsController::_showSituationWithPolygons(
									std::vector<Plast *> list_plast) {
	TetraTreePolygons		*tt_polygons;
	std::vector<Plast *>	new_list_plast;

	if (!list_plast.size())
		return ;
	for (Plast * plast : list_plast) {
		if (!plast)
			continue;
		tt_polygons = plast->_tt_polygons;
		if (!tt_polygons || !tt_polygons->polygon)
			continue;
		std::cerr << "PLAST of polygon: ";
		if (tt_polygons->polygon->parent)
			std::cerr << tt_polygons->polygon->parent->data.id;
		else
			std::cerr << "0";
		std::cerr << "\n";
		if (tt_polygons)
			tt_polygons->show_polygons_and_add_new_plast_to_show(new_list_plast);
	}
	LogisticsController::_showSituationWithPolygons(new_list_plast);
}

	// find by name ObjPolygon* in this->_all_polygons
ObjPolygon						*LogisticsController::_getPolygon_by_id(std::string id) {
	for (ObjPolygon *polygon : this->_all_polygons) {
		if (polygon->data.id == id)
			return polygon;
	}
	return 0;
}
