#include "LogisticsController.hpp"

#include "ObjPolygon.hpp"
#include "ObjPolyline.hpp"
#include "DotPolyline.hpp"

#include "TetraTreePolygons.hpp"
#include "Plast.hpp"

#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"


//MARK: - init Polyline

	// init polyline. pars from JSON and init polyline data
void		LogisticsController::init_polyline(JSON &json) {
	
	this->_polyline->free_polyline();
	this->_polyline->init_formJSON(json);

	this->_polyline->analyze_and_update_data();
	// this->_polyline->showDots();
	this->_addResult();
}




// MARK: - LOAD from data base
// 1) reloadPolygonsFrom_db
// 2) _getPolygon_from_db_PGresult

	// load polygons from data base
void				LogisticsController::reloadPolygonsFrom_db() {
	PGconn						*conn = this->_connectTo_db();
	PGresult					*res;
	std::vector<ObjPolygon *>	parts_of_one_area;	
	// ObjPolygon					*polygon;
	std::vector<ObjPolygon *>	conflictPolygons;

	std::cerr << "LogisticsController::reloadPolygonsFrom_db\n";
	this->_status = ResultStatus::Polygon_added;
	if (!conn) {
		std::cerr << "Connect error to db\n";
		exit(1);
	}
	PQsendQuery(conn, "SELECT arr_dot, color, id_polygon FROM table_polygons;");
	while ( (res = PQgetResult(conn)) ) {
		if (PQresultStatus(res) == PGRES_FATAL_ERROR){
			std::cerr<< PQresultErrorMessage(res)<<std::endl;
			exit(0);
		}
		int		nbrRow = PQntuples(res);
		int		i = -1;	

		std::cerr << "find " << nbrRow << " polygons in db\n\n\n\n\n\n";
		while (++i < nbrRow) {
			IntersectionType	answer;

			parts_of_one_area = this->_getPolygon_from_db_PGresult(res, i);
			this->_addPolygonTo_controller(parts_of_one_area);
		}
		PQclear(res);
	}
	std::cerr << "\n";
	std::vector<Plast *>	v;

	v.push_back(this->_plast);
	this->_showSituationWithPolygons(v);
	if (this->_status != ResultStatus::Polygon_added)
		std::cerr << "ERROR in db:\n" << this->_result.dump(2);
	PQfinish(conn);
}

	// create new polygon and return it
std::vector<ObjPolygon *>	LogisticsController::_getPolygon_from_db_PGresult(
								PGresult *res,
								int line)
{
	std::vector<ObjPolygon *>	r_parts;
	JSON						json_db;
	int							size;
	int							i = -1;

	std::cerr << "LogisticsController::_getPolygon_from_db_PGresult\n";

	json_db = JSON::parse(std::string(PQgetvalue(res, line, 0))); // [arr_dot]
	size = json_db.size();
	while (++i < size) {
		t_polygon_data				data;
		ObjPolygon					*polygon;
		std::string					strJSON;

		strJSON = json_db[i].dump();
		data.arr_dot = this->_get_arr_dot_fromPolygon_string(strJSON); // arr_dot
		data.color = std::string(PQgetvalue(res, line, 1)); // colot
		data.id = std::string(PQgetvalue(res, line, 2)); // id
		this->_add_max_min_center_XY_toPolygonData(data.arr_dot, data);
		polygon = new ObjPolygon(data);
		r_parts.insert(r_parts.end(), polygon);
	}
	return r_parts;
}






// MARK: - LOAD to data base and add to controller
// 1) addPolygon_to_controller_and_db
// 2) _addPolygonTo_controller
// 3) _addPolygonTo_db
// 4) _getPolygonIntoJSON
// 5) _connectTo_db

	// add polygon to db and controller
