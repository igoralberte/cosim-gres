#ifndef METRIC_ACCESS_H
#define METRIC_ACCESS_H

#include <string>
#include <sstream>
#include <vector>
using namespace std;

        //SPI_connect();                                                                                                                              \
        //SPI_execute("INSERT INTO similarql_indexes(name, idx_table, attrib_complex,distance_function) VALUES ('teste', 'teste', 'teste', 'teste')", false, 1);    \
        //if (SPI_connect() == SPI_OK_CONNECT) {                                                                                                                              \
        //    ereport(INFO, (errmsg("SPI conectado")));                                                                                                       \
        //    SPI_execute("INSERT INTO similarql_indexes(name, idx_table, attrib_complex,distance_function) VALUES ('teste', 'teste', 'teste', 'teste')", false, 1);            \
        //}                                                                                                                                                   \
        

#include <arboretum/stMetricAccessMethod.h>
#include <arboretum/stMetricTree.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stResult.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <distance_functions.h>
//#include "funcapi.h"


#ifdef __cplusplus
extern "C" {
#endif

    #include <postgres.h>
    #include <fmgr.h>
    #include <funcapi.h>
    #include "executor/spi.h"    
    #include "utils/builtins.h"


#ifdef __cplusplus
}
#endif

// Tipo do vetor de caracteristicas
// typedef float SignatureElementType;
//typedef float SignatureElementType;
// Vetor de caracteristicas
//typedef BasicArrayObject<SignatureElementType> Signature;

// Para o caso de trabalharmos com char
//typedef char SignatureElementTypeChar;
//typedef BasicArrayObject<SignatureElementTypeChar> SignatureChar;

// resultado da busca, isso eh necessario pelo fato da funcao que retorna um dataset ser um callback
typedef stResult < Signature > myResult;
typedef stResult < SignatureChar > myResultChar;

/*Global*/
vector<int32> answer_oids;
vector<float8> answer_dists;
vector<int32> answer_korder;

/*Functions Section*/
bytea * Signature2ByteArray(const Signature& imgSignature);
Signature ByteArrayToBasicArrayObject(bytea *byte_array);

bytea * CharSignature2ByteArray(const SignatureChar& imgSignature);
SignatureChar CharByteArrayToBasicArrayObject(bytea *byte_array);

text* concat4(text *arg1, text *arg2, text *arg3, text *arg4);
char *dup_pgtext(text *what);

// Singleton para gerenciar o pageManager
// isso evita que a aplicacao crie, diversas instancias e tenha problema de concorrencia
// referencia:  Efective C++, Scott Meyers
//              https://sourcemaking.com/design_patterns/singleton/cpp/1
class DiskManagerSingleton
{
    public:
        // Metodo de acesso a singleton
        static DiskManagerSingleton& getInstance()
        {
            static DiskManagerSingleton instance;
            return instance;
        }
    private:
        //Nome do arquivo e instancia da classe
    	stPlainDiskPageManager* pageManager = 0; // 0 = NULL
    	char fname[255];

        // Construtor privado
        DiskManagerSingleton() {
			memset(fname, '0', 255);
            pageManager = 0;
        }
        // Destrutor privado
        ~DiskManagerSingleton() {
			delete pageManager;
        }

    public:
        // Seta o nome do arquivo que o pagemanager esta utilizando
        // eh necessario rodar essa funcao sempre que for criá-lo para nao haver problema de acesso
        //pagesize é o tamanho de página em bytes
        void setPageManager(char* new_fname, uint32_t pagesize=8192){
        	if (fname == 0 || strcmp(fname, new_fname) != 0){
                if (pageManager != 0)
                    delete pageManager;
                strcpy(fname, new_fname);
                pageManager = new stPlainDiskPageManager(new_fname, pagesize);
            }
            else
            {
                strcpy(fname, new_fname);
                pageManager = new stPlainDiskPageManager(new_fname, pagesize);
            }

            // versão Willian
            /*if (fname == 0 || strcmp(fname, new_fname) != 0){
        		if (pageManager != 0)
                    delete pageManager;
        		strcpy(fname, new_fname);
        		pageManager = new stPlainDiskPageManager(new_fname, 8192);
        	}*/
        }
        // Apos criar podemos somente retornar o pageManager com o nome do arquivo
        stPlainDiskPageManager* getPageManager(char* new_fname){
        	if (strcmp(fname, new_fname) != 0) {
                delete pageManager;
                pageManager = new stPlainDiskPageManager(new_fname);
            }
            return pageManager;

            // versão Willian
            /*if (strcmp(fname, new_fname) != 0) {
                delete pageManager;
        		pageManager = new stPlainDiskPageManager(new_fname);
            }
        	return pageManager;*/
        }

        DiskManagerSingleton(DiskManagerSingleton const&) = delete;
        void operator=(DiskManagerSingleton const&) = delete;
};


#endif
