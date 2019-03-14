#include "pch.h"
#include "my_tests.h"

int main(int argc, char* argv[]) {

	std::filesystem::path pth;

    if(argc == 1) {
        std::cout << "'log_path' was not provided"<< std::endl;
        exit(1);
    }

    else if (argc == 2) {
        if ( std::filesystem::exists(argv[1]) ) {
            pth = std::filesystem::absolute(argv[1]);
        }
        else {
            std::cout << "Given path does not exist" << std::endl;
            exit(1);
        }
    }

    std::set<std::filesystem::path> dirs_level_1{test_functions::set_of_paths(pth)};

    for(auto& dir1 : dirs_level_1){
        std::set<std::filesystem::path> dirs_level_2{test_functions::set_of_paths(dir1)};
        for(auto& dir2 : dirs_level_2){
			my_tests::custom_test_run(pth,dir2);
        }
    }
	
    return 0;
}
