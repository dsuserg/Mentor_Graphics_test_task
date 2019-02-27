#include"parser_base.h"

using namespace std;
using namespace parser_impl;

void Parser_base::set_path(std::filesystem::path pth){
    this->path = pth;
}

filesystem::path Parser_base::get_path(){
    return this->path;
}

list<Rule*>& Parser_base::list_of_rules(){
    return this->rules;
}


