#include "ObjPolyline.hpp"
#include "ObjPolygon.hpp"

#include "DotPolyline.hpp"

#include "Plast.hpp"

#include "TetraTreePolygons.hpp"

#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"

ObjPolyline::ObjPolyline() {
	this->list_pDot = std::vector<DotPolyline *>();
}

ObjPolyline::ObjPolyline(std::vector<DotPolyline *> list_pDot) {
	this->list_pDot = list_pDot;
}

ObjPolyline::ObjPolyline(ObjPolyline const & ref) {
	*this = ref;
}

ObjPolyline::~ObjPolyline() {
	this->free_polyline();
}

ObjPolyline		&ObjPolyline::operator=(ObjPolyline const & ref) {
	this->list_pDot = ref.list_pDot;
	return *this;
}

	// free data polyline
void			ObjPolyline::free_polyline() {
	for (DotPolyline *pDot : this->list_pDot) {
		delete pDot;
		pDot = 0;
	}
	this->list_pDot = std::vector<DotPolyline *>();
}


//MARK : init form json

void			ObjPolyline::init_formJSON(JSON &json) {
	int		i = -1;
	int		size = json.size();

	this->free_polyline();
	while (++i < size) {
		DotPolyline		*pDot = new DotPolyline;

		pDot->dot.x = json[i][0];
		pDot->dot.y = json[i][1];
		pDot->current_time = json[i][3];
		pDot->isIntersect = false;
		this->list_pDot.insert(this->list_pDot.end(), pDot);
	}
}


// MARK : - analyze and update data
// 1) analyze_and_update_data
// 2) _setLocationOf_poliline_dots
// 3) _add_intersectionDots
// 4) _add_timeAndDistance_for_intersectionDots

void			ObjPolyline::analyze_and_update_data() {
	this->_setLocationOf_poliline_dots();
	this->_add_intersectionDots();
	this->_add_timeAndDistance_for_intersectionDots();
	// this->showDots();
}

void			ObjPolyline::_setLocationOf_poliline_dots() {
	ObjPolygon					*previousPolygon = 0;

	std::cerr << "ObjPolyline::_setLocationOf_poliline_dots\n";
	for (DotPolyline *pDot : this->list_pDot) {
		pDot->initLocation(previousPolygon);
		pDot->currentArea = pDot->listOf_contactAreas[0];
		previousPolygon = pDot->listOf_contactAreas[0];
	}
}

void			ObjPolyline::_add_intersectionDots() {
	int							i = -1;
	int							size = this->list_pDot.size();

	std::cerr << "ObjPolyline::_add_intersectionDots\n";
	while (++i < size - 1) {
		DotPolyline 				*pDot1 = this->list_pDot[i];
		DotPolyline 				*pDot2 = this->list_pDot[i + 1];
		std::vector<Dot>			list_intersect_dot;

		list_intersect_dot = ObjPolyline::get_list_intersect_dots(pDot1, pDot2);
		this->_add_list_intersect_dot_to_list_pDot_and_initLocate(list_intersect_dot, pDot2->dot, i);
		size = this->list_pDot.size();
		i += list_intersect_dot.size();
	}
	i = -1;
	while (++i < size - 2) {
		DotPolyline 				*pDot1 = this->list_pDot[i];
		DotPolyline 				*pDot2 = this->list_pDot[i + 1];
		DotPolyline 				*pDot3 = this->list_pDot[i + 2];
			//
		Dot							controlDot1 = Geometry::getCenter(Line(pDot1->dot, pDot2->dot));
		Dot							controlDot2 = Geometry::getCenter(Line(pDot2->dot, pDot3->dot));


		if (pDot2->position != RelativePosition::Border) {
			pDot2->isIntersect = false;
			continue;
		}
		DotPolyline					control_pDot1(controlDot1, "");
		DotPolyline					control_pDot2(controlDot2, "");

		control_pDot1.initLocation(pDot1->currentArea);
		control_pDot2.initLocation(pDot2->currentArea);

		if (control_pDot1.currentArea != control_pDot2.currentArea) {
			pDot2->nextArea = control_pDot2.currentArea;
			pDot2->isIntersect = true;
		}
		pDot2->currentArea = control_pDot1.currentArea;
		// pDot2->currentArea = 0;
		// for (ObjPolygon *polygon : pDot2->listOf_contactAreas) {
		// 	RelativePosition	position1 = Geometry::whereIs_dotInPolygon(controlDot1, polygon);
		// 	RelativePosition	position2 = Geometry::whereIs_dotInPolygon(controlDot2, polygon);

		// 	if (position1 != position2)
		// 		local_isIntersect = true;
		// 	if (position1 == RelativePosition::Inside && local_isIntersect)
		// 		pDot2->previousArea = polygon;
		// 	if (position2 == RelativePosition::Inside && local_isIntersect)
		// 		pDot2->nextArea = polygon;
		// 	if (pDot2->previousArea && pDot2->nextArea)
		// 		break ;
		// 	// if (local_isIntersect)
		// 	// 	break ;
		// }
		// pDot2->isIntersect = local_isIntersect;
	}
}

