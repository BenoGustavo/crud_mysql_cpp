#include <iostream>
#include <mysql.h>

// Function to establish a database connection
MYSQL* connectToDatabase(const char* host, const char* user, const char* pass, const char* db_name) {
    MYSQL* conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, host, user, pass, db_name, 0, NULL, 0)) {
        std::cout << "Failed to connect to the database: " << mysql_error(conn) << std::endl;
        return nullptr;
    }

    return conn;
}

// Function to disconnect from the database
void disconnectFromDatabase(MYSQL* conn) {
    if (conn != nullptr) {
        mysql_close(conn);
    }
}

// Function to execute a query and return the result set
MYSQL_RES* executeQuery(MYSQL* conn, const char* query) {
    if (mysql_query(conn, query)) {
        std::cout << "Query execution error: " << mysql_error(conn) << std::endl;
        return nullptr;
    }

    return mysql_use_result(conn);
}

// Function to free the result set
void freeResultSet(MYSQL_RES* res) {
    if (res != nullptr) {
        mysql_free_result(res);
    }
}

// Function to create a funcionario
void createFuncionario(MYSQL* conn, const char* CPF, const char* nome, const char* email, const char* telefone, const char* logradouro, const char* funcao, const char* cep, const char* numero, const char* bairro) {
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    
    //Creating Endereco
    
    const char* insert_query_endereco = "INSERT INTO endereco (logradouro, cep, numero, bairro) VALUES (?, ?, ?, ?)";
    mysql_stmt_prepare(stmt, insert_query_endereco, strlen(insert_query_endereco));

    MYSQL_BIND endereco_info[4];
    memset(endereco_info, 0, sizeof(endereco_info));
    
    endereco_info[0].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[0].buffer = (void*)cep;
    endereco_info[0].buffer_length = strlen(cep);
    
    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[1].buffer = (void*)numero;
    endereco_info[1].buffer_length = strlen(numero);
	
    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[2].buffer = (void*)bairro;
    endereco_info[2].buffer_length = strlen(bairro);
    
    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[3].buffer = (void*)logradouro;
    endereco_info[3].buffer_length = strlen(logradouro);
    
    mysql_stmt_bind_param(stmt, endereco_info);
    mysql_stmt_execute(stmt);

    int lastInsertId = mysql_stmt_insert_id(stmt);
    
    mysql_stmt_close(stmt);

    //Creating funcionario
	
	stmt = mysql_stmt_init(conn);
	
    const char* insert_query_funcionario = "INSERT INTO funcionario (CPF, nome, email, telefone ,funcao, id_endereco) VALUES (?,?,?,?,?,?)";
    mysql_stmt_prepare(stmt, insert_query_funcionario, strlen(insert_query_funcionario));

    MYSQL_BIND funcionario[6];
    memset(funcionario, 0, sizeof(funcionario));

    funcionario[0].buffer_type = MYSQL_TYPE_STRING;
    funcionario[0].buffer = (void*)CPF;
    funcionario[0].buffer_length = strlen(CPF);

    funcionario[1].buffer_type = MYSQL_TYPE_STRING;
    funcionario[1].buffer = (void*)nome;
    funcionario[1].buffer_length = strlen(nome);
    
    funcionario[2].buffer_type = MYSQL_TYPE_STRING;
    funcionario[2].buffer = (void*)email;
    funcionario[2].buffer_length = strlen(email);
    
    funcionario[3].buffer_type = MYSQL_TYPE_STRING;
    funcionario[3].buffer = (void*)telefone;
    funcionario[3].buffer_length = strlen(telefone);

    funcionario[4].buffer_type = MYSQL_TYPE_STRING;
    funcionario[4].buffer = (void*)funcao;
    funcionario[4].buffer_length = strlen(funcao);
    
    // Convert lastInsertId to a string
    char lastInsertIdStr[20];
    sprintf(lastInsertIdStr, "%i", lastInsertId);
    
    funcionario[5].buffer_type = MYSQL_TYPE_STRING;
    funcionario[5].buffer = lastInsertIdStr;
    funcionario[5].buffer_length = strlen(lastInsertIdStr);
	
    mysql_stmt_bind_param(stmt, funcionario);
    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a funcionario by ID
void readFuncionario(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM funcionario WHERE id_funcionario = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Personal info from funcionario (%s) - ID (%s)\n\n",row[2],row[0]);
        	
            std::cout << "ID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", Funcao: " << row[5] << std::endl;
            
            freeResultSet(res);
            
            snprintf(query, sizeof(query), "SELECT * FROM endereco WHERE id_endereco = %s", row[6]);
        	MYSQL_RES* res = executeQuery(conn, query);
        	
		    if (res != nullptr) {
		        MYSQL_ROW adress_info;
		        
		        while ((adress_info = mysql_fetch_row(res)) != nullptr) {
		        	
		        	printf("\nAdress info from funcionario\n\n");
		        	
		            std::cout << "Logradouro: " << adress_info[1] << ", CEP: " << adress_info[2] << ", Numero: " << adress_info[3] << ", Bairro: " << adress_info[4] << std::endl;
		        }
		
		        freeResultSet(res);
		    }
        
		}

    }
    
}

