#include "include/sqlite_wrapper.h"
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <cmath>

int main(){
    SqliteWrapper sqlwrap = SqliteWrapper("./TEST.db");

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
    double cl[5] =
            {12780.905273,
            11180.522461,
            9742.398438,
            8122.770508,
            12945.327148};

    double histent[5] =
            {0.812686,
            0.469909,
            0.631023,
            0.528284,
            0.840711};

    std::vector<std::variant<int*, double*, std::string*>> insert_arrays {
            euids,
            leads,
            nochange20,
            cl,
            histent
    };

    sqlwrap.BatchInsert("waveform_features", insert_arrays, 5);

    /*
    std::cout << sqlwrap.GetNumRows("wvfm_params");
    std::vector<std::pair<std::string, std::string>> get_cols {
            {"EUID", "TEXT"},
            {"LEAD", "INT"},
            {"CURVELENGTH", "REAL"},
            {"HISTENTROPY", "REAL"},
    };

    std::vector<std::string> restrictions {
        "LEAD=5"
    };

    std::vector<std::variant<int*, double*, std::string*>> results_of_query = sqlwrap.RandomBatchQuery("wvfm_params", "EUID", get_cols, restrictions, 300);

    for (int i = 0; i < 300; i++){
        std::cout << "Entry: " << std::get<2>(results_of_query[0])[i] << " " << std::get<0>(results_of_query[1])[i] << " " << std::get<1>(results_of_query[2])[i] << " " << std::get<1>(results_of_query[3])[i] << std::endl;
    }
    */

    std::string * to_remove = new std::string [3] {
            "e987",
            "e121",
            "e813"
    };
    sqlwrap.BatchRemoveByKey("waveform_features", "EUID", to_remove, 3);
    sqlwrap.CloseDB();

    return 0;
}



