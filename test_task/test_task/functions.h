#pragma once

namespace test_functions {

	struct Status {
		bool success{ false };
		std::stringstream info;
	};

	struct MetaData {
		std::filesystem::path path;
		test_functions::Status status;
		std::map<std::string, double> vars;
	};


    std::set<std::string> recursive_search_names(const std::filesystem::path&, const std::regex& = std::regex{ "\\w" });
    std::set<std::filesystem::path> recursive_search_paths(const std::filesystem::path&, const std::regex& = std::regex{ "\\w" });
    std::set<std::filesystem::path> set_of_paths(const std::filesystem::path& pth, const std::regex& pattern = std::regex{ "\\w" });

    Status exist(const std::filesystem::path& pth);
    Status same_file_set(const std::filesystem::path& s1, const std::filesystem::path& s2, const std::regex& re);
	MetaData parse(const std::filesystem::path& path, const std::vector<std::string>& must_include, const std::vector<std::regex>& must_exclude, const std::vector<std::string>& param_names);
	Status diff_param(MetaData& file1, const std::string& fname1, MetaData& file2, const std::string& fname2, const std::string& param_name, double criterion);

}