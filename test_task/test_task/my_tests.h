#pragma once

#include "pch.h"
#include "functions.h"

namespace my_tests {


    test_functions::Status custom_test(const std::filesystem::path& dir);
    test_functions::Status custom_test_run(const std::filesystem::path& dir, const std::initializer_list<std::ostream*>& out);

}