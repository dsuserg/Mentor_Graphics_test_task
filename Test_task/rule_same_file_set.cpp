#include<algorithm>
#include"rule_same_file_set.h"
#include"functions.h"
#include<vector>
using namespace std;
using namespace parser;


Rule_same_file_set::Rule_same_file_set(string dir1, string dir2, regex pattern){
    this->dir1 = dir1;
    this->dir2 = dir2;
    this->pattern = pattern;
}


bool Rule_same_file_set::start_enforcement(filesystem::path pth){

    set<string> dir1 {functions::recursive_search_names(pth.string()+filesystem::path::preferred_separator+this->dir1, this->pattern)};
    set<string> dir2 {functions::recursive_search_names(pth.string()+filesystem::path::preferred_separator+this->dir2, this->pattern)};
    set<string> d1_d2;
    set<string> d2_d1;


    set_difference(dir1.begin(), dir1.end(), dir2.begin(), dir2.end(), inserter(d1_d2,d1_d2.end()));
    set_difference(dir2.begin(), dir2.end(), dir1.begin(), dir1.end(), inserter(d2_d1,d2_d1.end()));

    if(d1_d2.empty() && d2_d1.empty()){
        return 1;
    }

    else {
        if(d1_d2.empty()){
            this->info += "In " + this->dir1 + " there are missing files present in " + this->dir2 +":";
            for(const string& it: d2_d1){
                this->info += " '" + it + "',";
            }

            this->info.pop_back();
        }

        else if(d2_d1.empty()) {
            this->info += "In " + this->dir1 + " there are extra files files not present in " + this->dir2 +":";
            for(const string& it: d1_d2){
                this->info += " '" + it + "',";
            }
            this->info.pop_back();
        }

        else{
            this->info += "In " + this->dir1 + " there are missing files present in " + this->dir2 +":";
            for(const string& it: d2_d1){
                this->info += " '" + it + "',";
            }
            this->info.pop_back();

            this->info += "\nIn " + this->dir1 + " there are extra files files not present in " + this->dir2 +":";

            for(const string& it: d1_d2){
                this->info += " '" + it + "',";
            }

            this->info.pop_back();
        }

        this->errors = 1;
        return 0;
    }

}
