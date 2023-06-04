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

// Reading all data from every table
void read_all_datas(MYSQL* conn, const char* table) {
    char query[100];
    
    snprintf(query, sizeof(query), "SELECT * from %s as f, endereco as e WHERE e.id_endereco = f.id_endereco",table);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	printf("\nPersonal info from funcionario (%s) - ID (%s)",row[2],row[0]);
            std::cout << "\nID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", Funcao: " << row[5] << "\n\nAdress info:\nID Endereço: " << row[7] << ", Lougradouro: " << row[8] << ", Cep: " << row[9] << ", Bairro: " << row[10] << std::endl;
		}
	}

}

/////////////////////////////////
//Crud MYSQL - FUNCINARIO TABLE//
/////////////////////////////////

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
    snprintf(query, sizeof(query), "SELECT * from funcionario WHERE id_funcionario = %d", id);

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

/////////////////////////////
//Crud MYSQL - MEDICO TABLE//
/////////////////////////////

// Function to create a medico
void createMedico (MYSQL* conn, const char* CPF, const char* nome, const char* email, const char* telefone, int id_hospital,const char* logradouro, const char* especialidade, const char* cep, const char* numero, const char* bairro) {
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

    //Creating Medico
	
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_funcionario = "INSERT INTO medicos (CPF, nome, email, telefone ,especialidade, id_endereco, id_hospital) VALUES (?,?,?,?,?,?,?)";
	    mysql_stmt_prepare(stmt, insert_query_funcionario, strlen(insert_query_funcionario));
	
	    MYSQL_BIND funcionario[7];
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
	    funcionario[4].buffer = (void*)especialidade;
	    funcionario[4].buffer_length = strlen(especialidade);
	    
	    // Convert lastInsertId to a string
	    char lastInsertIdStr[20];
	    sprintf(lastInsertIdStr, "%i", lastInsertId);
	    
	    funcionario[5].buffer_type = MYSQL_TYPE_STRING;
	    funcionario[5].buffer = lastInsertIdStr;
	    funcionario[5].buffer_length = strlen(lastInsertIdStr);
	    
	    funcionario[6].buffer_type = MYSQL_TYPE_LONG;
    	funcionario[6].buffer = (void*)&id_hospital;
		
	    mysql_stmt_bind_param(stmt, funcionario);
	    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a medico by ID
void readMedico(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from medicos WHERE id_medicos = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Personal info from medico (%s) - ID (%s)\n\n",row[2],row[0]);
        	
            std::cout << "ID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", Funcao: " << row[5] << std::endl;
            
            freeResultSet(res);
            
            snprintf(query, sizeof(query), "SELECT * FROM endereco WHERE id_endereco = %s", row[6]);
        	MYSQL_RES* res = executeQuery(conn, query);
        	
		    if (res != nullptr) {
		        MYSQL_ROW adress_info;
		        
		        while ((adress_info = mysql_fetch_row(res)) != nullptr) {
		        	
		        	printf("\nAdress info from medico\n\n");
		        	
		            std::cout << "Logradouro: " << adress_info[1] << ", CEP: " << adress_info[2] << ", Numero: " << adress_info[3] << ", Bairro: " << adress_info[4] << std::endl;
		        }
		
		        freeResultSet(res);
		    }
        
		}

    }
    
}

// Function to update a medico by ID
void updateMedico(MYSQL* conn, int id,int id_hospital, const char* newCPF, const char* newnome, const char* newemail, const char* newtelefone, const char* newlogradouro, const char* newespecialidade, const char* newcep, const char* newnumero, const char* newbairro) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Selecting adress id
    
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM medicos WHERE id_medicos = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
	
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    mysql_free_result(res);
	
	//Updating medico
    const char* update_query = "UPDATE medicos SET CPF = ?, nome = ?, email = ?, telefone = ?, especialidade = ?, id_hospital = ? WHERE id_medicos = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[7];
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
    bind[4].buffer = (void*)newespecialidade;
    bind[4].buffer_length = strlen(newespecialidade);
    
    bind[5].buffer_type = MYSQL_TYPE_LONG;
    bind[5].buffer = (void*)&id_hospital;
    
    bind[6].buffer_type = MYSQL_TYPE_LONG;
    bind[6].buffer = (void*)&id;
    
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

