#include<iostream>
#include<regex>
#include<filesystem>
#include<set>
#include<fstream>
#include"functions.h"
#include"parser_simple.h"
#include"rule_coexist.h"
#include"rule_same_file_set.h"
#include"rule_file_must_include.h"
#include"rule_file_must_exclude.h"
#include"rule_diff_param.h"

using namespace std;
using namespace filesystem;


bool specify(const filesystem::path& pth){return filesystem::is_directory(pth);}

int main(int argc, char **argv){

    ostream* myostream{&std::cout};
    ofstream file;
    filesystem::path pth;

    if(argc == 1){
        *myostream << "Neither 'log_path' nor 'out_file' were provided"<< endl;
        exit(1);
    }

    if(argc == 2){
        if(filesystem::exists(argv[1])) {
            pth = filesystem::absolute(argv[1]);
        }
        else {
            cout<< "Given path does not exist"<<endl;
            exit(1);
        }
    }

    if(argc == 3){
        if(filesystem::exists(argv[1])) {
            pth = filesystem::absolute(argv[1]);
            file = ofstream{argv[2]};
            myostream = &file;
        }
        else {
            cout<< "Given path does not exist"<<endl;
            exit(1);
        }
    }

cout<<"Testing started"<<endl;

    set<filesystem::path> dirs_level_1{parser::functions::set_of_paths(pth)};

    for(auto& dir1: dirs_level_1){

        set<filesystem::path> dirs_level_2{parser::functions::set_of_paths(dir1)};

        for(auto& dir2: dirs_level_2){
            bool status{1};

//report.txt generation stayed due to task point, but final reports generated without it
            ofstream report{dir2.string()+filesystem::path::preferred_separator+"report.txt"};

            string test_path{dir2.parent_path().filename().string()
                             + filesystem::path::preferred_separator
                             + dir2.filename().string()
                             + filesystem::path::preferred_separator};

            parser::Parser_simple prs{dir2};
            parser::Rule_coexist r1{{"ft_run", "ft_reference"}, specify};
            prs.list_of_rules().push_back(&r1);
            parser::Rule_same_file_set r2{"ft_run", "ft_reference", regex{"\\d+\\.stdout"} };
            prs.list_of_rules().push_back(&r2);
            status &= prs.parse();

            if(!status){
                *myostream << "FAIL: " << test_path<<endl;
                if(r1.is_errors()){
                    stringstream temp;
                    temp << "directory missing:" << r1.get_info() <<endl;
                    report << temp.str();
                    *myostream << temp.str();

                    continue;
                }

                if(r2.is_errors()){
                    stringstream temp;
                    temp<< r2.get_info() << endl;
                    report<< temp.str();
                    *myostream<< temp.str();
                    continue;
                }
            }

//Check presence of "Solver finished at" and "error"
            set<filesystem::path> files_of_ft_run{ parser::functions::recursive_search_paths(dir2, regex{"ft_run/\\d+/\\d+\\.stdout"})};

            stringstream errors;

            for(auto& pth:files_of_ft_run){
                parser::Parser_simple file_parser{pth};
                parser::Rule_file_must_include r4{"Solver finished at", regex::icase};
                file_parser.list_of_rules().push_back(&r4);
                parser::Rule_file_must_exclude r5{"error", regex::icase};
                file_parser.list_of_rules().push_back(&r5);

                status &= file_parser.parse();


                if(r4.is_errors()){
                    errors << pth.parent_path().filename().string()<<
                              filesystem::path::preferred_separator<<
                              pth.filename().string()<<": "<<
                              r4.get_info()<<endl;
                    continue;
                }

                if(r5.is_errors()){
                    errors << pth.parent_path().filename().string()<<
                              filesystem::path::preferred_separator<<
                              pth.filename().string()<<
                              r5.get_info()<<endl;
                    continue;
                }
            }

//Diff the params
            set<filesystem::path> files_of_ft_reference{ parser::functions::recursive_search_paths(dir2, regex{"ft_reference/\\d+/\\d+\\.stdout"})};
            for(auto pth1 = files_of_ft_run.begin(), pth2 = files_of_ft_reference.begin();
                 pth1 != files_of_ft_run.end() && pth2 != files_of_ft_reference.end();
                ++pth1, ++pth2){

                parser::Parser_simple parse_files{dir2};
                parser::Rule_diff_param r6{*pth1, *pth2, "Memory Working Set Peak", 4};
                parse_files.list_of_rules().push_back(&r6);
                parser::Rule_diff_param r7{*pth1, *pth2, "MESH::Bricks: Total", 0.1};
                parse_files.list_of_rules().push_back(&r7);

                status &= parse_files.parse();

                if(r6.is_errors()){
                    string info = r6.get_info();
                    info = regex_replace(info, regex{"file1"}, "ft_run");
                    info = regex_replace(info, regex{"file2"}, "ft_reference");

                    errors << pth1->parent_path().filename().string()<<
                              filesystem::path::preferred_separator<<
                              pth1->filename().string()<<": "<<
                              "different 'Memory Working Set Peak' " <<
                              info << endl;
                    continue;
                }

                if(r7.is_errors()){
                    string info = r7.get_info();
                    info = regex_replace(info,regex{"file1"},"ft_run");
                    info = regex_replace(info,regex{"file2"},"ft_reference");

                    errors << pth1->parent_path().filename().string()<<
                              filesystem::path::preferred_separator<<
                              pth1->filename().string()<<": "<<
                              "different 'Total' of bricks " <<
                              info << endl;
                    continue;
                }
            }

            if(!status){
                *myostream<<"FAIL: "<<test_path<<endl;
                report<<errors.str();
                *myostream<<errors.str();
            }

            if(status){
                *myostream<<"OK: "<<test_path<<endl;
            }

        }

    }

cout<<"Testing ended"<<endl;
    return 0;
}
