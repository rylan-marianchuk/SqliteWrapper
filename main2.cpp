#include "sqlite_wrapper.h"
#include <vector>
#include <string>


int main(){
    SqliteWrapper sqlwrap = SqliteWrapper("phony.db");

    std::vector<std::pair<std::string, std::string>> column2dtype {
            {"EUID", "TEXT PRIMARY KEY"},
            {"LEAD", "INT"},
            {"NOCHANGE20", "INT"},
            {"CURVELENGTH", "REAL"},
            {"HISTENTROPY", "REAL"}
    };
    sqlwrap.CreateTable("waveform_features", column2dtype);

    return 0;
}