// Function to delete a medico by ID
void deleteMedico(MYSQL* conn, int id) {
    char query[100];
    
    //Getting adress ID
    snprintf(query, sizeof(query), "SELECT * FROM medicos WHERE id_medicos = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    
    // Free the result set before executing additional queries
    mysql_free_result(res);
    
    //Del medico
    snprintf(query, sizeof(query), "DELETE FROM medicos WHERE id_medicos = %i", id);

    executeQuery(conn, query);
    
	//Del adress
	snprintf(query, sizeof(query), "DELETE FROM endereco WHERE id_endereco = %i", id_address);
	
	executeQuery(conn, query);
    //freeResultSet(res);   
}

///////////////////////////////
//Crud MYSQL - HOSPITAL TABLE//
///////////////////////////////

// Function to create a Hospital
void createHospital (MYSQL* conn, const char* nome, const char* email, const char* telefone, const char* logradouro, const char* cep, const char* numero, const char* bairro) {
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

    //Creating HOSPITAL
	
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_funcionario = "INSERT INTO hospital (nome, telefone, email, id_endereco) VALUES (?,?,?,?)";
	    mysql_stmt_prepare(stmt, insert_query_funcionario, strlen(insert_query_funcionario));
	
	    MYSQL_BIND funcionario[5];
	    memset(funcionario, 0, sizeof(funcionario));
	
	    funcionario[0].buffer_type = MYSQL_TYPE_STRING;
	    funcionario[0].buffer = (void*)nome;
	    funcionario[0].buffer_length = strlen(nome);
	    
	    funcionario[1].buffer_type = MYSQL_TYPE_STRING;
	    funcionario[1].buffer = (void*)telefone;
	    funcionario[1].buffer_length = strlen(telefone);
	    
	    funcionario[2].buffer_type = MYSQL_TYPE_STRING;
	    funcionario[2].buffer = (void*)email;
	    funcionario[2].buffer_length = strlen(email);
	    
	    // Convert lastInsertId to a string
	    char lastInsertIdStr[20];
	    sprintf(lastInsertIdStr, "%i", lastInsertId);
	    
	    funcionario[3].buffer_type = MYSQL_TYPE_STRING;
	    funcionario[3].buffer = lastInsertIdStr;
	    funcionario[3].buffer_length = strlen(lastInsertIdStr);
		
	    mysql_stmt_bind_param(stmt, funcionario);
	    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a Hospital by ID
void readHospital(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from hospital WHERE id_hospital = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Info from Hospital (%s) - ID (%s)\n\n",row[1],row[0]);
        	
            std::cout << "ID: " << row[0] << ", Nome: " << row[1] << ", Telefone: " << row[2] << ", Email: " << row[3] << std::endl;
            
            freeResultSet(res);
            
            snprintf(query, sizeof(query), "SELECT * FROM endereco WHERE id_endereco = %s", row[4]);
        	MYSQL_RES* res = executeQuery(conn, query);
        	
		    if (res != nullptr) {
		        MYSQL_ROW adress_info;
		        
		        while ((adress_info = mysql_fetch_row(res)) != nullptr) {
		        	
		        	printf("\nAdress info from hospital\n\n");
		        	
		            std::cout << "Logradouro: " << adress_info[1] << ", CEP: " << adress_info[2] << ", Numero: " << adress_info[3] << ", Bairro: " << adress_info[4] << std::endl;
		        }
		
		        freeResultSet(res);
		    }
        
		}

    }
    
}

// Function to update a Hospital by ID
void updateHospital(MYSQL* conn, int id, const char* newnome, const char* newtelefone, const char* newemail, const char* newlogradouro, const char* newcep, const char* newnumero, const char* newbairro) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Selecting adress id
    
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM hospital WHERE id_hospital = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
	
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[4]);
    mysql_free_result(res);
	
	//Updating Hospital
    const char* update_query = "UPDATE hospital nome = ?, telefone = ?, email = ?, WHERE id_hospital = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)newnome;
    bind[0].buffer_length = strlen(newnome);
    
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)newtelefone;
    bind[1].buffer_length = strlen(newtelefone);
    
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void*)newemail;
    bind[2].buffer_length = strlen(newemail);
    
    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = (void*)&id;
    
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

