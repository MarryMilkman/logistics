// #include "LServer.hpp"

// #include "LogisticsController.hpp"

// LServer::LServer() {}

// LServer::~LServer() {
// 	closesocket(this->_listen_socket);
//     freeaddrinfo(this->_addr);
//     WSACleanup();
// }

// LServer::LServer(std::string host, int port) {
// 	WSADATA	wsaData;
// 	int		result = WSAStartup(MAKEWORD(2, 2), &wsaData);

// 	if (result != 0) {
//         std::cerr << "WSAStartup failed: " << result << "\n";
//         return ;
//     }
// 	if (this->_init_addr(host, port))
// 		return ;
// 	if (this->_init_listen_soket())
// 		return ;
// 	this->_startWork();
// }



// 	// init this->_addr
// int			LServer::_init_addr(std::string host, int port) {
// 	struct addrinfo hints;

// 	this->_addr = 0;
// 	ZeroMemory(&hints, sizeof(hints));
// 	hints.ai_family = AF_INET; // AF_INET определяет, что будет
//     hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
//     hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
//     hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
//     int	result = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &this->_addr);

//     if (result != 0) {
//         std::cerr << "getaddrinfo failed: " << result << "\n";
//         WSACleanup();
//         return 1;
//     }
//     return 0;
// }

// 	// init this->_listen_soket
// int		LServer::_init_listen_soket() {
// 	this->_listen_socket = socket(this->_addr->ai_family,
// 		this->_addr->ai_socktype, this->_addr->ai_protocol);
// 	if (this->_listen_socket == INVALID_SOCKET) {
// 		std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
// 		freeaddrinfo(this->_addr);
// 		WSACleanup();
// 		return 1;
// 	}
// 	int	result;

// 	result = bind(this->_listen_socket, this->_addr->ai_addr, (int)this->_addr->ai_addrlen);
// 	if (result == SOCKET_ERROR) {
// 		std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
//         freeaddrinfo(this->_addr);
//         closesocket(this->_listen_socket);
//         WSACleanup();
//         return 1;
// 	}
// 	result = listen(this->_listen_socket, SOMAXCONN);
// 	if (result) {
//         std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
//         closesocket(this->_listen_socket);
//         WSACleanup();
//         return 1;
//     }
// 	return 0;
// }



// // MARK: - get request and send response
// // 1) _startWork
// // 2) _getResponse
// // 3) _get_json_from_request

// 	// first init LogisticsController and start get requests
// void		LServer::_startWork() {
// 	LogisticsController		&controller = LogisticsController::getInstance();

// 	controller.reloadPolygonsFrom_db();
// 	while (1) {
// 		int			client_socket = accept(this->_listen_socket, NULL, NULL);
// 		int			result;
// 		char		request[500000];

// 		if (client_socket == INVALID_SOCKET) {
// 			std::cerr << "accept failed: " << WSAGetLastError() << "\n";
// 			freeaddrinfo(this->_addr);
// 			closesocket(this->_listen_socket);
// 			WSACleanup();
// 			return ;
// 		}
// 		result = recv(client_socket, request, 500000, 0);
// 		if (result == SOCKET_ERROR)
// 			std::cerr << "recv failed: " << result << "\n";
// 		else if (result > 0) {
// 			std::string		response;

// 			request[result] = 0;

// 				std::cerr << "Request:\n" << request << "\n\n";

// 			response = this->_getResponse(std::string(request));
// 			result = send(client_socket, response.c_str(), response.size(), 0);
// 			if (result == SOCKET_ERROR)
// 				std::cerr << "send failed: " << WSAGetLastError() << "\n";
// 		}
// 		closesocket(client_socket);
// 	}
// }

// 	// get std::string server response 
// std::string	LServer::_getResponse(std::string request) {
// 	LogisticsController		&controller = LogisticsController::getInstance();
// 	JSON					json_received;
// 	TypeJSON				type;
// 	std::string				response;
// 	std::string				header_response;
// 	JSON					body_response;

