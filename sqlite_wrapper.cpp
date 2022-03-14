#include "sqlite_wrapper.h"

SqliteWrapper::SqliteWrapper(const char * db_name){
    this->db_name = db_name;
    sqlite3_open(db_name, &conx);
}

void SqliteWrapper::CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> column_dtype_pairs){
    // Creating the entry header strings from dtype mapping
    std::string header_dtype = "(";
    std::string header_nodtype = "(";

    std::unordered_map<std::string, std::string> column2dtype;
    for (auto const& pair : column_dtype_pairs){
        // pair:  column_name , dtype
        header_dtype.append(pair.first + " " + pair.second + ", ");
        header_nodtype.append(pair.first + ", ");
        column2dtype.insert(pair);
    }

    header_dtype = header_dtype.substr(0, header_dtype.size() - 2) + ")";
    header_nodtype = header_nodtype.substr(0, header_nodtype.size() - 2) + ")";

    this->table_entry_headers_dtype.insert({table_name, header_dtype});
    this->table_entry_headers_nodtype.insert({table_name, header_nodtype});
    this->table2colname2dtype.insert({table_name, column2dtype});

    if (this->debug){
        std::cout << "Creating Table with following headers:" << std::endl;
        std::cout << "\t\tDtype     " << header_dtype << std::endl;
        std::cout << "\t\tNo Dtype  " << header_nodtype << std::endl;
    }

    std::string create_table = "CREATE TABLE " + table_name + header_dtype + ";";

    char* messageError;
    int exit = sqlite3_exec(this->conx, create_table.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Error Creating Table" << std::endl;
        sqlite3_free(messageError);
    }
    sqlite3_close(this->conx);

    return;
}