// Function to delete a Hospital by ID
void deleteHospital(MYSQL* conn, int id) {
    char query[100];
    
    //Getting adress ID
    snprintf(query, sizeof(query), "SELECT * FROM hospital WHERE id_hospital = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[4]);
    
    // Free the result set before executing additional queries
    mysql_free_result(res);
    
    //Del hospital
    snprintf(query, sizeof(query), "DELETE FROM hospital WHERE id_hospital = %i", id);

    executeQuery(conn, query);
    
	//Del adress
	snprintf(query, sizeof(query), "DELETE FROM endereco WHERE id_endereco = %i", id_address);
	
	executeQuery(conn, query);
    freeResultSet(res);   
}

///////////////////////////////
//Crud MYSQL - PACIENTE TABLE//
///////////////////////////////

// Function to create a Paciente
void createPaciente (MYSQL* conn, const char* CPF, const char* nome, const char* email, const char* telefone, const char* logradouro, const char* alergias, const char* cep, const char* numero, const char* bairro) {
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

    //Creating Medico
	
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_funcionario = "INSERT INTO paciente (CPF, nome, email, telefone ,alergias, id_endereco) VALUES (?,?,?,?,?,?)";
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
	    funcionario[4].buffer = (void*)alergias;
	    funcionario[4].buffer_length = strlen(alergias);
	    
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

// Function to read a Paciente by ID
void readPaciente(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from paciente WHERE id_paciente = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Personal info from paciente (%s) - ID (%s)\n\n",row[2],row[0]);
        	
            std::cout << "ID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", Funcao: " << row[5] << std::endl;
            
            freeResultSet(res);
            
            snprintf(query, sizeof(query), "SELECT * FROM endereco WHERE id_endereco = %s", row[6]);
        	MYSQL_RES* res = executeQuery(conn, query);
        	
		    if (res != nullptr) {
		        MYSQL_ROW adress_info;
		        
		        while ((adress_info = mysql_fetch_row(res)) != nullptr) {
		        	
		        	printf("\nAdress info from paciente\n\n");
		        	
		            std::cout << "Logradouro: " << adress_info[1] << ", CEP: " << adress_info[2] << ", Numero: " << adress_info[3] << ", Bairro: " << adress_info[4] << std::endl;
		        }
		
		        freeResultSet(res);
		    }
        
		}

    }
    
}

// Function to update a Paciente by ID
void updatePaciente(MYSQL* conn, int id, const char* newCPF, const char* newnome, const char* newemail, const char* newtelefone, const char* newlogradouro, const char* newalergias, const char* newcep, const char* newnumero, const char* newbairro) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Selecting adress id
    
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM paciente WHERE id_paciente = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
	
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    mysql_free_result(res);
	
	//Updating paciente
    const char* update_query = "UPDATE paciente SET CPF = ?, nome = ?, email = ?, telefone = ?, alergias = ? WHERE id_paciente = ?";
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
    bind[4].buffer = (void*)newalergias;
    bind[4].buffer_length = strlen(newalergias);
    
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

// Function to delete a medico by ID
void deletePaciente(MYSQL* conn, int id) {
    char query[100];
    
    //Getting adress ID
    snprintf(query, sizeof(query), "SELECT * FROM paciente WHERE id_paciente = %d", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    
    int id_address = atoi(row[6]);
    
    // Free the result set before executing additional queries
    mysql_free_result(res);
    
    //Del paciente
    snprintf(query, sizeof(query), "DELETE FROM paciente WHERE id_paciente = %i", id);

    executeQuery(conn, query);
    
	//Del adress
	snprintf(query, sizeof(query), "DELETE FROM endereco WHERE id_endereco = %i", id_address);
	
	executeQuery(conn, query);
    //freeResultSet(res);   
}

//////////////////////////////
//Crud MYSQL - CLINICA TABLE//
//////////////////////////////

// Function to create a clinica
void createClinica (MYSQL* conn, const char* especialidade, int id_hospital) {
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    
    //Creating Clinica
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_clinica = "INSERT INTO clinica (especialidade, id_hospital) VALUES (?,?)";
	    mysql_stmt_prepare(stmt, insert_query_clinica, strlen(insert_query_clinica));
	
	    MYSQL_BIND clinica[3];
	    memset(clinica, 0, sizeof(clinica));
	
	    clinica[0].buffer_type = MYSQL_TYPE_STRING;
	    clinica[0].buffer = (void*)especialidade;
	    clinica[0].buffer_length = strlen(especialidade);
	    
	    clinica[1].buffer_type = MYSQL_TYPE_LONG;
	    clinica[1].buffer = (void*)&id_hospital;
		
	    mysql_stmt_bind_param(stmt, clinica);
	    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a clinica by ID
void readClinica(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from clinica WHERE id_clinica = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Info from Clinica (%s)\n\n",row[0]);
        	
            std::cout << "ID: " << row[0] << ", Especialidade: " << row[1] << ", Hospital atrelado: " << row[2] << std::endl;
            
            freeResultSet(res);
        
		}

    }
    
}

