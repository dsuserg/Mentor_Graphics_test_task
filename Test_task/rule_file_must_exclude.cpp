#include<fstream>
#include"rule_file_must_exclude.h"

using namespace std;
using namespace parser;


bool Rule_file_must_exclude::start_enforcement(filesystem::path pth){
    ifstream file{pth};
    regex re{this->pattern,this->flag};
    bool found{0};
    string buff;
    unsigned long counter{0};

    while(getline(file,buff)){
        ++counter;
        found = regex_search(buff, re);
        if(found) break;
    }

    this->errors = found;

    if(this->errors){
        this->info = "(" + to_string(counter) + ")" + ":" + buff;
    }

    return !found;
}
