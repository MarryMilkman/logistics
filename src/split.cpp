#include "lib.h"

std::vector<std::string>	split(std::string str) {
	std::stringstream			ss(str);
	std::string					line;
	std::vector<std::string>	vect;
	std::vector<std::string>	r_vect;
	std::string					save_str;

	while (getline(ss, line, '\n'))
		vect.push_back(line);
	for (std::string check_s : vect) {
		int		size = check_s.size();

		if (check_s[size - 1] == '\r') {
			check_s[size - 1] = 0;
			save_str = save_str + check_s;
			r_vect.push_back(save_str);
			save_str = "";
		}
		else
			save_str = save_str + check_s + "\n";
	}
	r_vect.push_back(save_str);
	return r_vect;
}