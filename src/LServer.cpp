#include "LServer.hpp"

#include "LogisticsController.hpp"

LServer::LServer() : 
	_acceptor(boost::asio::ip::tcp::acceptor(
		this->_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080)))
{}

LServer::~LServer() {
}



LServer::LServer(std::string host, int port) : 
	_acceptor(ip::tcp::acceptor(this->_service, ip::tcp::endpoint(ip::address::from_string(host.c_str()), port)))
{
	this->_startWork();
}




// MARK: - get request and send response
// 1) _startWork
// 2) _getResponse
// 3) _get_json_from_request

	// first init LogisticsController and start get requests
void		LServer::_startWork() {
	LogisticsController		&controller = LogisticsController::getInstance();

	controller.reloadPolygonsFrom_db();
	while (1) {
		boost::asio::ip::tcp::socket	client_socket(this->_service);
		int								bytes;
		char							request[500000];
		std::string						response;
		boost::system::error_code 		err;

		this->_acceptor.accept(client_socket);
		bytes = read(client_socket, boost::asio::buffer(request), boost::asio::transfer_at_least(1), err);
		request[bytes] = 0;
			std::cerr << "Request:\n" << request << "\n\n";
		response = this->_getResponse(std::string(request));
		client_socket.write_some(boost::asio::buffer(response));
		client_socket.close();

	}
}

	// get std::string server response 
std::string	LServer::_getResponse(std::string request) {
	LogisticsController		&controller = LogisticsController::getInstance();
	JSON					json_received;
	TypeJSON				type;
	std::string				response;
	std::string				header_response;
	JSON					body_response;

	body_response = {};
	json_received = this->_get_json_from_request(request, &type);
	if (type == TypeJSON::tPolyline) {
		int					i = -1;
		int					size = json_received.size();

		while (++i < size) {
			controller.init_polyline(json_received[i]);
			body_response[i] = controller.get_result();
		}
	}
	else if (type == TypeJSON::tPolygon) {
		int					i = -1;
		int					size = json_received.size();
		int					j = 0;

		while (++i < size) {
			controller.addPolygon_to_controller_and_db(json_received[i]);
			if (controller.get_status() != Polygon_added)
				body_response[j++] = controller.get_result();
		}
	}
	if (type == TypeJSON::tError)
		header_response = this->_get_errorHeader();
	else
		header_response = this->_get_successHeader(controller.get_status(),
			body_response.dump(2).size());
	response = header_response;
	if (!body_response.empty())
		response = response + body_response.dump(2);
	return response;
}

JSON		LServer::_get_json_from_request(std::string request, TypeJSON *type) {
	std::istringstream			tokenStream(request);
	std::vector<std::string>	parts_request;
	std::string					line;
	JSON						r_json;

	parts_request = split(request);
	tokenStream = std::istringstream(request);
	tokenStream >> line;
	// if (line != "POST") {
	// 	*type = tError;
	// 	return r_json;
	// }
	try {
		int		size = parts_request.size();

		r_json = JSON::parse(parts_request[size - 1]);
		*type = this->_getType_of_responseJSON(r_json);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << "\n";
		*type = tError;
		return r_json;
	}
	return r_json;
}

TypeJSON		LServer::_getType_of_responseJSON(JSON responseJSON) {
	try {
		responseJSON[0]["color"];
		return TypeJSON::tPolygon;
	}
	catch (std::exception &e) {
		try {
			responseJSON[0][0];
		}
		catch (std::exception &e) {
			return TypeJSON::tError;
		}
		return TypeJSON::tPolyline;
	}
}




// MARK: - get HEADER http response

std::string	LServer::_get_errorHeader() {
	std::stringstream	response("");
	std::string			str = "{\"ERROR\": \"Invalid json\"}";

	response << "HTTP/1.1 400 FAIL\r\n"
		<< "Version: HTTP/1.1\r\n"
		<< "Content-Type: application/json\r\n"
		<< "Content-Length: " << std::to_string(str.size())
		<< "\r\n\r\n"
		<< str;

    return response.str();
}

std::string LServer::_get_successHeader(ResultStatus status, int size) {
	std::stringstream	response;

	if (status == ResultStatus::Polygon_intersect) {
		response << "HTTP/1.1 406 Intersection\r\n";
	}
	else if (status == ResultStatus::Polygon_fullMatch) {
		response << "HTTP/1.1 406 FullMatch\r\n";
	}
	else if (status == ResultStatus::Polygon_errorPartsOfOneArya) {
		response << "HTTP/1.1 406 ErrorPartsOfOneArya\r\n";
	}
	else
		response << "HTTP/1.1 201 Created\r\n";
	response << "Version: HTTP/1.1\r\n"
		<< "Content-Type: application/json\r\n"
		<< "Content-Length: " << size << "\r\n\r\n";
	return response.str();
}