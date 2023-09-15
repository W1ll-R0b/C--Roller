#pragma once

#include "statreadandprocess.cpp"

inline std::vector<int> statget();
inline std::pair<int, int> processStringToken(std::string, std::vector<int>&);
inline std::string processAltStringToken(std::string);
inline void printRolls(std::vector<std::vector<int>>, std::vector<std::string>, int);
