#ifndef PARSER_DIR_H
#define PARSER_DIR_H

#include"parser_base.h"

namespace parser {

class Parser_dir:public parser_impl::Parser_base{
    void parse() override;
};

}
#endif // PARSER_DIR_H
