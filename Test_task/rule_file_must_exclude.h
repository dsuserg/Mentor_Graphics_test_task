#ifndef RULE_FILE_MUST_EXCLUDE_H
#define RULE_FILE_MUST_EXCLUDE_H
#include"rule_file_must_include.h"
//#include"string"
namespace parser {

class Rule_file_must_exclude: public Rule_file_must_include{
public:
    using Rule_file_must_include::Rule_file_must_include;
    bool start_enforcement(std::filesystem::path) override;
};

}

#endif // RULE_FILE_MUST_EXCLUDE_H
