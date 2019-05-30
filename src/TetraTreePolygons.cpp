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
	// this->deleteBranches();
	this->moreX_lessY = 0;
	this->moreX_moreY = 0;
	this->lessX_moreY = 0;
	this->lessX_lessY = 0;
}

void			TetraTreePolygons::addNewPolygon(ObjPolygon *polygon) {
	if (!this->polygon) {
		this->polygon = polygon;
		return ;
	}

	double					polygonX = polygon->data.center.x;
	double					polygonY = polygon->data.center.y;
	double					currentX = this->polygon->data.center.x;
	double					currentY = this->polygon->data.center.y;

	if (polygonX > currentX && polygonY <= currentY) {
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

void			TetraTreePolygons::addNew_tt_polygons(TetraTreePolygons *tt_polygons) {
	if (!tt_polygons || !tt_polygons->polygon || !this->polygon)
		return ;

	ObjPolygon				*polygon = tt_polygons->polygon;
	double					polygonX = polygon->data.center.x;
	double					polygonY = polygon->data.center.y;
	double					currentX = this->polygon->data.center.x;
	double					currentY = this->polygon->data.center.y;

	if (polygonX > currentX && polygonY <= currentY) {
		if (this->moreX_lessY)
			this->moreX_lessY->addNew_tt_polygons(tt_polygons);
		else
			this->moreX_lessY = tt_polygons;
		return ;
	}
	else if (polygonX >= currentX && polygonY > currentY) {
		if (this->moreX_moreY)
			this->moreX_moreY->addNew_tt_polygons(tt_polygons);
		else
			this->moreX_moreY = tt_polygons;
		return ;
	}
	else if (polygonX < currentX && polygonY >= currentY) {
		if (this->lessX_moreY)
			this->lessX_moreY->addNew_tt_polygons(tt_polygons);
		else
			this->lessX_moreY = tt_polygons;
		return ;
	}
	else if (polygonX <= currentX && polygonY < currentY) {
		if (this->lessX_lessY)
			this->lessX_lessY->addNew_tt_polygons(tt_polygons);
		else
			this->lessX_lessY = tt_polygons;
		return ;
	}
	else {
		if (this->moreX_moreY)
			this->moreX_moreY->addNew_tt_polygons(tt_polygons);
		else
			this->moreX_moreY = tt_polygons;
		return ;
	}
}

void			TetraTreePolygons::updateParent(ObjPolygon *parent) {
	if (!this || !this->polygon)
		return ;
	this->polygon->parent = parent;
	if (this->moreX_lessY)
		this->moreX_lessY->updateParent(parent);
	if (this->moreX_moreY)
		this->moreX_moreY->updateParent(parent);
	if (this->lessX_moreY)
		this->lessX_moreY->updateParent(parent);
	if (this->lessX_lessY)
		this->lessX_lessY->updateParent(parent);
}





// MARK: - static functions

	// get new (update) TetraTreePolygons whihout pull of polygons ()
void							TetraTreePolygons::setUpdate_tt_polygons_without(
										TetraTreePolygons *old_tt_polygons,
										std::vector<ObjPolygon *> polygons,
										TetraTreePolygons *new_tt_polygons)
{

	ObjPolygon			*check_polygon;

	check_polygon = old_tt_polygons->polygon;
	if (!TetraTreePolygons::is_existence(check_polygon, polygons))
		new_tt_polygons->addNewPolygon(check_polygon);
	if (old_tt_polygons->moreX_moreY)
		TetraTreePolygons::setUpdate_tt_polygons_without(old_tt_polygons->moreX_moreY,
											polygons, new_tt_polygons);
	if (old_tt_polygons->moreX_lessY)
		TetraTreePolygons::setUpdate_tt_polygons_without(old_tt_polygons->moreX_lessY,
											polygons, new_tt_polygons);
	if (old_tt_polygons->lessX_lessY)
		TetraTreePolygons::setUpdate_tt_polygons_without(old_tt_polygons->lessX_lessY,
											polygons, new_tt_polygons);
	if (old_tt_polygons->lessX_moreY)
		TetraTreePolygons::setUpdate_tt_polygons_without(old_tt_polygons->lessX_moreY,
											polygons, new_tt_polygons);
}

	// check existence ObjPolygon* in std::vector<ObjPolygon *>
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



void				TetraTreePolygons::deleteBranches_and_polygon() {
	if (this->polygon)
		std::cerr << "deleteBranches_and_polygon: " << this->polygon->data.id << "\n"; 
	if (this->moreX_moreY) {
		this->moreX_moreY->deleteBranches_and_polygon();
		delete this->moreX_moreY;
	}
	if (this->moreX_lessY) {
		this->moreX_lessY->deleteBranches_and_polygon();
		delete this->moreX_lessY;
	}
	if (this->lessX_lessY) {
		this->lessX_lessY->deleteBranches_and_polygon();
		delete this->lessX_lessY;
	}
	if (this->lessX_moreY) {
		this->lessX_moreY->deleteBranches_and_polygon();
		delete this->lessX_moreY;
	}
	if (this->polygon)
		delete polygon;
}





	// metod for recursion show of tt_polygons
	// and init list of plast, that included in showed polygons
void							TetraTreePolygons::show_polygons_and_add_new_plast_to_show(
									std::vector<Plast *> &list_plast)
{
	if (this->polygon) {
		std::cerr << this->polygon->data.id << " parent: ";
		if (this->polygon->parent)
			std::cerr << this->polygon->parent->data.id << "\n";
		else
			std::cerr << "0\n";
		if (this->polygon->plast)
			list_plast.push_back(this->polygon->plast);
	}
	if (this->moreX_lessY)
		this->moreX_lessY->show_polygons_and_add_new_plast_to_show(list_plast);
	if (this->moreX_moreY)
		this->moreX_moreY->show_polygons_and_add_new_plast_to_show(list_plast);
	if (this->lessX_moreY)
		this->lessX_moreY->show_polygons_and_add_new_plast_to_show(list_plast);
	if (this->lessX_lessY)
		this->lessX_lessY->show_polygons_and_add_new_plast_to_show(list_plast);
}