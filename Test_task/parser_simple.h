#ifndef PARSER_DIR_H
#define PARSER_DIR_H
#include<filesystem>
#include"parser_base.h"

using namespace std;

namespace parser {

class Parser_simple:public parser_impl::Parser_base{
public:
    Parser_simple(filesystem::path);
    void parse() override;
};

}
#endif // PARSER_DIR_H
