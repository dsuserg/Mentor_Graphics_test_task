#ifndef RULE_SAME_FILE_SET_H
#define RULE_SAME_FILE_SET_H

#include<regex>
#include"rule_base.h"

namespace parser {

class Rule_same_file_set: public parser_impl::Rule_base{
public:
    Rule_same_file_set(std::string dir1, std::string dir2, std::regex pattern);
    ~Rule_same_file_set() override = default;

    bool start_enforcement(std::filesystem::path) override;
private:
    std::regex pattern;
    std::string dir1;
    std::string dir2;
};

}

#endif // RULE_SAME_FILE_SET_H
