#ifndef SQLITEWRAPPER_SQLITE_WRAPPER_H
#define SQLITEWRAPPER_SQLITE_WRAPPER_H

#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include <iostream>
#include <variant>
#include <vector>

class SqliteWrapper {
public:
    std::string path_to_db;
    std::unordered_map<std::string, std::string> table_entry_headers_nodtype;
    std::unordered_map<std::string, std::string> table_entry_headers_dtype;
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> table2ordered_colname_dtype;
    sqlite3 * conx;
    bool debug = true;

    SqliteWrapper(std::string db_name);

    void CloseDB();

    void CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> column_dtype_pairs);

    int BatchInsert(std::string table_name, std::vector<std::variant<int*, double*, std::string*>> insert_arrays, int batch_size);

    std::vector<std::variant<int*, double*, std::string*>> RandomBatchQuery(std::string table_name,
                                                                            std::string primary_key_name,
                                                                            std::vector<std::pair<std::string, std::string>> columns,
                                                                            std::vector<std::string> restrictions,
                                                                            int batch_size);

    unsigned long GetNumRows(std::string table_name);
};


#endif //SQLITEWRAPPER_SQLITE_WRAPPER_H
