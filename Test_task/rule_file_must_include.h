#ifndef RULE_FILE_MUST_CONTAIN_H
#define RULE_FILE_MUST_CONTAIN_H

#include<regex>
#include"rule_base.h"

namespace parser {

class Rule_file_must_include: public parser_impl::Rule_base{
public:
    Rule_file_must_include(std::string pattern);
    Rule_file_must_include(std::string pattern, std::regex::flag_type flag);
    ~Rule_file_must_include() override = default;

    bool start_enforcement(std::filesystem::path) override;
protected:
    std::string pattern;
    std::regex::flag_type flag;
};

}

#endif // RULE_FILE_MUST_CONTAIN_H

