#ifndef RULE_DIR_EXIST_H
#define RULE_DIR_EXIST_H
#include<filesystem>
#include<initializer_list>
#include<regex>
#include<set>
#include"rule_base.h"
namespace parser {

class Rule_coexist: parser_impl::Rule_base{
public:
    Rule_coexist() = default;
    Rule_coexist(std::regex, std::function<bool(std::filesystem::path)>);
    Rule_coexist(std::initializer_list<std::regex>, std::function<bool(std::filesystem::path)>);
    ~Rule_coexist() override = default;

    bool start_enforcement(std::filesystem::path path) override;
private:
    std::set<std::regex> d_set;
    std::function<bool(std::filesystem::path)> is_type;
};

}
#endif // RULE_DIR_EXIST_H
