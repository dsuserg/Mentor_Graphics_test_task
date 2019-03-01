#include<rule_file_must_include.h>
#include<fstream>

using namespace std;
using namespace parser;

Rule_file_must_include::Rule_file_must_include(string pattern){
    this->pattern = pattern;
}

Rule_file_must_include::Rule_file_must_include(string pattern, regex::flag_type flag):Rule_file_must_include{pattern} {
    this->flag = flag;
}

bool Rule_file_must_include::start_enforcement(filesystem::path pth){
    ifstream file{pth};
    regex re{this->pattern,this->flag};
    bool found = 0;
    string buff;

    while(getline(file,buff)){
        found = regex_search(buff, re);
        if(found) break;
    }

    this->errors = !found;

    if(this->errors){
        this->info = "missing '" + this->pattern + "'";
    }

    return found;
}
