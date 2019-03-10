#include "pch.h"
#include "functions.h"

void test_functions::Checker::push_include(const std::string& include){
    include_lst.push_back(include);
}

void test_functions::Checker::pop_include(){
    include_lst.pop_back();
}

void test_functions::Checker::push_exclude(const std::regex& exclude){
    exclude_lst.push_front(exclude);
}

void test_functions::Checker::pop_exclude(){
    exclude_lst.pop_front();
}

test_functions::Status test_functions::Checker::check(const std::filesystem::path& pth){
    std::ifstream file{pth};
    std::string buff;
    Status status;
    size_t counter{0};
	size_t inc_lst_sz{ include_lst.size() };
    std::vector<bool> presense(this->include_lst.size(), 0);

    while ( getline(file,buff) ) {
        ++counter;

        for (size_t i{0}; i < inc_lst_sz; ++i) {
            if ( std::regex_search(buff, std::regex{ this->include_lst[i] }) ) presense[i] = 1;
        }

        for (const std::regex& it: this->exclude_lst) {
            if ( std::regex_search(buff, it) ) {
                status.info = "(" + std::to_string(counter) + ")" + ": " + buff + "\n";
                return status;
            }
        }

    }

    status.success = 1;

    for (size_t i{0}; i < inc_lst_sz; ++i) {
        if (!presense[i]) {
            status.info += ": missing '" + this->include_lst[i] + "'\n";
            status.success &= 0;
        }
    }

    return  status;
}


std::set<std::string> test_functions::recursive_search_names(const std::filesystem::path& pth, const std::regex& pattern){
    std::set<std::string> s;

    for (const auto &it: std::filesystem::recursive_directory_iterator(pth)) {
        std::smatch sm;
        std::string path{it.path().string()};
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

    if (!status.success) status.info = "directory missing: " + pth.filename().string() + "\n";

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
        status.success = 1;
        return status;
     }

        else {
            if ( d1_d2.empty() ) {
                status.info += "In " + dir1_name + " there are missing files present in " + dir2_name + ":";
                for (const std::string& it: d2_d1) status.info += " '" + it + "',";

                status.info.pop_back();
            }

            else if ( d2_d1.empty() ) {
                status.info += "In " + dir1_name + " there are extra files not present in " + dir2_name +":";
                for (const std::string& it: d1_d2) status.info += " '" + it + "',";

                status.info.pop_back();
            }

            else {
                status.info += "In " + dir1_name + " there are missing files present in " + dir2_name +":";
                for (const std::string& it: d2_d1) status.info += " '" + it + "',";

                status.info.pop_back();

                status.info += "\nIn " + dir1_name + " there are extra files not present in " + dir2_name +":";
                for (const std::string& it: d1_d2) status.info += " '" + it + "',";

                status.info.pop_back();
            }

            status.info += "\n";
            status.success = 0;
            return status;
        }
}

test_functions::Status test_functions::diff_param(std::ifstream& f1, const std::string& f1_name,
                                                  std::ifstream& f2, const std::string& f2_name,
                                                  const std::string& param_name, double criterion){
    Status status; 

    double f1_max{extract_max(f1, param_name)};
    double f2_max{extract_max(f2, param_name)};

    double rel_diff = static_cast<double>( std::max(f1_max, f2_max) )/std::min(f1_max, f2_max ) - 1.0;

    status.success = (rel_diff < criterion);

    if (!status.success) {
        std::stringstream fmt;
        fmt << "(" << f1_name << "=" << f1_max << ", "
            << f2_name << "=" << f2_max << ", "
            << "rel.diff="<<  std::round(rel_diff*100)/100 << ", "
            << "criterion="<< criterion << ")\n";
        status.info = fmt.str();
    }

    return status;
}

double test_functions::extract_max(std::ifstream &file, const std::string& var_name){
    std::string buff;
    std::regex re{ var_name+"\\W*(-?\\d+(?:\\.\\d+)?)" };
    std::smatch sm;
    double max{ std::numeric_limits<double>::min() };

    while ( getline(file, buff) ) {
        if ( regex_search(buff, sm, re) ) {
            double tmp{ stod(sm[1]) };
            if (tmp > max) max = tmp;
        }
    }

    file.clear();
    file.seekg(0);

    return max;
}
