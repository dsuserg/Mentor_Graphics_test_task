#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <regex>
#include <set>
#include <filesystem>

std::set<std::string> recursive_search(std::filesystem::path, std::regex);



#endif // FUNCTIONS_H