void			ObjPolyline::_add_timeAndDistance_for_intersectionDots() {
		// distance part
	int		i = -1;
	int		size = this->list_pDot.size();

	while (++i < size - 1) {
		DotPolyline 	*pDot1 = this->list_pDot[i];
		DotPolyline 	*pDot2 = this->list_pDot[i + 1];
		Line			line(pDot1->dot, pDot2->dot);

		pDot2->distance = Geometry::get_distance(line);
	}
		// time part
	DotPolyline					*control_pDot1 = 0;
	DotPolyline					*control_pDot2 = 0;
	std::vector<DotPolyline*>	list_noTime_pDot;

	for (DotPolyline *pDot : this->list_pDot) {
		if (pDot->current_time != "unknown") {
			if (!control_pDot1)
				control_pDot1 = pDot;
			else
				control_pDot2 = pDot;
		}
		else
			list_noTime_pDot.push_back(pDot);

		if (control_pDot1 && control_pDot2) {
			if (!list_noTime_pDot.empty()) {
				this->_initTime_of_list(control_pDot1, control_pDot2, list_noTime_pDot);
				list_noTime_pDot.clear();
			}
			control_pDot1 = control_pDot2;
			control_pDot2 = 0;
		}
		
	}
}


void			ObjPolyline::_initTime_of_list(
					DotPolyline *pDot1,
					DotPolyline *pDot2, 
					std::vector<DotPolyline*> list)
{
	std::string	format(TIME_FORMAT);
	int		s_time1 = LTime::get_unix_second(pDot1->current_time, format.c_str());
	int		s_time2 = LTime::get_unix_second(pDot2->current_time, format.c_str());
	int		sum_sec;
	double	distance;
	double	speed;
	Line	control_line(pDot1->dot, pDot2->dot);

	pDot1->sumTime = s_time1;
	pDot2->sumTime = s_time2;
	sum_sec = s_time2 - s_time1;
	distance = Geometry::get_distance(control_line);
	speed = distance/sum_sec;

	for (DotPolyline *pDot : list) {
		distance = pDot->distance;
		pDot->sumTime = distance/speed + s_time1;
		s_time1 = pDot->sumTime;
		pDot->init_current_time();
	}
}





// MARK: - useful function
// 1) _add_list_intersect_dot_to_list_pDot_and_initLocate
// 2) _sort_list_intersect
// 3) get_list_intersect_dots (static)
// 4) _find_intersect
// 5) showDots

void			ObjPolyline::_add_list_intersect_dot_to_list_pDot_and_initLocate(
								std::vector<Dot> &list_dot,
								Dot dirention,
								int	i)
{
	int		j;

	this->_sort_list_intersect(list_dot, dirention);
	j = 0;
	for (Dot dot : list_dot) {
		DotPolyline	*new_pDot = new DotPolyline();

		new_pDot->dot = dot;
		new_pDot->initLocation(this->list_pDot[i + j]->listOf_contactAreas[0]);
		this->list_pDot.insert(this->list_pDot.begin() + i + 1, new_pDot);
		j++;
	}
}

