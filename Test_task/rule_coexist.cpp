#include"rule_coexist.h"
#include"regex"
using namespace std;
using namespace filesystem;
using namespace parser;

//Rule_dir_coexist::Rule_dir_coexist(initializer_list<regex> names){
//    for(regex name: names){
//    }
//}

Rule_coexist::Rule_coexist(initializer_list<regex> names, function<bool(path)> is_type){
    for (regex name: names){
        this->d_set.insert(name);
    }
    this->is_type = is_type;
}

bool Rule_coexist::start_enforcement(path pth){
    for(const regex& r: this->d_set ){

    }
    return 0;
}

