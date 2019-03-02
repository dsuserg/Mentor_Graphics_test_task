#include"functions.h"

using namespace std;

set<string> parser::functions::recursive_search_names(filesystem::path pth, regex pattern){

    set<string> s;

    for(auto &it: filesystem::recursive_directory_iterator(pth)){
        string tmp{it.path().filename().string()};
        if(regex_search(tmp,pattern)){
            s.insert(tmp);
        }
    }

    return s;
}

set<filesystem::path> parser::functions::recursive_search_paths(filesystem::path pth, regex pattern){

    set<filesystem::path> s;

    for(auto &it: filesystem::recursive_directory_iterator(pth)){
        if(regex_search(it.path().string(),pattern)){
            s.insert(it.path());
        }
    }

    return s;
}

set<filesystem::path> parser::functions::set_of_paths(filesystem::path pth, regex pattern){
    set<filesystem::path> s;

    for(auto& it: filesystem::directory_iterator(pth)){
        if(regex_search(it.path().string(), pattern)) s.insert(it.path());
    }

    return  s;
}
