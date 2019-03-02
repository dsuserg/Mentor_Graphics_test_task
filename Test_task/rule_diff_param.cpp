#include<algorithm>
#include<fstream>
#include<iostream>
#include<math.h>
#include"rule_diff_param.h"

using namespace std;
using namespace parser;

Rule_diff_param::Rule_diff_param(filesystem::path file1, filesystem::path file2, string param_name, float criterion){
    this->file1 = file1;
    this->file2 = file2;
    this->param_name = param_name;
    this->criterion = criterion;
}


bool Rule_diff_param::start_enforcement(std::filesystem::path pth){
    ifstream f{this->file1};
    string buff;
    regex re{this->param_name+"\\W*(\\d+)"};
    smatch sm;

    unsigned long f1_max{};
    unsigned long f2_max{};

    while (getline(f,buff)) {
        if(regex_search(buff, sm, re)){
            unsigned long tmp{stoul(sm[1])};
            if(tmp > f1_max) f1_max = tmp;
        }
    }

    f = this->file2;
    while (getline(f,buff)) {
        if(regex_search(buff, sm, re)){
            unsigned long tmp{stoul(sm[1])};
            if(tmp > f2_max) f2_max = tmp;
        }
    }

    float rel_diff = static_cast<float>(max(f1_max,f2_max))/min(f1_max,f2_max) - 1.0;

    this->errors = ( rel_diff > this->criterion);

    if(this->errors){
        stringstream fmt;
        fmt << "(file1="<< f1_max << ", " << "file2="<< f2_max <<", "<< "rel.diff="<< round(rel_diff*100)/100 << ", " "criterion="<< this->criterion <<")";
        this->info = fmt.str();
    }

    return !this->errors;
}
