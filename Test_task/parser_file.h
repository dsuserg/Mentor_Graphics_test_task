#ifndef PARSER_FILE_H
#define PARSER_FILE_H

#include"parser_base.h"

namespace parser {

class Parser_file:public parser_impl::Parser_base{
    void parse() override;
};

}
#endif // PARSER_FILE_H
