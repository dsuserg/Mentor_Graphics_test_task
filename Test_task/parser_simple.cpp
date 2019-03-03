#include"parser_simple.h"
#include<iostream>
using namespace std;
using namespace parser;

Parser_simple::Parser_simple(filesystem::path pth){
        this->set_path(pth);
}

bool Parser_simple::parse(){
    for(Rule* rule : this->list_of_rules()){
        if(!(rule->start_enforcement(this->get_path()))){
            return 0;
        }
    }
    return 1;
}

