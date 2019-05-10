#include "ObjPolyline.hpp"
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
}

void			ObjPolyline::_setLocationOf_poliline_dots() {
	ObjPolygon					*previousPolygon = 0;

	std::cerr << "ObjPolyline::_init_polyline_data_setLocationOf_poliline_dot\n";
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
			// ?
		this->_add_list_intersect_dot_to_list_pDot_and_initLocate(list_intersect_dot, pDot2->dot, i);
		size = this->list_pDot.size();
		i += list_intersect_dot.size();
	}
	i = -1;
	while (++i < size - 2) {
		DotPolyline 				*pDot1 = list_pDot[i];
		DotPolyline 				*pDot2 = list_pDot[i + 1];
		DotPolyline 				*pDot3 = list_pDot[i + 2];
			//
		Dot							controlDot1 = Geometry::getCenter(Line(pDot1->dot, pDot2->dot));
		Dot							controlDot2 = Geometry::getCenter(Line(pDot2->dot, pDot3->dot));

		if (pDot2->position != RelativePosition::Border) {
			pDot2->isIntersect = false;
			continue;
		}
		bool	local_isIntersect = false;

		for (ObjPolygon *polygon : pDot2->listOf_contactAreas) {
			RelativePosition	position1 = Geometry::whereIs_dotInPolygon(controlDot1, polygon);
			RelativePosition	position2 = Geometry::whereIs_dotInPolygon(controlDot2, polygon);

			if (position1 != position2)
				local_isIntersect = true;
			if (position1 == RelativePosition::Inside && local_isIntersect)
				pDot2->previousArea = polygon;
			if (position2 == RelativePosition::Inside && local_isIntersect)
				pDot2->nextArea = polygon;
			if (pDot2->previousArea && pDot2->nextArea)
				break ;
			// if (local_isIntersect)
			// 	break ;
		}
		pDot2->isIntersect = local_isIntersect;
	}
}

void			ObjPolyline::_add_timeAndDistance_for_intersectionDots() {
	int		i = -1;
	int		size = this->list_pDot.size();

	while (++i < size - 1) {
		DotPolyline 	*pDot1 = this->list_pDot[i];
		DotPolyline 	*pDot2 = this->list_pDot[i + 1];
		Line			line(pDot1->dot, pDot2->dot);

		pDot2->distance = Geometry::get_distance(line);
	}
}






// useful function

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
		new_pDot->initLocation(this->list_pDot[i]->listOf_contactAreas[0]);
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
			if (dot.x < sort_list[i].x || dot.y < sort_list[i].y)
				sort_list.insert(sort_list.begin() + i, dot);
		}
		if (size == sort_list.size())
			sort_list.insert(sort_list.begin() + i, dot);
	}
	if (sort_list[0].x < dirention.x || sort_list[0].y < dirention.y)
		std::reverse(sort_list.begin(), sort_list.end());
	list_dot = sort_list;
}

std::vector<Dot>		ObjPolyline::get_list_intersect_dots(DotPolyline *pDot1, DotPolyline *pDot2) {
	std::vector<Dot>	r_list;
	Plast				*plast = Plast::getOveralPlast(pDot1->listOf_contactAreas[0], pDot2->listOf_contactAreas[0]);
	Line				line(pDot1->dot, pDot2->dot);

	ObjPolyline::_find_intersect(plast->_tt_polygons, line, r_list);
	return r_list;
}

void					ObjPolyline::_find_intersect(
								TetraTreePolygons *tt_polygons,
								Line line,
								std::vector<Dot> &r_list)
{
	ObjPolygon			*check_polygon = tt_polygons->polygon;
	std::vector<Dot>	new_list_intersectDots;
	bool				is_exist;

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
	if (tt_polygons->moreX_lessY)
		ObjPolyline::_find_intersect(tt_polygons->moreX_lessY, line, r_list);
	if (tt_polygons->moreX_moreY)
		ObjPolyline::_find_intersect(tt_polygons->moreX_moreY, line, r_list);
	if (tt_polygons->lessX_moreY)
		ObjPolyline::_find_intersect(tt_polygons->lessX_moreY, line, r_list);
	if (tt_polygons->lessX_lessY)
		ObjPolyline::_find_intersect(tt_polygons->lessX_lessY, line, r_list);
}

void					ObjPolyline::showDots() {
	int	i = 0;

	std::cerr << "ObjPolyline::_showDotsPolyline\n";
	for (DotPolyline *pDot : this->list_pDot) {
		std::cerr << i++ << " dot:\n" << pDot->dot.x << "\n" 
				<< pDot->dot.y << "\n" << pDot->current_time << "\n" 
				<< pDot->isIntersect << "\n"
				<< pDot->position << "\n"
				<< pDot->listOf_contactAreas[0] << "\n"
				<< pDot->distance << "\n";
	}
}