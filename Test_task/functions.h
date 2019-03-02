#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <regex>
#include <set>
#include <filesystem>


namespace  parser{
    namespace functions {

    std::set<std::string> recursive_search_names(std::filesystem::path, std::regex);
    std::set<std::filesystem::path> recursive_search_paths(std::filesystem::path, std::regex);
    std::set<std::filesystem::path>set_of_paths(std::filesystem::path pth, std::regex pattern = std::regex{"\\w"});

    }
}

#endif // FUNCTIONS_H