void			ObjPolyline::_sort_list_intersect(
								std::vector<Dot> &list_dot,
								Dot dirention)
{
	if (list_dot.size() <= 1)
		return ;

	std::vector<Dot>	sort_list;

	for (Dot dot : list_dot) {
		int	i = -1;
		int	size = sort_list.size();

		if (!size) {
			sort_list.insert(sort_list.begin(), dot);
			continue;
		}
		while (++i < size) {
			if (dot.x < sort_list[i].x || dot.y < sort_list[i].y) {
				sort_list.insert(sort_list.begin() + i, dot);
				break ;
			}
		}
	}
	if (sort_list[0].x < dirention.x || sort_list[0].y < dirention.y)
		std::reverse(sort_list.begin(), sort_list.end());
	list_dot = sort_list;
}

std::vector<Dot>		ObjPolyline::get_list_intersect_dots(DotPolyline *pDot1, DotPolyline *pDot2) {
	std::vector<Dot>		r_list;
	Plast					*plast = Plast::getOveralPlast(pDot1->listOf_contactAreas[0], pDot2->listOf_contactAreas[0]);
	Line					line(pDot1->dot, pDot2->dot);
	std::vector<Plast *>	list_plast;

	list_plast.push_back(plast);
	while (list_plast.size()) {
		std::vector<Plast *>	next_list_plast;
		for (Plast *check_plast : list_plast) {
			std::vector<Plast *>	locat_list_plast;
			locat_list_plast = ObjPolyline::_find_intersect(check_plast->_tt_polygons, line, r_list);
			for (Plast *local_plast : locat_list_plast)
				next_list_plast.push_back(local_plast);
		}
		list_plast = next_list_plast;
	}
	return r_list;
}

std::vector<Plast *>		ObjPolyline::_find_intersect(
								TetraTreePolygons *tt_polygons,
								Line line,
								std::vector<Dot> &r_list)
{
	if (!tt_polygons)
		return std::vector<Plast *>();

	ObjPolygon				*check_polygon = tt_polygons->polygon;
	std::vector<Dot>		new_list_intersectDots;
	std::vector<Plast *>	r_list_plast = std::vector<Plast *>();
	bool					is_exist;

	if (check_polygon && check_polygon->plast->_tt_polygons &&
			check_polygon->plast->_tt_polygons->polygon) {
		r_list_plast.push_back(check_polygon->plast);
	}
	new_list_intersectDots = Geometry::getDotIntersect(line, check_polygon);
	for (Dot dot : new_list_intersectDots) {
		is_exist = false;
		for (Dot check_dot : r_list) {
			if (check_dot == dot) {
				is_exist = true;
				break ;
			}
		}
		if (!is_exist)
			r_list.insert(r_list.end(), dot);
	}
	if (tt_polygons->moreX_lessY) {
		for (Plast *local_plast : ObjPolyline::_find_intersect(tt_polygons->moreX_lessY, line, r_list))
			r_list_plast.push_back(local_plast);
	}
	if (tt_polygons->moreX_moreY) {
		for (Plast *local_plast : ObjPolyline::_find_intersect(tt_polygons->moreX_moreY, line, r_list))
			r_list_plast.push_back(local_plast);
	}
	if (tt_polygons->lessX_moreY) {
		for (Plast *local_plast : ObjPolyline::_find_intersect(tt_polygons->lessX_moreY, line, r_list))
			r_list_plast.push_back(local_plast);
	}
	if (tt_polygons->lessX_lessY) {
		for (Plast *local_plast : ObjPolyline::_find_intersect(tt_polygons->lessX_lessY, line, r_list))
			r_list_plast.push_back(local_plast);
	}
	return r_list_plast;
}

void					ObjPolyline::showDots() {
	int	i = 0;

	std::cerr << "ObjPolyline::_showDotsPolyline\n";
	for (DotPolyline *pDot : this->list_pDot) {
		std::cerr << i++ << " dot:\n" << pDot->dot.x << "\n" 
				<< pDot->dot.y << "\n" << "time: " << pDot->current_time << "\n" 
				<< pDot->isIntersect << "\n"
				<< pDot->position << "\n"
				<< pDot->listOf_contactAreas[0]
				<< pDot->distance << "\n"
				<< pDot->listOf_contactAreas.size() << "\n\n";
	}
}