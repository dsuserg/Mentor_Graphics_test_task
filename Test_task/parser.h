#ifndef PARSER_H
#define PARSER_H

#include<list>
#include<filesystem>
#include"rule.h"

namespace parser {

class Parser{
public:
    virtual ~Parser() = default;
    virtual void parse() = 0;
    virtual void set_path(std::filesystem::path) = 0;
    virtual std::filesystem::path get_path() = 0;
    virtual std::list<Rule*>& list_of_rules() = 0;
};

}
#endif // PARSER_H
