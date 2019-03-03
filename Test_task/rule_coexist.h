#ifndef RULE_DIR_EXIST_H
#define RULE_DIR_EXIST_H
#include<filesystem>
#include<initializer_list>
#include<functional>
#include<map>
#include"rule_base.h"
namespace parser {

class Rule_coexist: public parser_impl::Rule_base{
public:
    Rule_coexist() = default;
    Rule_coexist(std::string file, std::function<bool(std::filesystem::path)> is_file_type);
    Rule_coexist(std::initializer_list<std::string> file, std::function<bool(const std::filesystem::path&)> is_file_type);
    ~Rule_coexist() override = default;

    bool start_enforcement(std::filesystem::path) override;
    std::string get_info() override;
private:
    std::map<std::string,bool> dict;
    std::function<bool(const std::filesystem::path&)> is_type;
};

}
#endif // RULE_DIR_EXIST_H
