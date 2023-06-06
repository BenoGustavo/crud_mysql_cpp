#include <iostream>
#include <mysql.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

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

// Reading all data from every row in tables (funcionario,medicos,paciente)
void read_all_datas(MYSQL* conn, const char* table) {
    char query[100];
    
    snprintf(query, sizeof(query), "SELECT * from %s as f, endereco as e WHERE e.id_endereco = f.id_endereco",table);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	printf("\n-=Personal info from %s (%s) - ID (%s)=-",table,row[2],row[0]);
            std::cout << "\nID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", Funcao: " << row[5] << "\n\nAdress info:\nID EndereÃ§o: " << row[7] << ", Lougradouro: " << row[8] << ", Cep: " << row[9] << ", Numero residencial: " << row[10] << ", Bairro: " << row[11] << std::endl;
		}
	}

}

// Reading all IDs from every table
void read_all_ids(MYSQL* conn, const char* table) {
    char query[100];
    
    snprintf(query, sizeof(query), "SELECT id_%s FROM %s",table,table);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        printf("\nID's from all %s:\n",table);
        int count = 0;
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	
			count = count+1;
            std::cout << "ID: "<< row[0] << ",";
            
            if ((count % 5) == 0){
            	printf("\n");
			}
            
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
    endereco_info[0].buffer = (void*)logradouro;
    endereco_info[0].buffer_length = strlen(logradouro);
    
    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[1].buffer = (void*)cep;
    endereco_info[1].buffer_length = strlen(cep);
    
    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[2].buffer = (void*)numero;
    endereco_info[2].buffer_length = strlen(numero);
	
    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[3].buffer = (void*)bairro;
    endereco_info[3].buffer_length = strlen(bairro);
    
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
    endereco_info[0].buffer = (void*)logradouro;
    endereco_info[0].buffer_length = strlen(logradouro);
    
    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[1].buffer = (void*)cep;
    endereco_info[1].buffer_length = strlen(cep);
    
    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[2].buffer = (void*)numero;
    endereco_info[2].buffer_length = strlen(numero);
	
    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[3].buffer = (void*)bairro;
    endereco_info[3].buffer_length = strlen(bairro);
    
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
    endereco_info[0].buffer = (void*)logradouro;
    endereco_info[0].buffer_length = strlen(logradouro);
    
    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[1].buffer = (void*)cep;
    endereco_info[1].buffer_length = strlen(cep);
    
    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[2].buffer = (void*)numero;
    endereco_info[2].buffer_length = strlen(numero);
	
    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[3].buffer = (void*)bairro;
    endereco_info[3].buffer_length = strlen(bairro);
    
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

// Reads all the hospital table and their adress
void read_all_datas_hospital(MYSQL* conn, const char* table) {
    char query[100];
    
    snprintf(query, sizeof(query), "SELECT * from %s as f, endereco as e WHERE e.id_endereco = f.id_endereco",table);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res)) != nullptr) {
        	printf("\nHospital info ID (%s)",table,row[2],row[0]);
            std::cout << "\nNome: " << row[1] << ", Telefone: " << row[2] << ", Email: " << row[3] << "\n\nAdress info:\nID EndereÃ§o: " << row[4] << ", Lougradouro: " << row[6] << ", Cep: " << row[7] << ", Numero residencial: " << row[8] << ", Bairro: " << row[9] << std::endl;
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
	
	//Updating Hospital (FIXED, TINHA UMA ',' ANTES DO WHERE E NÃO TAVA FUNCIOONADNO HAHAHHAHAHAHAHAAHAHAHAHAHAHAHHAHA)
    const char* update_query = "UPDATE hospital SET nome = ?, telefone = ?, email = ? WHERE id_hospital = ?";
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
    
    // Closing and reinitializing the statement
    mysql_stmt_close(stmt);
    stmt = mysql_stmt_init(conn);
    
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
    endereco_info[0].buffer = (void*)logradouro;
    endereco_info[0].buffer_length = strlen(logradouro);
    
    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[1].buffer = (void*)cep;
    endereco_info[1].buffer_length = strlen(cep);
    
    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[2].buffer = (void*)numero;
    endereco_info[2].buffer_length = strlen(numero);
	
    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
    endereco_info[3].buffer = (void*)bairro;
    endereco_info[3].buffer_length = strlen(bairro);
    
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

// Function to read a doenca by ID
void readProntuario(MYSQL* conn, int id) {
    char query[100];
    snprintf(query, sizeof(query), "SELECT * from prontuario WHERE id_prontuario = %i", id);

    MYSQL_RES* res = executeQuery(conn, query);
    
    if (res != nullptr) {
        MYSQL_ROW row;
        
        row = mysql_fetch_row(res);
        	
        printf("INFO FROM PRONTUARIO\n");
        std::cout << "ID do prontuario: " << row[0] << ", ID da doenca: " << row[1] << ", ID do paciente: " << row[2] << ", ID do medico: " << row[3] << "\n" << "Comentario: " << row[4] << "\n" << std::endl;
        
        //Saving info from prontuario
        int id_doenca =  atoi(row[1]), id_paciente = atoi(row[2]), id_medico = atoi(row[3]);
        freeResultSet(res); //Limpando info do prontuario
        
        //Formating info from prontuario
        snprintf(query, sizeof(query), "SELECT * FROM hospital_v2.doencas AS d JOIN hospital_v2.paciente AS p JOIN hospital_v2.medicos AS m WHERE d.id_doencas = '%i' AND p.id_paciente = '%i' AND m.id_medicos = '%i'",id_doenca,id_paciente,id_medico);
    	res = executeQuery(conn, query);
        
        if (res != nullptr) {
	        MYSQL_ROW row;
	        
	        row = mysql_fetch_row(res);
	        	
	        printf("INFO FROM PACIENTE\n");
	        std::cout << "Nome (paciente): " << row[6] << ", CPF (paciente): " << row[5] << ", Email (Paciente) " << row[7] << "\nTelefone (Paciente): " << row[8] << ", Alergias (Paciente): " << row[9] << "\n"  << std::endl;
	        
			printf("INFO FROM MEDICO\n");
	        std::cout << "Nome (medico): " << row[13] << ", CPF (medico): " << row[12] << ", Email (medico) " << row[14] << ", Telefone (medico): " << row[15] << "\nEspecializacao (medico): " << row[16] << "\n" << std::endl;
	        
	        printf("INFO FROM DOENCA\n");
	        std::cout << "Nome (doenca): " << row[1] << ", Remedio: " << row[2] << ", Sintomas: " << row[3] << "\n" << std::endl;
        
    	}
    }
}

// Function to update a prontuario by ID
void updateProntuario(MYSQL* conn, const int id_prontuario, const int id_doenca, const int id_paciente, const int id_medico, const char* comentario){
	MYSQL_STMT* stmt = mysql_stmt_init(conn);
	
	//Updating prontuario
    const char* update_query = "UPDATE prontuario SET id_doenca = ?, id_paciente = ?, id_medico = ? , comentario = ? WHERE id_prontuario = ?";
    mysql_stmt_prepare(stmt, update_query, strlen(update_query));

    MYSQL_BIND bind[5];
    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (void*)&id_doenca;
    
	bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = (void*)&id_paciente;
    
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (void*)&id_medico;
    
    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (void*)comentario;
    bind[3].buffer_length = strlen(comentario);
    
    bind[4].buffer_type = MYSQL_TYPE_LONG;
    bind[4].buffer = (void*)&id_prontuario;
    
    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);
    
    mysql_stmt_close(stmt);
}