void				LogisticsController::addPolygon_to_controller_and_db(
						JSON json_polygon)
{
	std::vector<ObjPolygon *>	parts_of_one_area;
	std::vector<ObjPolygon *>	save_parts;
	std::string					id_exist_polygon = "";

	std::cerr << "LogisticsController::addPolygon_to_controller_and_db\n";
	this->_status = ResultStatus::Polygon_added;
	this->_result = {}; // JSON
	parts_of_one_area = this->_getPolygonIntoJSON(json_polygon);
		// with help of sql check existence polygon in db 
	if (this->_checkExistPolygon(parts_of_one_area[0]->data.id)) {
		id_exist_polygon = parts_of_one_area[0]->data.id;			
		ObjPolygon	*exist_polygon = this->_getPolygon_by_id(id_exist_polygon);
		if (exist_polygon)
			save_parts = this->_delete_from_controller_and_return_polygon(exist_polygon);
		else
			std::cerr << "Warning: polygon " << id_exist_polygon << " not find in controller\n";
	}

		std::vector<Plast *>	v;

		v.push_back(this->_plast);

	this->_addPolygonTo_controller(parts_of_one_area);
	if (this->_status == ResultStatus::Polygon_added) {
		if (!id_exist_polygon.empty()) {
			this->_delete_from_all_polygons_by_id(id_exist_polygon);
			this->deletePolygonFrom_db(id_exist_polygon);
		}
		this->_addPolygonTo_db(json_polygon["coords"], parts_of_one_area[0]);
		for (ObjPolygon *polygon : parts_of_one_area)
			this->_add_to_all_polygons(polygon);		
		
		this->_showSituationWithPolygons(v);
	}
	else if (!id_exist_polygon.empty()) {
		this->_addPolygonTo_controller(save_parts);
		for (ObjPolygon *polygon : parts_of_one_area)
			delete polygon;

		this->_showSituationWithPolygons(v);
	}
}

	// add polygon to controller, if checks pass
void			LogisticsController::_addPolygonTo_controller(std::vector<ObjPolygon *> parts_of_one_area) {
	IntersectionType			answer;
	std::vector<ObjPolygon *>	conflictPolygons;

	for (ObjPolygon * polygon : parts_of_one_area) {
		conflictPolygons = this->_plast->addNewPolygon_toPlast(polygon, 0, answer);
		if (conflictPolygons.size()) {
			// change status
			this->_addResult_error(polygon, conflictPolygons, answer);
			continue;
		}
		this->_add_to_all_polygons(polygon);
	}
	answer = LogisticsController::_checkPartsOfOneArya_and_link_otherParts(parts_of_one_area);
	if (answer == IntersectionType::ErrorPartsOfOneArya)
		this->_addResult_error(0, parts_of_one_area, IntersectionType::ErrorPartsOfOneArya);
}

	// add polygon to dt (create sql and exec)
void				LogisticsController::_addPolygonTo_db(JSON &json, ObjPolygon *polygon) {
	std::string		sql_str;
	PGconn			*conn = this->_connectTo_db();

	std::cerr << "LogisticsController::_addPolygonTo_db\n";
	sql_str = this->_createNewSQL_forAddPolygonInto_db(json, polygon);
	PQsendQuery(conn, sql_str.c_str());
	PGresult				*res;

	res = PQgetResult(conn);
	if (res && PQresultStatus(res) == PGRES_FATAL_ERROR){
		std::cerr<< PQresultErrorMessage(res)<<std::endl;
	}
	PQfinish(conn);
	// sql = sql + "INSERT INTO PeopleTable (id, arr_dot, color) VALUES";

}

	// get std::vector<ObjPolygon *> into json_polygon
std::vector<ObjPolygon *>		LogisticsController::_getPolygonIntoJSON(JSON &json_polygon) {
	std::vector<ObjPolygon *>	parts_of_one_area;
	t_polygon_data				data;
	std::string					string_arr;
	int							nbr_parts;
	int							i = -1;

	std::cerr << "LogisticsController::_getPolygonIntoJSON\n";
	nbr_parts = json_polygon["coords"].size();
	while (++i < nbr_parts) {
		data.id = json_polygon["name"].get<std::string>(); // id
		string_arr = json_polygon["coords"][i].dump();
		data.arr_dot = this->_get_arr_dot_fromPolygon_string(string_arr); // arr_dot
		this->_add_max_min_center_XY_toPolygonData(data.arr_dot, data); // max min XY center
		data.color = json_polygon["color"]; // colot
		parts_of_one_area.push_back(new ObjPolygon(data));
	}
	return parts_of_one_area;
}

	//connect to db
