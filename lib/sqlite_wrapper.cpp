#include "../include/sqlite_wrapper.h"

SqliteWrapper::SqliteWrapper(const char * db_name){
    this->db_name = db_name;
    sqlite3_open(db_name, &conx);
}

void SqliteWrapper::CloseDB() {
    sqlite3_close(this->conx);
}

void SqliteWrapper::CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> column_dtype_pairs){
    // Creating the entry header strings from dtype mapping
    std::string header_dtype = "(";
    std::string header_nodtype = "(";

    for (auto const& pair : column_dtype_pairs){
        // pair:  column_name , dtype
        header_dtype.append(pair.first + " " + pair.second + ", ");
        header_nodtype.append("@" + pair.first + ", ");
    }

    header_dtype = header_dtype.substr(0, header_dtype.size() - 2) + ")";
    header_nodtype = header_nodtype.substr(0, header_nodtype.size() - 2) + ")";

    this->table_entry_headers_dtype.insert({table_name, header_dtype});
    this->table_entry_headers_nodtype.insert({table_name, header_nodtype});
    this->table2ordered_colname_dtype.insert({table_name, column_dtype_pairs});

    if (this->debug){
        std::cout << "Creating Table with following headers:" << std::endl;
        std::cout << "\t\tDtype     " << header_dtype << std::endl;
        std::cout << "\t\tNo Dtype  " << header_nodtype << std::endl;
    }

    std::string create_table = "CREATE TABLE IF NOT EXISTS " + table_name + header_dtype + ";";

    char* messageError;
    int exit = sqlite3_exec(this->conx, create_table.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Error Creating Table" << std::endl;
        sqlite3_free(messageError);
    }

    return;
}


int SqliteWrapper::BatchInsert(std::string table_name, std::vector<std::variant<int*, double*, std::string*>> insert_arrays, int batch_size){



    std::vector<int> int_types;
    for (auto & varnt : insert_arrays){
        int_types.push_back(varnt.index());
    }

    sqlite3_stmt * statement;
    const char * tail = 0;
    char * sErrMsg = 0;
    sqlite3_exec(this->conx, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);

    std::string sqlstr = "INSERT OR IGNORE INTO " + table_name + " VALUES " + this->table_entry_headers_nodtype[table_name];
    sqlite3_prepare_v2(this->conx, sqlstr.c_str(), 256, &statement, &tail);


    sqlite3_exec(this->conx, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);

    for (int i = 0; i < batch_size; i++){

        for (int j = 0; j < insert_arrays.size(); j++){
            int int_type = int_types[j];
            if (int_type == 0){
                auto v = std::get<0>(insert_arrays[j])[i];
                sqlite3_bind_int(statement, j+1, v);
            }
            else if (int_type == 1){
                auto v = std::get<1>(insert_arrays[j])[i];
                sqlite3_bind_double(statement, j+1, v);
            }
            else if (int_type == 2){
                auto v = std::get<2>(insert_arrays[j])[i];
                sqlite3_bind_text(statement, j+1, v.c_str(), -1, SQLITE_TRANSIENT);
            }
        }

        sqlite3_step(statement);
        sqlite3_clear_bindings(statement);
        sqlite3_reset(statement);
    }

    sqlite3_exec(this->conx, "END TRANSACTION", NULL, NULL, &sErrMsg);
    sqlite3_finalize(statement);
    return 0;
}

std::vector<std::variant<int*, double*, std::string*>> SqliteWrapper::RandomBatchQuery(
        std::string table_name,
        std::string primary_key_name,
        std::vector<std::pair<std::string, std::string>> columns,
        int batch_size){

    std::vector<std::variant<int*, double*, std::string*>> results;

    std::string select_cols;

    for (auto & column_dtype_pair : columns){
        if (column_dtype_pair.second == "INT"){
            int * intP = new int[batch_size];
            results.push_back(intP);
        }
        else if (column_dtype_pair.second == "REAL"){
            double * dblP = new double[batch_size];
            results.push_back(dblP);
        }
        else if (column_dtype_pair.second == "TEXT"){
            std::string * strP = new std::string[batch_size];
            results.push_back(strP);
        }
        select_cols.append(column_dtype_pair.first + ", ");
    }
    select_cols = select_cols.substr(0, select_cols.size() - 2);

    if (this->debug){
        std::cout << select_cols << std::endl;

        for (int i = 0; i < results.size(); i++){
            switch(results[i].index()){
                case 0:
                    std::cout << std::get<0>(results[i]) << "  " << typeid(std::get<0>(results[i])).name() << std::endl;
                    break;
                case 1:
                    std::cout << std::get<1>(results[i]) << "  " << typeid(std::get<1>(results[i])).name() << std::endl;
                    break;
                case 2:
                    std::cout << std::get<2>(results[i]) << "  " << typeid(std::get<2>(results[i])).name() << std::endl;
                    break;
            }
        }
    }

    int rc;
    sqlite3_stmt * stmt;
    std::string sql_stmt = "SELECT " + select_cols + " FROM " + table_name + " WHERE " + primary_key_name + " IN (SELECT " +
            primary_key_name + " FROM " + table_name + " ORDER BY RANDOM() LIMIT " + std::to_string(batch_size) + ");";

    rc = sqlite3_prepare_v2(this->conx, sql_stmt.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK){
        //std::cout << "Error in RandomBatchQuery(): " << sqlite3_errmsg(this->conx);
        std::cout << "Error in RandomBatchQuery(): ";
        return results;
    }
    int i = 0;

    while( (rc = sqlite3_step(stmt)) == SQLITE_ROW){
        for (int j = 0; j < columns.size(); j++){
            if (columns[j].second == "INT"){
                std::get<0>(results[j])[i] = sqlite3_column_int(stmt, j);
            }
            else if (columns[j].second == "REAL"){
                std::get<1>(results[j])[i] = sqlite3_column_double(stmt, j);
            }
            else if (columns[j].second == "TEXT"){
                const unsigned char * uchar_entry = sqlite3_column_text(stmt, j);
                std::string str_entry(reinterpret_cast< char const* >(uchar_entry));
                std::get<2>(results[j])[i] = str_entry;
            }
        }
        i++;
    }

    if (rc != SQLITE_DONE) {
        std::cout << "Error in RandomBatchQuery(): ";
    }
    sqlite3_finalize(stmt);

    return results;
}
