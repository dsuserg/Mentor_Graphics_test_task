#include "pch.h"
#include "my_tests.h"

test_functions::Status my_tests::custom_test(const std::filesystem::path &dir){
	char sep = dir.preferred_separator;
	test_functions::Checker checker{};
    checker.push_include("Solver finished at");
    checker.push_exclude(std::regex{ "error", std::regex::icase });

    std::string pth_ft_run { dir.string()+sep+"ft_run" };
    std::string pth_ft_reference { dir.string()+sep+"ft_reference" };

    test_functions::Status s;
    s = test_functions::exist(pth_ft_run);
    if (!s.success) return s;
    s = test_functions::exist(pth_ft_reference);
    if (!s.success) return s;
    s = test_functions::same_file_set(pth_ft_run, pth_ft_reference, std::regex{ "(\\d+.\\d+\\.stdout)" });
    if (!s.success) return s;

    std::set<std::filesystem::path> files_of_ft_run{ test_functions::recursive_search_paths(pth_ft_run, std::regex{ "\\d+.\\d+\\.stdout" }) };

	std::stringstream info_aggregator;

    for (const auto& path: files_of_ft_run) {
        test_functions::Status s{checker.check(path)};
        if (!s.success) info_aggregator << path.lexically_relative(dir).string()
                                        << s.info;
    }

    if (info_aggregator.tellp() != 0) {
        s.success = 0;
        s.info = info_aggregator.str();
        return s;
    }

    std::set<std::filesystem::path> files_of_ft_reference{ test_functions::recursive_search_paths(pth_ft_reference, std::regex{ "\\d+.\\d+\\.stdout" }) };

    for (auto pth1 = files_of_ft_run.begin(), pth2 = files_of_ft_reference.begin();
         pth1 != files_of_ft_run.end() && pth2 != files_of_ft_reference.end();
         ++pth1, ++pth2) {

        std::ifstream f1{*pth1};
        std::ifstream f2{*pth2};
        test_functions::Status s;
        std::string info;

        s = test_functions::diff_param(f1, "ft_run", f2, "ft_reference", "Memory Working Set Peak", 4);

        if (!s.success) info_aggregator << pth1->lexically_relative(dir).string()
                                        << ": different 'Memory Working Set Peak' " << s.info;

        s = test_functions::diff_param(f1, "ft_run", f2, "ft_reference", "MESH::Bricks: Total", 0.1);
        if (!s.success) info_aggregator << pth2->lexically_relative(dir).string()
                                        << ": different 'Total' of bricks " << s.info;

    }

    if (info_aggregator.tellp() != 0) {
        s.success = 0;
        s.info = info_aggregator.str();
        return s;
    }

    s.success = 1;
    return s;
}

test_functions::Status my_tests::custom_test_run(const std::filesystem::path& dir, const std::initializer_list<std::ostream*>& out_streams){
    test_functions::Status s {my_tests::custom_test(dir)};

    for (auto *stream: out_streams) {
        if(s.success){
            s.success = 1;
            *stream << "OK: " << dir.parent_path().filename().string()
                              << char(dir.preferred_separator)
                              << dir.filename().string()
                              << char(dir.preferred_separator)
                              << std::endl;
        }
        else
            *stream << "FAIL: " << dir.parent_path().filename().string()
                                << char(dir.preferred_separator)
                                << dir.filename().string()
                                << char(dir.preferred_separator)
                                << std::endl << s.info;
   }

    return s;
}