PGconn							*LogisticsController::_connectTo_db() {
	JSON			json;
	std::ifstream	file;
	PGconn			*conn = 0;
	std::string		user; 
	std::string		password;
	std::string		host;
	std::string		dbname;
	int				port;

	file.open(DATA_BASE_INFO_PATH);
	if (!file.is_open()) {
		std::cerr << "Fail open: " << DATA_BASE_INFO_PATH << "\n";
		std::cerr << "Add this file or reinit \n	#define DATA_BASE_INFO_PATH\nin inc/lib.h\n";
		return 0;
	}
	try {
		file >> json;
		user = json["user"];
		password = json["password"];
		host = json["host"];
		dbname = json["dbname"];
		port = json["port"];
	}
	catch (std::exception &e) {
		std::cerr << "Incorect JSON: " << DATA_BASE_INFO_PATH << "\n";
		return 0;
	}
	conn = PQsetdbLogin(host.c_str(), std::to_string(port).c_str(), 0, 0, dbname.c_str(), user.c_str(), password.c_str());
	if (PQstatus(conn) != CONNECTION_OK) {
		std::cerr << "Error connecting to Postgres server\n";
		return 0;
	}
	return conn;
}




// MARK: - result hendler

	//add result of polyline
void							LogisticsController::_addResult() {
	JSON	part_result;
	int		i;

	part_result = {
		{
			{"lat", 0.0},	// coord_x (latitude)
			{"lon", 0.0},	// coord_y (longitude)
			{"sumDistance", 0.0},	// distance
			{"time", ""},		// time
			{"polygonName", ""},
			{"polygonFeature", ""}
		}
	};
	i = 0;
	for (DotPolyline * pDot : this->_polyline->list_pDot) {
		part_result[0]["lat"] = pDot->dot.x;		// coord_x (latitude)
		part_result[0]["lon"] = pDot->dot.y;		// coord_y (longitude)
		part_result[0]["sumDistance"] = pDot->distance;	// distance
		part_result[0]["time"] = pDot->current_time;	// time
		// part_result[0]["id_dot"] = pDot->id;			// id_dot

		part_result[0]["polygonFeature"] = pDot->isIntersect ? "OUTPUT" : "";
		part_result[0]["polygonName"] = pDot->currentArea == 0 ? "" : pDot->currentArea->data.id;
		// part_result[0]["next_location"] = pDot->nextArea == 0 ? "" : pDot->nextArea->data.id;
		this->_result[i++] = part_result;
	}
}

	//add error reuslt
void							LogisticsController::_addResult_error(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons,
									IntersectionType typeError)
{
	std::cerr << "LogisticsController::_addResult_error\n";

	if (typeError == IntersectionType::Intersection_true
		|| typeError == IntersectionType::FullMatch)
		this->_addResult_error_intersect_fullMatch(polygon, conflictPolygons);
	if (typeError == IntersectionType::ErrorPartsOfOneArya)
		this->_addResult_error_intersect_errorPartsOfOneArya(conflictPolygons);
	
	if (this->_status != ResultStatus::Polygon_errorPartsOfOneArya
			&& this->_status != ResultStatus::Polygon_fullMatch)
	{
		if (typeError == IntersectionType::FullMatch)
			this->_status = ResultStatus::Polygon_fullMatch;
		else if (typeError == IntersectionType::ErrorPartsOfOneArya)
			this->_status = ResultStatus::Polygon_errorPartsOfOneArya;
		else
			this->_status = ResultStatus::Polygon_intersect;
	}
}

	// _addResult_error_intersect_fullMatch
