#include"rule_base.h"

using namespace std;
using namespace parser_impl;

bool Rule_base::is_errors(){
    return this->errors;
}

string Rule_base::get_info(){
    return this->info;
}


