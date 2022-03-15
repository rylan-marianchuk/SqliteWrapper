#include "sqlite_wrapper.h"
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <cmath>

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

    std::string euids[5] = {"e987", "e121", "e813", "e649", "e447"};
    int leads[5] = {0, 1, 2, 3, 4};
    int nochange20[5] = {0, 0, 1, 0, 0};
    float cl[5] =
            {12780.905273,
            11180.522461,
            9742.398438,
            8122.770508,
            12945.327148};

    float histent[5] =
            {0.812686,
            0.469909,
            0.631023,
            0.528284,
            0.840711};

    std::vector<std::variant<int*, float*, std::string*>> insert_arrays {
            euids,
            leads,
            nochange20,
            cl,
            histent
    };

    sqlwrap.BatchInsert("waveform_features", insert_arrays, 5);


    sqlwrap.CloseDB();

    float f1 = 1.0f;
    float f2 = 1.0f;
    double result = std::sqrt(f1+f2);
    std::cout << typeid(0.0d).name() << std::endl;
    return 0;
}



