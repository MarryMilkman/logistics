#include "TetraTreePolygons.hpp"
#include "ObjPolygon.hpp"

TetraTreePolygons::TetraTreePolygons() {
	this->moreX_lessY = 0;
	this->moreX_moreY = 0;
	this->lessX_moreY = 0;
	this->lessX_lessY = 0;
	this->polygon = 0;
}

TetraTreePolygons::TetraTreePolygons(ObjPolygon *polygon) {
	this->moreX_lessY = 0;
	this->moreX_moreY = 0;
	this->lessX_moreY = 0;
	this->lessX_lessY = 0;
	this->polygon = polygon;
}

TetraTreePolygons::~TetraTreePolygons() {
	this->moreX_lessY = 0;
	this->moreX_moreY = 0;
	this->lessX_moreY = 0;
	this->lessX_lessY = 0;
	if (this->polygon)
		delete this->polygon;
	this->deleteBranches();
}

void			TetraTreePolygons::addNewPolygon(ObjPolygon *polygon) {
	double					polygonX = polygon->data.center.x;
	double					polygonY = polygon->data.center.y;
	double					currentX = this->polygon->data.center.x;
	double					currentY = this->polygon->data.center.y;

	if (!this->polygon) {
		this->polygon = polygon;
		return ;
	}
	else if (polygonX > currentX && polygonY <= currentY) {
		if (this->moreX_lessY)
			this->moreX_lessY->addNewPolygon(polygon);
		else
			this->moreX_lessY = new TetraTreePolygons(polygon);
		return ;
	}
	else if (polygonX >= currentX && polygonY > currentY) {
		if (this->moreX_moreY)
			this->moreX_moreY->addNewPolygon(polygon);
		else
			this->moreX_moreY = new TetraTreePolygons(polygon);
		return ;
	}
	else if (polygonX < currentX && polygonY >= currentY) {
		if (this->lessX_moreY)
			this->lessX_moreY->addNewPolygon(polygon);
		else
			this->lessX_moreY = new TetraTreePolygons(polygon);
		return ;
	}
	else if (polygonX <= currentX && polygonY < currentY) {
		if (this->lessX_lessY)
			this->lessX_lessY->addNewPolygon(polygon);
		else
			this->lessX_lessY = new TetraTreePolygons(polygon);
		return ;
	}
	else {
		if (this->moreX_moreY)
			this->moreX_moreY->addNewPolygon(polygon);
		else
			this->moreX_moreY = new TetraTreePolygons(polygon);
		return ;
	}
}

void				TetraTreePolygons::deleteBranches() {
	if (this->moreX_moreY) {
		this->moreX_moreY->deleteBranches();
		delete this->moreX_moreY;
	}
	if (this->moreX_lessY) {
		this->moreX_lessY->deleteBranches();
		delete this->moreX_lessY;
	}
	if (this->lessX_lessY) {
		this->lessX_lessY->deleteBranches();
		delete this->lessX_lessY;
	}
	if (this->lessX_moreY) {
		this->lessX_moreY->deleteBranches();
		delete this->lessX_moreY;
	}
}




// MARK: - static functions

	// get new (update) TetraTreePolygons whihout pull of polygons ()
TetraTreePolygons					*TetraTreePolygons::getUpdate_tt_polygons_without(
										TetraTreePolygons *old_tt_polygons,
										std::vector<ObjPolygon *> polygons)
{
	TetraTreePolygons	*new_tt_polygons = new TetraTreePolygons();
	TetraTreePolygons	*tt_polygons;
	ObjPolygon				*check_polygon;

	check_polygon = old_tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
		new_tt_polygons->addNewPolygon(check_polygon);
	tt_polygons = old_tt_polygons;
	while ((tt_polygons = tt_polygons->moreX_moreY)) {
		check_polygon = tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
			new_tt_polygons->addNewPolygon(check_polygon);
	}
	tt_polygons = old_tt_polygons;
	while ((tt_polygons = tt_polygons->moreX_lessY)) {
		check_polygon = tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
			new_tt_polygons->addNewPolygon(check_polygon);
	}
	tt_polygons = old_tt_polygons;
	while ((tt_polygons = tt_polygons->lessX_lessY)) {
		check_polygon = tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
			new_tt_polygons->addNewPolygon(check_polygon);
	}
	tt_polygons = old_tt_polygons;
	while ((tt_polygons = tt_polygons->lessX_moreY)) {
		check_polygon = tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
			new_tt_polygons->addNewPolygon(check_polygon);
	}
	return new_tt_polygons;
}

	// check existence ObjPolygon* is std::vector<ObjPolygon *>
