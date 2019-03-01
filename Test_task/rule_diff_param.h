#ifndef RULE_DIFF_PARAM_H
#define RULE_DIFF_PARAM_H

#include<regex>
#include"rule_base.h"
namespace parser {

class Rule_diff_param: public parser_impl::Rule_base{
public:
    Rule_diff_param(std::filesystem::path file1, std::filesystem::path file2, std::string param_name, float criterion);
    bool start_enforcement(std::filesystem::path) override;
private:
    float criterion;
    std::filesystem::path file1;
    std::filesystem::path file2;
    std::string param_name;
};

}
#endif // RULE_DIFF_PARAM_H
