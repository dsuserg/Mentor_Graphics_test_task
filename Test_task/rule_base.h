#ifndef RULE_BASE_H
#define RULE_BASE_H

#include "parser.h"

namespace parser_impl {
using namespace parser;
class Rule_base:public Rule{
public:
    bool is_errors() override;
    std::string get_info() override;
private:
    bool erors;
    std::string info;
};

}
#endif // RULE_BASE_H