// Function to update a clinica by ID
void updateClinica(MYSQL* conn, int id, const char* especialidade, int id_hospital) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Updating Clinica
    const char* update_query = "UPDATE clinica SET especialidade = ?, id_hospital = ? WHERE id_clinica = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)especialidade;
    bind[0].buffer_length = strlen(especialidade);
    
    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = (void*)&id_hospital;
    
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (void*)&id;
    
    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);
    
    mysql_stmt_close(stmt);
}

// Function to delete a clinica by ID
void deleteClinica(MYSQL* conn, int id) {
    char query[100];
    
    //Del clinica
    snprintf(query, sizeof(query), "DELETE FROM clinica WHERE id_clinica = %i", id);
    executeQuery(conn, query);
    
}

// Function to create a prontuario

//////////////////////////////
//Crud MYSQL - DOENCA TABLE//
////////////////////////////

//Function that creates a doenca on MYSLQ DB
void createDoenca (MYSQL* conn, const char* nome, const char* remedio, const char* sintomas) {
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    
    //Creating doenca
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_clinica = "INSERT INTO doencas (nome, remedio, sintomas) VALUES (?,?,?)";
	    mysql_stmt_prepare(stmt, insert_query_clinica, strlen(insert_query_clinica));
	
	    MYSQL_BIND clinica[3];
	    memset(clinica, 0, sizeof(clinica));
		
	    clinica[0].buffer_type = MYSQL_TYPE_STRING;
	    clinica[0].buffer = (void*)nome;
	    clinica[0].buffer_length = strlen(nome);
		
		clinica[1].buffer_type = MYSQL_TYPE_STRING;
	    clinica[1].buffer = (void*)remedio;
	    clinica[1].buffer_length = strlen(remedio);    
	    
	    clinica[2].buffer_type = MYSQL_TYPE_STRING;
	    clinica[2].buffer = (void*)sintomas;
	    clinica[2].buffer_length = strlen(sintomas);
		
	    mysql_stmt_bind_param(stmt, clinica);
	    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a doenca by ID
void readDoenca(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from doencas WHERE id_doencas = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Info from Doenca (%s)\n\n",row[0]);
        	
            std::cout << "ID: " << row[0] << ", Nome: " << row[1] << ", Remedio: " << row[2] << ", Sintomas: " << row[3] << std::endl;
            
            freeResultSet(res);
        
		}

    }
    
}

// Function to update a doenca by ID
void updateDoencas(MYSQL* conn, int id, const char* nome, const char* remedio, const char* sintomas) {
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Updating doenca
    const char* update_query = "UPDATE doencas SET nome = ?, remedio = ?, sintomas = ?  WHERE id_doencas = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)nome;
    bind[0].buffer_length = strlen(nome);
    
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)remedio;
    bind[1].buffer_length = strlen(remedio);
    
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void*)sintomas;
    bind[2].buffer_length = strlen(sintomas);
    
    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = (void*)&id;
    
    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);
    
    mysql_stmt_close(stmt);
}

// Function to delete a clinica by ID
void deleteDoencas(MYSQL* conn, int id) {
    char query[100];
    
    //Del clinica
    snprintf(query, sizeof(query), "DELETE FROM doencas WHERE id_doencas = %i", id);
    executeQuery(conn, query);
    
}

//////////////////////////////////
//Crud MYSQL - PRONTUARIO TABLE//
////////////////////////////////

// Function to create a prontuario
void createProntuario (MYSQL* conn, int id_doenca, int id_paciente, int id_medico, const char* comentario) {
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    
    //Creating prontuario
	stmt = mysql_stmt_init(conn);
	
	    const char* insert_query_clinica = "INSERT INTO prontuario (id_doenca, id_paciente, id_medico, comentario) VALUES (?,?,?,?)";
	    mysql_stmt_prepare(stmt, insert_query_clinica, strlen(insert_query_clinica));
	
	    MYSQL_BIND clinica[4];
	    memset(clinica, 0, sizeof(clinica));
		
		clinica[0].buffer_type = MYSQL_TYPE_LONG;
	    clinica[0].buffer = (void*)&id_doenca;
	    
		clinica[1].buffer_type = MYSQL_TYPE_LONG;
	    clinica[1].buffer = (void*)&id_paciente;
	    
		clinica[2].buffer_type = MYSQL_TYPE_LONG;
	    clinica[2].buffer = (void*)&id_medico;	    
	    
	    clinica[3].buffer_type = MYSQL_TYPE_STRING;
	    clinica[3].buffer = (void*)comentario;
	    clinica[3].buffer_length = strlen(comentario);
		
	    mysql_stmt_bind_param(stmt, clinica);
	    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    
    return;
}