// Function to update a funcionario by ID
void updateFuncionario(MYSQL* conn, int id, const char* newCPF, const char* newnome, const char* newemail, const char* newtelefone, const char* newlogradouro, const char* newfuncao, const char* newcep, const char* newnumero, const char* newbairro) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Selecting adress id
    
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM funcionario WHERE id_funcionario = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
	
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    mysql_free_result(res);
	
	//Updating funcionario
    const char* update_query = "UPDATE funcionario SET CPF = ?, nome = ?, email = ?, telefone = ?, funcao = ? WHERE id_funcionario = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[6];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)newCPF;
    bind[0].buffer_length = strlen(newCPF);
    
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)newnome;
    bind[1].buffer_length = strlen(newnome);
    
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void*)newemail;
    bind[2].buffer_length = strlen(newemail);
    
    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (void*)newtelefone;
    bind[3].buffer_length = strlen(newtelefone);
    
    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = (void*)newfuncao;
    bind[4].buffer_length = strlen(newfuncao);
    
    bind[5].buffer_type = MYSQL_TYPE_LONG;
    bind[5].buffer = (void*)&id;
    
    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);
        
	//Updating adress
	
    const char* update_query_adress = "UPDATE endereco SET logradouro = ?, cep = ?, numero = ?, bairro = ? WHERE id_endereco = ?";
    mysql_stmt_prepare(stmt, update_query_adress, strlen(update_query_adress));

    MYSQL_BIND bind_adress[5];
    memset(bind_adress, 0, sizeof(bind_adress));

    bind_adress[0].buffer_type = MYSQL_TYPE_STRING;
    bind_adress[0].buffer = (void*)newlogradouro;
    bind_adress[0].buffer_length = strlen(newlogradouro);
    
    bind_adress[1].buffer_type = MYSQL_TYPE_STRING;
    bind_adress[1].buffer = (void*)newcep;
    bind_adress[1].buffer_length = strlen(newcep);
    
    bind_adress[2].buffer_type = MYSQL_TYPE_STRING;
    bind_adress[2].buffer = (void*)newnumero;
    bind_adress[2].buffer_length = strlen(newnumero);
    
    bind_adress[3].buffer_type = MYSQL_TYPE_STRING;
    bind_adress[3].buffer = (void*)newbairro;
    bind_adress[3].buffer_length = strlen(newbairro);
    
    bind_adress[4].buffer_type = MYSQL_TYPE_LONG;
    bind_adress[4].buffer = (void*)&id_address;
    
    mysql_stmt_bind_param(stmt, bind_adress);
    mysql_stmt_execute(stmt);
    
    mysql_stmt_close(stmt);
}


// Function to delete a funcionario by ID
void deleteFuncionario(MYSQL* conn, int id) {
    char query[100];
    
    //Getting adress ID
    snprintf(query, sizeof(query), "SELECT * FROM funcionario WHERE id_funcionario = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    
    // Free the result set before executing additional queries
    mysql_free_result(res);
    
    //Del funcionario
    snprintf(query, sizeof(query), "DELETE FROM funcionario WHERE id_funcionario = %i", id);

    executeQuery(conn, query);
    
	//Del adress
	snprintf(query, sizeof(query), "DELETE FROM endereco WHERE id_endereco = %i", id_address);
	
	executeQuery(conn, query);
    freeResultSet(res);   
}


int main() {
    const char* HOST = "localhost";
    const char* USER = "root";
    const char* PASS = "root";
    const char* DB_NAME = "hospital_v2";

    MYSQL* conn = connectToDatabase(HOST, USER, PASS, DB_NAME);
    if (conn == nullptr) {
        return 1;
    }
	
	
    // Create a funcionario (working)
    //createFuncionario(conn, "1234567890", "John Doe","john@gmail.com","2342342","logra","dev","242342422","45","vargem");

    // Read a funcionario by ID (working)
    //readFuncionario(conn, 37);

    // Update a funcionario (Update personal funcionario info but not the adress)
    updateFuncionario(conn,31 ,"555555","Gustavo","gmail.com","326695585","ze da manga","dev","3333333","130","aaaaaaaaa");
    
    //Del funcionnario (Working)
	//deleteFuncionario(conn,36);
	
    disconnectFromDatabase(conn);

    return 0;
}

