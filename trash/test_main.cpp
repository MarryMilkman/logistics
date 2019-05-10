
// #include <iostream>
// #include <nlohmann/json.hpp>

// int main(int argc, char const *argv[])
// {
// 	std::cout << "Hello world\n";
// 	return 0;
// }


#include<iostream>
using namespace std;
#define sqr(x) x*x

int main()
{
 int y = 3;
 int z = sqr ( y++ );
 cout << y << " ";
 cout << z << " ";
 z = sqr ( y + 1 );
 cout << z << " ";
 return 0;
}

// #include "lib.h"

// using json = nlohmann::json;

// PGconn	*connect() {
// 	// PgConnection::PgConnection("");


// 	PGconn	*conn = 0;
// 	std::string	user = "postgres";
// 	std::string	password = "1234";
// 	std::string	host = "localhost";
// 	std::string	dbname = "postgres";
// 	std::string	port = "5432";


// 	conn = PQsetdbLogin(host.c_str(), port.c_str(), 0, 0, dbname.c_str(), user.c_str(), password.c_str());
// 	// request = "user=" + user + " password="
// 	// 		+ password + " host=" + host + " dbname=" + dbname;
// 	// conn = PQconnectdb(request.c_str());
// 	if (PQstatus(conn) != CONNECTION_OK) {
// 		std::cerr << "Error in connection to server\n";
// 		return 0;
// 	}
// 	std::cout << "Success!\n";
// 	return conn;
// }

// int main()
// {
// 	PGconn	*conn = connect();
// 	PGresult *res_;

// 	if (!conn)
// 		return 1;
// 	PQsendQuery(conn, "SELECT * from sss;");
// 	while (res_ = PQgetResult(conn) ) {
// 		std::cout << "HAMMM\n";
// 		int	nbr_rows = PQntuples(res_);
// 		int	nbr_colons = PQnfields(res_);
// 		int	i = 0;
// 		while (i < nbr_rows) {
// 			if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
//            		auto ID = PQgetvalue (res_ ,i, 0);
//            		std::cout << ID << "\n";
// 			}
//          	if (PQresultStatus(res_) == PGRES_FATAL_ERROR)
//             	std::cout<< PQresultErrorMessage(res_)<<std::endl;
//             i++;
//         }
//         PQclear( res_ );
//     }
// 	return (0);
// }

// int main(int argc, char const *argv[])
// {
// 	std::ifstream file;

// 	std::cerr << "AMMMMMM?\n";
// 	file.open("../resources/polyline.txt");

// 	if (file.is_open())
// 		std::cerr << "Shalom file\n";
// 	else
// 		std::cerr << "Fail open file\n";
//     // create a JSON object
//     json j;
//     file >> j;

//     int		size;

//     std::cout << j;

// 	return 0;
// }



// DELETE FROM CUSTOMERS
// WHERE ID = 6;