// 	body_response = {};
// 	json_received = this->_get_json_from_request(request, &type);
// 	if (type == TypeJSON::tPolyline) {
// 		int					i = -1;
// 		int					size = json_received.size();

// 		while (++i < size) {
// 			controller.init_polyline(json_received[i]);
// 			body_response[i] = controller.get_result();
// 		}
// 	}
// 	else if (type == TypeJSON::tPolygon) {
// 		int					i = -1;
// 		int					size = json_received.size();
// 		int					j = 0;

// 		while (++i < size) {
// 			controller.addPolygon_to_controller_and_db(json_received[i]);
// 			if (controller.get_status() != Polygon_added)
// 				body_response[j++] = controller.get_result();
// 		}
// 	}
// 	if (type == TypeJSON::tError)
// 		header_response = this->_get_errorHeader();
// 	else
// 		header_response = this->_get_successHeader(controller.get_status(),
// 			body_response.dump(2).size());
// 	response = header_response;
// 	if (!body_response.empty())
// 		response = response + body_response.dump(2);
// 	return response;
// }

// JSON		LServer::_get_json_from_request(std::string request, TypeJSON *type) {
// 	std::istringstream			tokenStream(request);
// 	std::vector<std::string>	parts_request;
// 	std::string					line;
// 	JSON						r_json;

// 	parts_request = split(request);
// 	tokenStream = std::istringstream(request);
// 	tokenStream >> line;
// 	// if (line != "POST") {
// 	// 	*type = tError;
// 	// 	return r_json;
// 	// }
// 	try {
// 		int		size = parts_request.size();

// 		r_json = JSON::parse(parts_request[size - 1]);
// 		*type = this->_getType_of_responseJSON(r_json);
// 	}
// 	catch (std::exception &e) {
// 		std::cerr << e.what() << "\n";
// 		*type = tError;
// 		return r_json;
// 	}
// 	return r_json;
// }

// TypeJSON		LServer::_getType_of_responseJSON(JSON responseJSON) {
// 	try {
// 		responseJSON[0]["color"];
// 		return TypeJSON::tPolygon;
// 	}
// 	catch (std::exception &e) {
// 		try {
// 			responseJSON[0][0];
// 		}
// 		catch (std::exception &e) {
// 			return TypeJSON::tError;
// 		}
// 		return TypeJSON::tPolyline;
// 	}
// }




// // MARK: - get HEADER http response

// std::string	LServer::_get_errorHeader() {
// 	std::stringstream	response("");
// 	std::string			str = "{\"ERROR\": \"Invalid json\"}";

// 	response << "HTTP/1.1 400 FAIL\r\n"
// 		<< "Version: HTTP/1.1\r\n"
// 		<< "Content-Type: application/json\r\n"
// 		<< "Content-Length: " << std::to_string(str.size())
// 		<< "\r\n\r\n"
// 		<< str;

//     return response.str();
// }

// std::string LServer::_get_successHeader(ResultStatus status, int size) {
// 	std::stringstream	response;

// 	if (status == ResultStatus::Polygon_intersect) {
// 		response << "HTTP/1.1 406 Intersection\r\n";
// 	}
// 	else if (status == ResultStatus::Polygon_fullMatch) {
// 		response << "HTTP/1.1 406 FullMatch\r\n";
// 	}
// 	else if (status == ResultStatus::Polygon_errorPartsOfOneArya) {
// 		response << "HTTP/1.1 406 ErrorPartsOfOneArya\r\n";
// 	}
// 	else
// 		response << "HTTP/1.1 201 Created\r\n";
// 	response << "Version: HTTP/1.1\r\n"
// 		<< "Content-Type: application/json\r\n"
// 		<< "Content-Length: " << size << "\r\n\r\n";
// 	return response.str();
// }