void							LogisticsController::_addResult_error_intersect_fullMatch(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons)
{
	int			i;
	JSON		part_result = {
		{"conflict", {""}},
		{"name", ""}
	};
	
	i = 0;
	for (ObjPolygon *c_polygon : conflictPolygons)
		part_result["conflict"][i++] = c_polygon->data.id;
	if (polygon)
		part_result["name"] = polygon->data.id;
	i = this->_result.size();
	this->_result[i] = part_result;
}

	// _addResult_error_intersect_errorPartsOfOneArya
void							LogisticsController::_addResult_error_intersect_errorPartsOfOneArya(
									std::vector<ObjPolygon *> parts_of_one_area)
{
	int			i;
	JSON		part_result = {
		{"parents", {}},
		{"name", ""}
	};
	
	i = 0;
	if (!parts_of_one_area.size())
		return ;
	part_result["name"] = parts_of_one_area[0]->data.id;
	for (ObjPolygon *c_polygon : parts_of_one_area) {
		if (c_polygon->parent)
			part_result["parents"][i++] = c_polygon->parent->data.id;
		else
			part_result["parents"][i++] = "";
	}
	i = this->_result.size();
	this->_result[i] = part_result;
}





// MARK: - work with SQL
// 1) _createNewSQL_forAddPolygonInto_db
// 2) deletePolygonFrom_db
// 3) _checkExistPolygon

	// ctreate sql for add data to db
std::string			LogisticsController::_createNewSQL_forAddPolygonInto_db(JSON &json, ObjPolygon *polygon) {
	std::string		fits_purt;
	std::string		values_id;
	std::string		values_arr_dot;
	std::string		values_color;
	std::string		r_sql;

	std::cerr << "LogisticsController::_createNewSQL_forAddPolygonInto_db\n";

	fits_purt = "INSERT INTO table_polygons (arr_dot, color, id_polygon) VALUES (";
	
	std::string	str = json.dump();
	values_arr_dot = "\'" + json.dump() + "\', ";
	values_color = "\'"+ polygon->data.color + "\', ";
	values_id = "\'" + polygon->data.id + "\'";
	r_sql = fits_purt + values_arr_dot + values_color + values_id + ");";
	polygon->get_log();
	return r_sql;
}

	// delete polygon from db (create sql and exec)
void							LogisticsController::deletePolygonFrom_db(std::string id_polygon) {
	std::string		sql_str;
	PGconn			*conn = this->_connectTo_db();
	PGresult		*res;

	sql_str = "DELETE FROM table_polygons WHERE id_polygon = \'" + id_polygon + "\'";
	PQsendQuery(conn, sql_str.c_str());
	res = PQgetResult(conn);
	if (res && PQresultStatus(res) == PGRES_FATAL_ERROR) {
		std::cerr << PQresultErrorMessage(res) << std::endl;
	}
	PQfinish(conn);
}

	// with help of sql check existence polygon in db 
bool	LogisticsController::_checkExistPolygon(std::string id) {
	std::string		sql_str;
	PGconn			*conn = this->_connectTo_db();
	PGresult		*res;

	sql_str = "SELECT id_polygon from table_polygons where id_polygon = \'" + id + "\';";
	PQsendQuery(conn, sql_str.c_str());
	res = PQgetResult(conn);
	if (res && PQresultStatus(res) == PGRES_FATAL_ERROR) {
		std::cerr << PQresultErrorMessage(res) << std::endl;
		PQfinish(conn);
		return false;
	}
	int	nbrRow = PQntuples(res);
	PQfinish(conn);
	if (nbrRow)
		return true;
	return false;
}


void			LogisticsController::_delete_from_all_polygons_by_id(std::string id) {
	int		i;
	int		size;
	ObjPolygon *polygon;
	std::vector<ObjPolygon *> newList_all_polygons;

	i = -1;
	size = this->_all_polygons.size();
	while (++i < size) {
		polygon = this->_all_polygons[i];
		if (polygon->data.id == id)
			continue;
		newList_all_polygons.push_back(polygon);
	}
	this->_all_polygons.clear();
	this->_all_polygons = newList_all_polygons;
}

void			LogisticsController::_add_to_all_polygons(ObjPolygon *polygon) {
	this->_all_polygons.push_back(polygon);
}