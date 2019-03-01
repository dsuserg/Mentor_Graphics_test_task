#include<regex>
#include<filesystem>
#include"functions.h"
#include"parser_simple.h"
#include"rule_coexist.h"
#include"rule_same_file_set.h"
#include"rule_file_must_include.h"
#include"rule_file_must_exclude.h"
#include"rule_diff_param.h"
#include<set>
using namespace std;
using namespace filesystem;

bool lol(const filesystem::path& pth){return filesystem::is_directory(pth);}
#include"tst.h"
int main()
{
    filesystem::path pth{"./logs"};
    for(auto& dir_level_1: filesystem::directory_iterator(pth)){
        for(auto& dir_level_2: filesystem::directory_iterator(dir_level_1)){
            parser::Parser_simple prs{dir_level_2};
            parser::Rule_coexist r1{{"ft_run", "ft_reference"},lol};
            prs.list_of_rules().push_back(&r1);
            parser::Rule_same_file_set r2{"ft_run", "ft_reference", regex{"\\d+\\.stdout"}};
            prs.list_of_rules().push_back(&r2);
            prs.parse();

            if(r1.is_errors()){
                cout<<dir_level_2.path()<<endl;
                cout<<"directory missing:"<< r1.get_info() <<endl;
                continue;
            }
            if(r2.is_errors()){
                cout<<dir_level_2.path()<<endl;
                cout<<r2.get_info()<<endl;
                continue;
            }

            set<filesystem::path> files_of_ft_run{ parser::functions::recursive_search_paths(dir_level_2, regex{"ft_run/\\d+/\\d+\\.stdout"})};

            for(auto& pth:files_of_ft_run){
                parser::Parser_simple file_parser{pth};
                parser::Rule_file_must_include r4{"Solver finished at", regex::icase};
                file_parser.list_of_rules().push_back(&r4);
                parser::Rule_file_must_exclude r5{"error", regex::icase};
                file_parser.list_of_rules().push_back(&r5);
                file_parser.parse();
                if(r4.is_errors()){
                    cout<<pth<<endl;
                    cout<<r4.get_info()<<endl;
                    continue;
                }
                if(r5.is_errors()){
                    cout<<pth<<endl;
                    cout<<r5.get_info()<<endl;
                    continue;
                }
            }

            set<filesystem::path> files_of_ft_reference{ parser::functions::recursive_search_paths(dir_level_2, regex{"ft_reference/\\d+/\\d+\\.stdout"})};
            for(auto pth1 = files_of_ft_run.begin(), pth2 = files_of_ft_reference.begin();
                 pth1 != files_of_ft_run.end() && pth2 != files_of_ft_reference.end();
                ++pth1, ++pth2){
                parser::Parser_simple parse_files{dir_level_2};
                parser::Rule_diff_param r6{*pth1, *pth2, "Memory Working Set Peak", 4};
                parse_files.list_of_rules().push_back(&r6);
                parser::Rule_diff_param r7{*pth1, *pth2, "MESH::Bricks: Total", 0.1};
                parse_files.list_of_rules().push_back(&r7);
                parse_files.parse();
                if(r6.is_errors()){
                    cout<<parse_files.get_path()<<endl;
                    cout<<r6.get_info()<<endl;
                    continue;
                }

                if(r7.is_errors()){
                    cout<<parse_files.get_path()<<endl;
                    cout<<r7.get_info()<<endl;
                    continue;
                }
            }



//            for(auto& pth:files_of_ft_run){
//                parser::Parser_simple file_parser{pth};
//                parser::Rule_diff_param(pth,)
//            }
        }

    }

}
