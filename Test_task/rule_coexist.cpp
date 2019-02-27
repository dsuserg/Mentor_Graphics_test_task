#include"rule_coexist.h"
#include"regex"
#include"iostream"
using namespace std;
using namespace filesystem;
using namespace parser;

Rule_coexist::Rule_coexist(initializer_list<string> patterns, function<bool(const path&)> is_type){
    for (string pattern: patterns){
        this->dict[pattern];
    }
    this->is_type = is_type;
}

bool Rule_coexist::start_enforcement(path pth){
    bool state = 1;
    for(auto& it:this->dict){
        bool found = 0;
        for(auto& entity : directory_iterator(pth)){
            if(is_type(entity)){
                found = regex_search(string(entity.path().filename()), regex(it.first));
                if(found){
                    it.second = 1;
                    break;
                }
            }
        }
        state &= found;
    }
    this->errors = !state;
    return state;
}

string Rule_coexist::get_info(){
    string missing;
    for(auto &it:this->dict){
        if(!it.second){
            missing += " " + it.first + ",";
        }
    }
    missing.pop_back();
    return missing;
}
