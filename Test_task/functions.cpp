#include"functions.h"

using namespace std;

set<string> recursive_search(filesystem::path pth, regex pattern){
    set<string> s;

    for(auto &it: filesystem::recursive_directory_iterator(pth)){
        string tmp{it.path().filename().string()};
        if(regex_search(tmp,pattern)){
            s.insert(tmp);
        }
    }

    return s;
}
