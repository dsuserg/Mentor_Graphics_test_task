#include<regex>
#include<filesystem>
#include"parser_simple.h"
#include"rule_coexist.h"
#include"rule_same_file_set.h"
#include"rule_file_must_contain.h"

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


            for(auto& pth: filesystem::recursive_directory_iterator(dir_level_2)){
                string file_pth{pth.path()};
                if(regex_search(file_pth,regex("ft_run/\\d+/\\d+\\.stdout"))){
                        parser::Parser_simple file_parser{pth};
                        parser::Rule_file_must_contain r4{"Solver finished at"};
                        file_parser.list_of_rules().push_back(&r4);
                        file_parser.parse();
                        if(r4.is_errors()){
                            cout<<pth.path()<<endl;
                            cout<<r4.get_info()<<endl;
                    }
                }
            }
        }
    }

}
