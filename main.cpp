#include <iostream>
#include "include/sqlite_wrapper.h"
#include <string>


double * curvelengths = new double[5];
double * histentropy = new double[5];

int callback(void *NotUsed, int argc, char **argv, char **azColName){
    // Run for every row returned in query
    // int argc: holds the number of columns
    // (array) azColName: holds each column returned
    // (array) argv: holds each value
    static int i = 0;

    curvelengths[i] = strtod(argv[3], nullptr);
    histentropy[i] = strtod(argv[4], nullptr);

    /*
    for(int i = 0; i < argc; i++) {
        // Show column name, value, and newline
        if ( i > 1){
            double d = strtod(argv[i], nullptr);
            std::cout << d << "  " << typeid(d).name() << std::endl;
            //std::cout << azColName[i] << ": " << reinterpret_cast<float>(argv[i]) << std::endl;
        }
        else{
            std::cout << azColName[i] << ": " << argv[i] << "  " << typeid(argv[i]).name() << std::endl;
        }


    }

    // Insert a newline
    std::cout << std::endl;
    */
    // Return successful
    i++;
    return 0;
}

int main(){

    sqlite3 * db;

    sqlite3_open("/home/rylan/CLionProjects/preprocess/wvfm_params.db", &db);

    char *zErrMsg = nullptr;
    int rc;
    std::string sql = "SELECT * FROM wvfm_params WHERE EUID IN (SELECT EUID FROM wvfm_params ORDER BY RANDOM() LIMIT 5);";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    for (int j = 0; j < 5; j++){
        std::cout << "Entry " << j << ":  " << curvelengths[j] << std::endl;
    }


    delete[] curvelengths;
    delete[] histentropy;
    return 0;
}




