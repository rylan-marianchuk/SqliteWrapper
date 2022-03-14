#ifndef SQLITEWRAPPER_SQLITE_WRAPPER_H
#define SQLITEWRAPPER_SQLITE_WRAPPER_H

#include <string>
#include <unordered_map>
#include <map>
#include <sqlite3.h>
#include <iostream>
#include <any>
#include <vector>

class SqliteWrapper {
public:
    const char * db_name;
    std::unordered_map<std::string, std::string> table_entry_headers_nodtype;
    std::unordered_map<std::string, std::string> table_entry_headers_dtype;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> table2colname2dtype;
    sqlite3 * conx;
    bool debug = true;

    SqliteWrapper(const char * db_name);

    void CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> column_dtype_pairs);

    std::unordered_map<std::string, std::any> RandomBatchQuery(std::string table_name, std::vector<std::string> columns, int batch_size);
};


#endif //SQLITEWRAPPER_SQLITE_WRAPPER_H
