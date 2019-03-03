#ifndef RULE_H
#define RULE_H
#include<string>
#include<filesystem>

namespace parser {
class Rule{
public:
    virtual ~Rule() = default;

    virtual bool start_enforcement(std::filesystem::path) = 0;
    virtual bool is_errors() = 0;
    virtual std::string get_info() = 0;
};
}
#endif // RULE_H