// Function to read a doenca by ID (NOT FINISHED YET)
void readProntuario(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from prontuario WHERE id_prontuario = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
        	printf("Info from Doenca (%s)\n\n",row[0]);
        	
            std::cout << "ID: " << row[0] << ", Nome: " << row[1] << ", Remedio: " << row[2] << ", Sintomas: " << row[3] << std::endl;
            
            freeResultSet(res);
        
		}

    }
    
}

int main() {
	//Colocando tipos de caracter para UTF-8
	setlocale(LC_ALL,"portuguese");
	
	
    const char* HOST = "localhost";
    const char* USER = "root";
    const char* PASS = "root";
    const char* DB_NAME = "hospital_v2";

    MYSQL* conn = connectToDatabase(HOST, USER, PASS, DB_NAME);
    if (conn == nullptr) {
        return 1;
    }
	
	// Read all datas from every table with adress (working)
    //read_all_datas(conn,"funcionario");
	
	//////////////////////
	//Funcionario DEBUG//
	////////////////////
	
    // Create a funcionario (working)
    //createFuncionario(conn, "456", "nome2","gmail2@gmail.com","456","logra2","dev2","456","45","vargem");

    // Read a funcionario by ID (working)
    //readFuncionario(conn, 3);

    // Update a funcionario (working)
    //updateFuncionario(conn,31 ,"555555","Gustavo","gmail.com","326695585","ze da manga","dev","3333333","130","aaaaaaaaa");
    
    //Del funcionnario (Working)
	//deleteFuncionario(conn,3);
	
	/////////////////
	//Medico DEBUG//
	///////////////
	
	// Create a Medico (working)
    //createMedico(conn, "456", "nome2","gmail2@gmail.com","456",3,"logra2","dev2","456","45","vargem");
    
    // Read a medico by ID (working)
    //readMedico(conn,1);
    
    // Update a medico (working)
    //updateMedico(conn,9,4,"555555","Gustavo","gmail.com","326695585","ze da manga","dev","3333333","130","aaaaaaaaa");
    
    // Delete medico (working)
    //deleteMedico(conn,9);
    
    ///////////////////
	//Hospital DEBUG///
	//////////////////
	
	//Create a HOSPITAL (working)
    //createHospital(conn,"nome3","gmail2@gmail.com","456","logra2","456","45","vargem");
    
    // Read a Hospital by ID (working)
    //readHospital(conn,1);
    
    // Update a Hospital (Update adress but not update hospital info...)
    //updateHospital(conn,1 ,"Gustavo","326695585","gmail.com","ze da manga","3333333","130","aaaaaaaaa");
    
    // Delete Hospital (working)
    //deleteHospital(conn,2);
    
    ///////////////////
	//Paciente DEBUG//
	/////////////////
	
	// Create a Paciente (working)
    //createPaciente(conn, "45666", "nome2","gmail2@gmail.com","456","logra2","banana","456","45","vargem");
    
    // Read a Paciente by ID (working)
    //readPaciente(conn,1);
    
    // Update a Paciente by ID (working)
    //updatePaciente(conn,3 ,"45666", "nome2","gmail2@gmail.com","456","logra2","pineapple","456","45","update");
    
    // Delete Paciente (working)
    //deletePaciente(conn,4);
    
    //////////////////
	//Clinica DEBUG//
	////////////////
    
    // Create a clinica(working)
    //createClinica(conn,"Gases",4);
    
    // Read a clinica by ID (working)
	//readClinica(conn,1);
	
    // Update a clinica by ID (working)
    //updateClinica(conn,2,"dale",3);
    
    // Delete Clinica (working)
    //deleteClinica(conn,1);
    
    //////////////////
	//DOENCAS DEBUG//
	////////////////
	
	// Create a doenca
	//createDoenca(conn,"doenca","dipirona monohidratada","febre");
    
	// Read a doenca by ID (WORKING)
	//readDoenca(conn,1);
	
	// Update a doenca by ID (WORKING)
	//updateDoencas(conn,1,"doenca novo_nome","remedin pra dormi","sonin");
	
	// Delete a doenca by ID (Working)
	//deleteDoencas(conn,1);
    
    /////////////////////
	//PRONTUARIO DEBUG//
	///////////////////
	
    // Create a prontuario(working)
    //createProntuario (conn,2,5,10,"Esse cara e poggers");
    
    // Read a prontuario by ID
	//readPronturario(conn,1);
		
    disconnectFromDatabase(conn);

    return 0;
}
