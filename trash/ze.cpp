


#include <iostream>
#include <vector>

#include <fstream>
#include <iomanip>
// #include <nlohmann/json.hpp>
// #include <libpq-fe.h>
#include <cmath>

// #include <WinSock2.h>
// #include <WS2tcpip.h>

// #pragma comment(lib, "Ws2_32.lib")


enum    IntersectionType {
    PolygonInclude = 0,
    PolygonUpper,
    Intersection_true,
    Intersection_false,
    FullMatch,
    ErrorPartsOfOneArya
};

enum    RelativePosition {
    Outside = 0,
    Inside,
    Border
};

enum    TypeJSON {
    tPolygon,
    tPolyline,
    tError
}

struct s
{
    int s;
};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}