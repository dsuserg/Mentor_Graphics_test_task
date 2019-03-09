#pragma once

#include "functions.h"

namespace my_tests {

    void cl_args_handler(int argc, char* argv[]);
    test_functions::Status custom_test(const std::filesystem::path& dir);
    test_functions::Status custom_test_run(const std::filesystem::path& dir, const std::initializer_list<std::ostream*>& out);

}
