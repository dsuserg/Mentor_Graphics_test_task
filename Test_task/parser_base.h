#ifndef PARSER_BASE_H
#define PARSER_BASE_H

#include<parser.h>

namespace parser_impl {
using namespace parser;
class Parser_base:public Parser{
public:
    void set_path(std::filesystem::path) override;
    std::filesystem::path get_path() override;
    std::list<Rule*>& list_of_rules() override;
private:
    std::filesystem::path path;
    std::list<Rule*> rules;
};
}
#endif // PARSER_BASE_H