bool								TetraTreePolygons::is_existence(
										ObjPolygon *check_polygon, 
										std::vector<ObjPolygon *> polygons)
{
	int		i;
	int		size;

	size = polygons.size();
	i = 0;
	while (i < size) {
		if (polygons[i] == check_polygon)
			return true;
		i++;
	}
	return false;
}

TetraTreePolygons					*TetraTreePolygons::createNew_tt_polygons_with(
										std::vector<ObjPolygon *> polygons)
{
	TetraTreePolygons	*new_tt_polygons = new TetraTreePolygons();
	int					size = polygons.size();
	int					i = 0;

	while (i < size) {
		new_tt_polygons->addNewPolygon(polygons[i]);
		i++;
	}
	return new_tt_polygons;
}


std::vector<TetraTreePolygons *>	TetraTreePolygons::createSortListFromChildOf_tt_polygons_withDestination(
										TetraTreePolygons *tt_polygon, Dot &destination) 
{
	double								destX = destination.x;
	double								destY = destination.y;
	double								currentX = tt_polygon->polygon->data.center.x;
	double								currentY = tt_polygon->polygon->data.center.y;
	std::vector<TetraTreePolygons *>	r_list;

	if (destX > currentX && destY <= currentY) {
		if (tt_polygon->moreX_lessY)
			r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
		if ((destX - currentX) >= (currentY - destY)) {
			if (tt_polygon->moreX_moreY)
				r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
			if (tt_polygon->lessX_lessY)
				r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
		}
		else {
			if (tt_polygon->lessX_lessY)
				r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
			if (tt_polygon->moreX_moreY)
				r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
		}
		if (tt_polygon->lessX_moreY)
			r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
	}
	else if (destX >= currentX && destY > currentY) {
		if (tt_polygon->moreX_moreY)
			r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
		if ((destX - currentX) >= (destY - currentY)) {
			if (tt_polygon->lessX_moreY)
				r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
			if (tt_polygon->moreX_lessY)
				r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
		}
		else {
			if (tt_polygon->moreX_lessY)
				r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
			if (tt_polygon->lessX_moreY)
				r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
		}
		if (tt_polygon->lessX_lessY)
			r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
	}
	else if (destX < currentX && destY >= currentY) {
		if (tt_polygon->lessX_moreY)
			r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
		if ((currentX - destX) >= (destY - currentY)) {
			if (tt_polygon->moreX_moreY)
				r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
			if (tt_polygon->lessX_lessY)
				r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
		}
		else {
			if (tt_polygon->lessX_lessY)
				r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
			if (tt_polygon->moreX_moreY)
				r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
		}
		if (tt_polygon->moreX_lessY)
			r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
	}
	else if (destX <= currentX && destY < currentY) {
		if (tt_polygon->lessX_lessY)
			r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
		if ((currentX - destX) >= (currentY - destY)) {
			if (tt_polygon->lessX_moreY)
				r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
			if (tt_polygon->moreX_lessY)
				r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
		}
		else {
			if (tt_polygon->moreX_lessY)
				r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
			if (tt_polygon->lessX_moreY)
				r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
		}
		if (tt_polygon->moreX_moreY)
			r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
	}
	else {
		if (tt_polygon->moreX_lessY)
			r_list.insert(r_list.end(), tt_polygon->moreX_lessY);
		if (tt_polygon->moreX_moreY)
			r_list.insert(r_list.end(), tt_polygon->moreX_moreY);
		if (tt_polygon->lessX_moreY)
			r_list.insert(r_list.end(), tt_polygon->lessX_moreY);
		if (tt_polygon->lessX_lessY)
			r_list.insert(r_list.end(), tt_polygon->lessX_lessY);
	}
	return r_list;
}

