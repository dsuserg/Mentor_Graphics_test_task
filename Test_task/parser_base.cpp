#include"parser_base.h"

using namespace std;
using namespace parser_impl;

filesystem::path Parser_base::get_path(){
    return this->path;
}

std::list<Rule>& Parser_base::list_of_rules(){
    return this->rules;
}
