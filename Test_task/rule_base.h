#ifndef RULE_BASE_H
#define RULE_BASE_H

#include "parser.h"

namespace parser_impl {
using namespace parser;

class Rule_base:public Rule{
public:
    ~Rule_base() override = default;

    bool is_errors() override;
    std::string get_info() override;
protected:
    std::string info;
    bool errors{0};
};

}
#endif // RULE_BASE_H
