#include "pch.h"
#include "functions.h"

test_functions::MetaData test_functions::parse(const std::filesystem::path& path, const std::vector<std::string>& must_include, 
	                                           const std::vector<std::regex>& must_exclude, const std::vector<std::string>& param_names) {
	MetaData meta;
	meta.path = path;
	std::ifstream file{ path };
	std::string buff;
	size_t counter{ 0 };
	size_t must_include_sz{ must_include.size() };
	std::vector<bool> presense(must_include_sz, false);
	
	std::vector <std::regex> must_include_re;
	for (const std::string &it: must_include) 
		must_include_re.emplace_back(std::regex{ it });
	
	size_t param_names_sz{ param_names.size() };
	std::vector <std::regex> param_names_re;
	for (const std::string &it : param_names)
		param_names_re.emplace_back(std::regex{ it + "\\W*(-?\\d+(?:\\.\\d+)?)" });

	std::map<std::string, double> vars;
	for (const std::string& nm: param_names) {
		meta.vars[nm] = std::numeric_limits<double>::min();
	}

	while (getline(file, buff)) {
		++counter;

		for (const std::regex& it : must_exclude) {
			if (std::regex_search(buff, it)) {
				meta.status.info << "(" << counter << "): " << buff << "\n";
				return meta;
			}
		}

		for (size_t i{ 0 }; i < must_include_sz; ++i) {
			if (std::regex_search(buff, must_include_re[i])) presense[i] = true;
		}

		std::smatch sm;
		for (size_t i{ 0 }; i < param_names_sz; ++i) {
			if (regex_search(buff, sm, param_names_re[i])) {
				double tmp{ stod(sm[1]) };
				meta.vars[param_names[i]] = std::max(tmp, vars[param_names[i]]);
			}
		}

	}

	meta.status.success = true;

	for (size_t i{ 0 }; i < must_include_sz; ++i) {
		if (!presense[i]) {
			meta.status.info << ": missing '" << must_include[i] << "'\n";
			meta.status.success &= 0;
		}
	}

	return meta;
}


std::set<std::string> test_functions::recursive_search_names(const std::filesystem::path& pth, const std::regex& pattern){
    std::set<std::string> s;

    for (const auto &it: std::filesystem::recursive_directory_iterator(pth)) {
        std::smatch sm;
        std::string path{ it.path().string() };
        if ( regex_search(path, sm, pattern) ) s.insert(sm[0]);
    }

    return s;
}

std::set<std::filesystem::path> test_functions::recursive_search_paths(const std::filesystem::path& pth, const std::regex& pattern){
    std::set<std::filesystem::path> s;

    for (const auto &it: std::filesystem::recursive_directory_iterator(pth)) {
        if ( regex_search( it.path().string(), pattern) ) s.insert( it.path() );
    }

    return s;
}

std::set<std::filesystem::path> test_functions::set_of_paths(const std::filesystem::path& pth, const std::regex& pattern){
    std::set<std::filesystem::path> s;

    for(const auto& it: std::filesystem::directory_iterator(pth)){
        if ( regex_search(it.path().string(), pattern) ) s.insert( it.path() );
    }

    return  s;
}

test_functions::Status test_functions::exist(const std::filesystem::path& pth){
    test_functions::Status status;

    status.success = std::filesystem::exists(pth);
    if (!status.success) status.info << "directory missing: " << pth.filename().string() << "\n";

    return status;
}

test_functions::Status test_functions::same_file_set(const std::filesystem::path& pth1, const std::filesystem::path& pth2, const std::regex& re){
    Status status;

    std::string dir1_name = pth1.filename().string();
    std::string dir2_name = pth2.filename().string();

    std::set<std::string> dir1{test_functions::recursive_search_names(pth1, re)};
    std::set<std::string> dir2{test_functions::recursive_search_names(pth2, re)};

    std::set<std::string> d1_d2;
    std::set<std::string> d2_d1;

    set_difference(dir1.begin(), dir1.end(), dir2.begin(), dir2.end(), inserter(d1_d2,d1_d2.end()));
    set_difference(dir2.begin(), dir2.end(), dir1.begin(), dir1.end(), inserter(d2_d1,d2_d1.end()));

    if ( d1_d2.empty() && d2_d1.empty() ) {
        status.success = true;
        return status;
     }

    else {

		if (!d2_d1.empty()) {
			status.info << "In " << dir1_name << " there are missing files present in " << dir2_name << ":";
			for (const std::string& it : d2_d1) status.info << " '" << it << "',";
			status.info.seekp(-1, std::ios_base::end);
			status.info << "\n";
		}

		if (!d1_d2.empty()) {
			status.info << "In " << dir1_name << " there are extra files not present in " << dir2_name << ":";
			for (const std::string& it : d1_d2) status.info << " '" << it << "',";
			status.info.seekp(-1, std::ios_base::end);
			status.info << "\n";
		}

		status.success = false;
		return status;
	}
}

test_functions::Status test_functions::diff_param(MetaData& file1, const std::string& fname1, MetaData& file2, const std::string& fname2, const std::string& param_name, double criterion) {
    Status status; 

		double f1_max{ file1.vars[param_name] };
		double f2_max{ file2.vars[param_name] };
		double rel_diff = static_cast<double>(std::max(f1_max, f2_max)) / std::min(f1_max, f2_max) - 1.0;
		status.success = (rel_diff < criterion);

		if (!status.success)
			status.info << "(" << fname1 << "=" << f1_max << ", "
						<< fname2 << "=" << f2_max << ", "
						<< "rel.diff=" << std::round(rel_diff * 100) / 100 << ", "
						<< "criterion=" << criterion << ")\n";

    return status;
}