#include "pch.h"
#include "my_tests.h"

bool my_tests::custom_test(const std::filesystem::path &dir, std::ostream& out) {

	std::vector<std::string> must_include{ {"Solver finished at"} };
	std::vector<std::regex> must_exclude{ {std::regex{ "error", std::regex::icase }} };
	std::vector<std::string> param_names{ { std::string("Memory Working Set Peak"), std::string("MESH::Bricks: Total")} };
	bool success{ true };

	char sep = dir.preferred_separator;
    std::string pth_ft_run { dir.string()+sep+"ft_run" };
    std::string pth_ft_reference { dir.string()+sep+"ft_reference" };

    if ( !test_functions::exist( pth_ft_run, out) ) return false;
    if ( !test_functions::exist( pth_ft_reference, out) ) return false;
    if ( !test_functions::same_file_set(pth_ft_run, pth_ft_reference, std::regex{ "(\\d+.\\d+\\.stdout)" }, out) ) return false;

    std::set<std::filesystem::path> files_of_ft_run{ test_functions::recursive_search_paths(pth_ft_run, std::regex{ "\\d+.\\d+\\.stdout" }) };
    std::set<std::filesystem::path> files_of_ft_reference{ test_functions::recursive_search_paths(pth_ft_reference, std::regex{ "\\d+.\\d+\\.stdout" }) };

    for (auto pth1 = files_of_ft_run.begin(), pth2 = files_of_ft_reference.begin();
         pth1 != files_of_ft_run.end() && pth2 != files_of_ft_reference.end();
         ++pth1, ++pth2) {

		test_functions::MetaData meta1{ test_functions::parse(*pth1, must_include, must_exclude, param_names, out) };
		if (meta1.errors) { 
			success &= !meta1.errors;
			continue; 
		}

		test_functions::MetaData meta2{ test_functions::parse(*pth2, must_include, must_exclude, param_names, out) };

		std::string fmt{ pth1->parent_path().filename().append(pth1->filename().string()).string()  };
		if (!test_functions::diff_param( fmt + ": different 'Memory Working Set Peak' ",
										 meta1, "ft_run", meta2, "ft_reference", 
										 "Memory Working Set Peak", 4, out ) ) success &= 0;
			

		if (!test_functions::diff_param(fmt + ": different 'Total' of bricks ",
			                           meta1, "ft_run", meta2, "ft_reference", 
			                           "MESH::Bricks: Total", 0.1, out ) ) success &= 0;

    }
	
    return success;
}

bool my_tests::custom_test_run(const std::filesystem::path& pth_to_test, const std::filesystem::path& dir) {
	std::filesystem::path report_pth{ std::filesystem::path {dir}.append("report.txt") };
	std::ofstream oreport{ report_pth };
	bool success{ custom_test(dir, oreport) };
	oreport.close();
	if (success) {
			std::cout << "OK: " << dir.lexically_relative(pth_to_test).string()  << std::endl;
			return true;
	} 
	else {
		std::ifstream ireport{ report_pth };
		std::string buff;
		std::cout << "FAIL: " << dir.lexically_relative(pth_to_test).string() << std::endl;
		while (getline(ireport, buff)) {
			std::cout << buff << std::endl;
		}
		while (getline(ireport, buff)) {
			std::cout << buff << std::endl;
		}
		return false;
	}

}