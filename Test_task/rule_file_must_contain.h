#ifndef RULE_FILE_MUST_CONTAIN_H
#define RULE_FILE_MUST_CONTAIN_H

#include<regex>
#include"rule_base.h"

namespace parser {

class Rule_file_must_contain: public parser_impl::Rule_base{
public:
    Rule_file_must_contain(std::string);
    Rule_file_must_contain(std::string, std::regex::flag_type);
    bool start_enforcement(std::filesystem::path) override;
private:
    std::string pattern;
    std::regex::flag_type flag{std::regex_constants::ECMAScript};
};

}

#endif // RULE_FILE_MUST_CONTAIN_H