// Function to delete a prontuario by ID
void deleteProntuario(MYSQL* conn, int id){
	char query[100];
	
    snprintf(query, sizeof(query), "DELETE FROM prontuario WHERE id_prontuario = %i", id);
    executeQuery(conn, query);
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
    //updateHospital(conn,11 ,"Gustavo","326695585","gmail.com","ze da manga","3333333","130","aaaaaaaaa");
    
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
    
    // Read a prontuario by ID (Working very well)
	//readProntuario(conn,1);
	
	// Update a prontuario by ID (Working)
	//updateProntuario(conn,1,2,5,10,"funcionadeuspf");
	
	// Delete a prontuario by ID (Working)
	//deleteProntuario(conn,1);
	
	///////////////////
	//FINESHED DEBUG//
	/////////////////
	
	char menu,start;
	
	//Create variables
	std::string cpf_create,nome_create,email_create,telefone_create,logradouro_endereco,funcao_create,cep_endereco,numero_endereco,bairro_endereco;
	int ids,ids_create,id_hospital;
	
	int id_doencaxx,id_prontuarioxx;
	int id_pacientexx;
	int id_medicoxx;
	
	start:
	
	//Starting the main menu
	do{
		system("cls");
		
		printf("Hospital - Menu\nInsert the desired table:\n\n");
		printf("1 - Funcionario\n2 - Medico\n3 - Hospital\n4 - Paciente\n5 - Clinica\n6 - Doencas\n7 - Prontuario\n\nType 8 to exit.");
		menu = getche();
		
	}while(menu > '8' || menu < '1');
		
	switch(menu){
		
		//Funcionario (Complete)
		case '1':{
			do{
				system("cls");
				printf("Funcionario - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert the cargo: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						createFuncionario(conn,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
						do{
						system("cls");
						printf("Funcionario - Read\nWhat do you want to do?:\n\n");
						printf("1 - Read all\n2 - Read by ID\n\nType 3 to return.");
						menu = getche();
						
						}while(menu > '3' || menu < '1');
						
						switch(menu){
							//READ ALL
							case '1':{
								printf("ALL DATAS FROM FUNCIONARIO:\n\n");
								read_all_datas(conn,"funcionario");
								printf("\n");
								
								system("pause");
								goto start;
								break;
							}
							
							//READ BY ID
							case '2':{
								system("cls");
								std::cout << "0 to leave." << std::endl;
									
								read_all_ids(conn,"funcionario");
								
								do{	
									printf("\n-=-=-=-=*-*-=-=-=-=\n");
									std::cout << "\ninsert the id: " << std::endl;
									std::cin >> ids;
									
									readFuncionario(conn, ids);
								
								}while(ids != 0);
								
								goto start;
								break;
							}
							
							//Return
							case '3':{
								goto start;
								break;
							}
						}
						
						break;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						
						read_all_ids(conn,"funcionario");
						
						printf("\n");
						
						std::cout << "Insert the ID: " << std::endl;
						std::cin >> ids_create;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert the cargo: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						updateFuncionario(conn,ids_create,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						system("cls");
						printf("0 to return\n");
						read_all_ids(conn,"funcionario");
						
						std::cout << "\nInsert the ID: " << std::endl;
						std::cin >> ids_create;
						
						if(ids_create == 0){
							goto start;
						}
						
						deleteFuncionario(conn,ids_create);
						
						goto start;
						break;
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				}
				
				break;
			}while(menu > '5' || menu < '1');
		}
		
		//Medico (Complete)
		case '2':{
			do{
				system("cls");
				printf("Medico - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						printf("ID from hospitals:\n");
						read_all_ids(conn,"hospital");
						
						std::cout << "\nPersonal INFO:" << std::endl;
						
						std::cout << "Insert the hospital ID: " << std::endl;
						std::cin >> id_hospital;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert the especialização: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						createMedico(conn,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),id_hospital,logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
						do{
						system("cls");
						printf("Medico - Read\nWhat do you want to do?:\n\n");
						printf("1 - Read all\n2 - Read by ID\n\nType 3 to return.");
						menu = getche();
						
						}while(menu > '3' || menu < '1');
						
						switch(menu){
							//READ ALL
							case '1':{
								printf("\n\nALL DATAS FROM MEDICOS:\n\n");
								read_all_datas(conn,"medicos");
								printf("\n");
								
								system("pause");
								goto start;
								break;
							}
							
							//READ BY ID
							case '2':{
								system("cls");
								std::cout << "0 to leave." << std::endl;
									
								read_all_ids(conn,"medicos");
								
								do{	
									printf("\n-=-=-=-=*-*-=-=-=-=\n");
									std::cout << "\ninsert the id: " << std::endl;
									std::cin >> ids;
									
									readMedico(conn, ids);
								
								}while(ids != 0);
								
								goto start;
								break;
							}
							
							//Return
							case '3':{
								goto start;
								break;
							}
						}
						
						break;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						
						read_all_ids(conn,"medicos");
						
						read_all_ids(conn,"hospital");
						
						printf("\n");
						
						std::cout << "\nInsert the Medico ID: " << std::endl;
						std::cin >> ids_create;
						
						std::cout << "Insert the Hospital ID: " << std::endl;
						std::cin >> id_hospital;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert the especialização: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						updateMedico(conn,ids_create,id_hospital,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						system("cls");
						printf("0 to return\n");
						read_all_ids(conn,"medicos");
						
						std::cout << "\n\nInsert the ID: " << std::endl;
						std::cin >> ids_create;
						
						if(ids_create == 0){
							goto start;
						}
						
						deleteMedico(conn,ids_create);
						
						goto start;
						break;
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				
				}
				break;
				
			}while(menu > '5' || menu < '1');
		}
		
		//Hospital (Complete)
		case '3':{
			do{
				system("cls");
				printf("Hospital - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "\nPersonal INFO:" << std::endl;
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//Debuging variables
						//std::cout << nome_create << email_create << telefone_create << logradouro_endereco << cep_endereco << numero_endereco << bairro_endereco << std::endl;
						//system("pause");
						
						//I needed to tranform all the string values to char values...
						createHospital(conn,nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Read (Need to fix read all function)
					case '2':{
						do{
						system("cls");
						printf("Hospital - Read\nWhat do you want to do?:\n\n");
						printf("1 - Read all\n2 - Read by ID\n\nType 3 to return.");
						menu = getche();
						
						}while(menu > '3' || menu < '1');
						
						switch(menu){
							//READ ALL
							case '1':{
								printf("\n\nALL DATAS FROM MEDICOS:\n\n");
								read_all_datas_hospital(conn,"hospital");
								printf("\n");
								
								system("pause");
								goto start;
								break;
							}
							
							//READ BY ID
							case '2':{
								system("cls");
								std::cout << "0 to leave." << std::endl;
									
								read_all_ids(conn,"hospital");
								
								do{	
									printf("\n-=-=-=-=*-*-=-=-=-=\n");
									std::cout << "\ninsert the id: " << std::endl;
									std::cin >> ids;
									
									readHospital(conn, ids);
								
								}while(ids != 0);
								
								goto start;
								break;
							}
							
							//Return
							case '3':{
								goto start;
								break;
							}
						}
						
						break;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						
						read_all_ids(conn,"hospital");
						
						printf("\n");
						
						std::cout << "\nInsert the Hospital ID: " << std::endl;
						std::cin >> id_hospital;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values... (don't work)
						updateHospital(conn,id_hospital,nome_create.c_str(),telefone_create.c_str(),email_create.c_str(),logradouro_endereco.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						do{
							system("cls");
							printf("0 to return\n");
							read_all_ids(conn,"hospital");
							
							std::cout << "\n\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if (ids_create == 0){
								goto start;
							}
							
							deleteHospital(conn,ids_create);
						}while (1 == 1);
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				
				}
				break;
				
			}while(menu > '5' || menu < '1');
		}
		
		//Paciente (Complete)
		case '4':{
			do{
				system("cls");
				printf("Paciente - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert alergias: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						createPaciente(conn,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
						do{
						system("cls");
						printf("Paciente - Read\nWhat do you want to do?:\n\n");
						printf("1 - Read all\n2 - Read by ID\n\nType 3 to return.");
						menu = getche();
						
						}while(menu > '3' || menu < '1');
						
						switch(menu){
							//READ ALL
							case '1':{
								printf("ALL DATAS FROM PACIENTES:\n\n");
								read_all_datas(conn,"paciente");
								printf("\n");
								
								system("pause");
								goto start;
								break;
							}
							
							//READ BY ID
							case '2':{
								system("cls");
								std::cout << "0 to leave." << std::endl;
									
								read_all_ids(conn,"paciente");
								
								do{	
									printf("\n-=-=-=-=*-*-=-=-=-=\n");
									std::cout << "\ninsert the id: " << std::endl;
									std::cin >> ids;
									
									readPaciente(conn, ids);
								
								}while(ids != 0);
								
								goto start;
								break;
							}
							
							//Return
							case '3':{
								goto start;
								break;
							}
						}
						
						break;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Personal INFO:" << std::endl;
						
						read_all_ids(conn,"paciente");
						
						printf("\n");
						
						std::cout << "Insert the ID: " << std::endl;
						std::cin >> ids_create;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the CPF: " << std::endl;
						std::getline(std::cin, cpf_create);
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the email: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the telefone: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						std::cout << "insert alergias: " << std::endl;
						std::getline(std::cin, funcao_create);
						
						std::cout << "\nAdress INFO:\n" << std::endl;
						
						std::cout << "insert the logradouro: " << std::endl;
						std::getline(std::cin, logradouro_endereco);
						
						std::cout << "insert the CEP: " << std::endl;
						std::getline(std::cin, cep_endereco);
						
						std::cout << "insert the numero residencial: " << std::endl;
						std::getline(std::cin, numero_endereco);
						
						std::cout << "insert the bairro: " << std::endl;
						std::getline(std::cin, bairro_endereco);
						
						//I needed to tranform all the string values to char values...
						updatePaciente(conn,ids_create,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						do{
							system("cls");
							printf("0 to return\n");
							read_all_ids(conn,"paciente");
							
							std::cout << "\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if(ids_create == 0){
								goto start;
							}
							
							deletePaciente(conn,ids_create);
						}while(1 == 1);
						
						goto start;
						break;
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				}
				
				break;
			}while(menu > '5' || menu < '1');
		}
		
		//Clinica (Complete)
		case '5':{
			do{
				system("cls");
				printf("Clinica - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "Clinica INFO:" << std::endl;
						
						read_all_ids(conn,"hospital");
						
						std::cout << "\n\nInsert Especialização da clinica: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert hospital id: " << std::endl;
						std::cin >> ids;
						
						//I needed to tranform all the string values to char values...
						createClinica(conn,nome_create.c_str(),ids);
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
							system("cls");
							std::cout << "0 to leave." << std::endl;
									
							read_all_ids(conn,"clinica");
								
							do{	
								printf("\n-=-=-=-=*-*-=-=-=-=\n");
								std::cout << "\ninsert the id: " << std::endl;
								std::cin >> ids;
									
								readClinica(conn, ids);
								
							}while(ids != 0);
								
								goto start;
								break;
							}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Clinica INFO:" << std::endl;
						
						read_all_ids(conn,"clinica");
						
						printf("\n");
						
						read_all_ids(conn,"hospital");
						
						std::cout << "\n\nInsert the Clinica ID: " << std::endl;
						std::cin >> ids;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert Especialização da clinica: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "insert hospital id: " << std::endl;
						std::cin >> ids;
						
						//I needed to tranform all the string values to char values... (don't work)
						updateClinica(conn,ids,nome_create.c_str(),ids);
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						do{
							system("cls");
							printf("0 to return\n");
							read_all_ids(conn,"clinica");
							
							std::cout << "\n\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if (ids_create == 0){
								goto start;
							}
							
							deleteClinica(conn,ids_create);
						}while (1 == 1);
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				
				}
				break;
				
			}while(menu > '5' || menu < '1');
		}
		
		//Doencas
		case '6':{
			do{
				system("cls");
				printf("Doencas - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "DOENCAS INFO:" << std::endl;
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the remedio: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the sintomas: " << std::endl;
						std::getline(std::cin, telefone_create);
						
						//I needed to tranform all the string values to char values...
						createDoenca(conn,nome_create.c_str(),email_create.c_str(),telefone_create.c_str());
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
						system("cls");
						std::cout << "0 to leave." << std::endl;
									
						read_all_ids(conn,"doencas");
								
						do{	
							printf("\n-=-=-=-=*-*-=-=-=-=\n");
							std::cout << "\ninsert the id: " << std::endl;
							std::cin >> ids;
									
							readDoenca(conn, ids);
								
						}while(ids != 0);
								
						goto start;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "DOENCAS INFO:" << std::endl;
						
						read_all_ids(conn,"doencas");
						
						printf("\n");
						
						std::cout << "Insert the ID: " << std::endl;
						std::cin >> ids_create;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "Insert the nome: " << std::endl;
						std::getline(std::cin, nome_create);
						
						std::cout << "Insert the remedio: " << std::endl;
						std::getline(std::cin, email_create);
						
						std::cout << "insert the sintomas: " << std::endl;
						std::getline(std::cin, telefone_create);
				
						//I needed to tranform all the string values to char values...
						updateDoencas(conn,ids_create,nome_create.c_str(),email_create.c_str(),telefone_create.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						do{
							system("cls");
							printf("0 to return\n");
							read_all_ids(conn,"doencas");
							
							std::cout << "\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if(ids_create == 0){
								goto start;
							}
							
							deleteDoencas(conn,ids_create);
						}while(1 == 1);
						
						goto start;
						break;
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				}
				
				break;
			}while(menu > '5' || menu < '1');
		}
		
		//Prontuario
		case '7':{
			do{
				system("cls");
				printf("Prontuario - Menu\nWhat do you want to do?:\n");
				printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
				menu = getche();
				
				switch(menu){
					
					//Create
					case '1':{
						system("cls");
						
						std::cout << "Prontuario INFO:" << std::endl;
						
						read_all_ids(conn,"doencas");
						
						std::cout << "\nInsert the id doenca: " << std::endl;
						std::cin >> id_doencaxx;
						
						read_all_ids(conn,"paciente");
						
						std::cout << "\nInsert the id paciente: " << std::endl;
						std::cin >> id_pacientexx;
						
						read_all_ids(conn,"medicos");
						
						std::cout << "\nInsert the id medico: " << std::endl;
						std::cin >> id_medicoxx;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "\nInsert a comment about the PRONTUARIO: " << std::endl;
						std::getline(std::cin, nome_create);
						
						//I needed to tranform all the string values to char values...
						createProntuario(conn,id_doencaxx,id_pacientexx,id_medicoxx,nome_create.c_str());
						
						goto start;
						break;
					}
					
					//Read
					case '2':{
						system("cls");
						std::cout << "0 to leave." << std::endl;
									
						read_all_ids(conn,"prontuario");
								
						do{	
							printf("\n-=-=-=-=*-*-=-=-=-=\n");
							std::cout << "\ninsert the id: " << std::endl;
							std::cin >> ids;
							
							if(ids == 0){
								goto start;
							}
									
							readProntuario(conn, ids);
								
						}while(ids != 0);
								
						goto start;
					}
					
					//Update
					case '3':{
						system("cls");
						
						std::cout << "Prontuario INFO:" << std::endl;
						
						read_all_ids(conn,"prontuario");
						std::cout << "\nInsert the id prontuario: " << std::endl;
						std::cin >> id_prontuarioxx;
						
						read_all_ids(conn,"doencas");
						
						std::cout << "\nInsert the id doenca: " << std::endl;
						std::cin >> id_doencaxx;
						
						read_all_ids(conn,"paciente");
						
						std::cout << "\nInsert the id paciente: " << std::endl;
						std::cin >> id_pacientexx;
						
						read_all_ids(conn,"medicos");
						
						std::cout << "\nInsert the id medico: " << std::endl;
						std::cin >> id_medicoxx;
						
						std::cin.ignore();  // Discard the remaining newline character
						
						std::cout << "\nInsert a comment about the PRONTUARIO: " << std::endl;
						std::getline(std::cin, nome_create);
						
						//I needed to tranform all the string values to char values...
						updateProntuario(conn,id_prontuarioxx,id_doencaxx,id_pacientexx,id_medicoxx,nome_create.c_str());
						
						goto start;
						break;
					}
					
					//Delete
					case '4':{
						do{
							system("cls");
							printf("0 to return\n");
							read_all_ids(conn,"prontuario");
							
							std::cout << "\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if(ids_create == 0){
								goto start;
							}
							
							deleteProntuario(conn,ids_create);
						}while(1 == 1);
						
						goto start;
						break;
					}
					
					//return
					case '5':{
						goto start;
						break;
					}
				}
				
				break;
			}while(menu > '5' || menu < '1');
		}
		
		//Exit
		case '8':{
			printf("\n\n\nExiting...");
			break;
		}
	}
		
	disconnectFromDatabase(conn);
    return 0;
}
