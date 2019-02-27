#include"parser_simple.h"
#include<iostream>
using namespace std;
using namespace parser;

Parser_simple::Parser_simple(filesystem::path pth){
        this->set_path(pth);
}

void Parser_simple::parse(){
    for(Rule* rule : this->list_of_rules()){
        rule->start_enforcement(this->get_path());
        if(rule->is_errors()){break;}
    }
}
