#pragma once

#include <stdafx.h>

namespace test_functions {


    struct Status {
        bool success{0};
        std::string info;
    };


    class Checker {
    public:
        Status check(const std::filesystem::path& pth);
        void push_include(const std::string&);
        void pop_include();
        void push_exclude(const std::regex&);
        void pop_exclude();
    private:
        std::vector<std::string> include_lst;
        std::forward_list<std::regex> exclude_lst;
    };


    std::set<std::string> recursive_search_names(const std::filesystem::path&, const std::regex& = std::regex{"\\w"});
    std::set<std::filesystem::path> recursive_search_paths(const std::filesystem::path&, const std::regex& = std::regex{"\\w"});
    std::set<std::filesystem::path> set_of_paths(const std::filesystem::path& pth, const std::regex& pattern = std::regex{"\\w"});


    Status exist(const std::filesystem::path& pth);
    Status same_file_set(const std::filesystem::path& s1, const std::filesystem::path& s2, const std::regex& re);
    Status diff_param(std::ifstream& f1, const std::string& f1_name,
                      std::ifstream& f2, const std::string& f2_name,
                      const std::string& param_name, double criterion);

    double extract_max(std::ifstream& file, const std::string& name);


}




