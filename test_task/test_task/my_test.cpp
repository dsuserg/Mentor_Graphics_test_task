#include "pch.h"
#include "my_tests.h"


test_functions::Status my_tests::custom_test(const std::filesystem::path &dir) {
	std::vector<std::string> must_include{ {"Solver finished at"} };
	std::vector<std::regex> must_exclude{ {std::regex{ "error", std::regex::icase }} };
	std::vector<std::string> param_names{ { std::string("Memory Working Set Peak"), std::string("MESH::Bricks: Total")} };

	char sep = dir.preferred_separator;

    std::string pth_ft_run { dir.string()+sep+"ft_run" };
    std::string pth_ft_reference { dir.string()+sep+"ft_reference" };

    test_functions::Status status;
	status = test_functions::exist(pth_ft_run);
    if (!status.success) return status;
	status = test_functions::exist(pth_ft_reference);
    if (!status.success) return status;
	status = test_functions::same_file_set(pth_ft_run, pth_ft_reference, std::regex{ "(\\d+.\\d+\\.stdout)" });
    if (!status.success) return status;

    std::set<std::filesystem::path> files_of_ft_run{ test_functions::recursive_search_paths(pth_ft_run, std::regex{ "\\d+.\\d+\\.stdout" }) };
    std::set<std::filesystem::path> files_of_ft_reference{ test_functions::recursive_search_paths(pth_ft_reference, std::regex{ "\\d+.\\d+\\.stdout" }) };

	std::stringstream info_aggregator;

    for (auto pth1 = files_of_ft_run.begin(), pth2 = files_of_ft_reference.begin();
         pth1 != files_of_ft_run.end() && pth2 != files_of_ft_reference.end();
         ++pth1, ++pth2) {

		test_functions::MetaData meta1{ test_functions::parse(*pth1, must_include, must_exclude, param_names) };
		test_functions::MetaData meta2{ test_functions::parse(*pth2, must_include, must_exclude, param_names) };

		if (!meta1.status.success) {
			info_aggregator << meta1.path.lexically_relative(dir).string() << meta1.status.info.str();
			continue;
		}
	 
		test_functions::Status s;
		s = test_functions::diff_param(meta1, "ft_run", meta2, "ft_reference", "Memory Working Set Peak", 4);
        if (!s.success) info_aggregator << pth1->lexically_relative(dir).string()
                                        << ": different 'Memory Working Set Peak' " << s.info.str();

		s = test_functions::diff_param(meta1, "ft_run", meta2, "ft_reference", "MESH::Bricks: Total", 0.1);
		if (!s.success) info_aggregator << pth2->lexically_relative(dir).string()
			                            << ": different 'Total' of bricks " << s.info.str();

    }

    if (info_aggregator.tellp() != 0) {
        status.success = false;
        status.info << info_aggregator.str();
        return status;
    }

	status.success = true;
    return status;
}

test_functions::Status my_tests::custom_test_run(const std::filesystem::path& dir, const std::initializer_list<std::ostream*>& out_streams, bool create_report) {
    test_functions::Status status {my_tests::custom_test(dir)};
	
	if (create_report) {
		std::filesystem::path report_pth{ dir };
		std::ofstream report{ report_pth.append("report.txt") };
		report << status.info.str();
	}

    for (auto *stream: out_streams) {
		status.success ? *stream << "OK: " : *stream << "FAIL: ";
        *stream << dir.parent_path().filename().string()
                << char(dir.preferred_separator)
                << dir.filename().string()
                << char(dir.preferred_separator)
                << std::endl << status.info.str();
   }

    return status;
}

