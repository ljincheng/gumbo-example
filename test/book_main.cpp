#include <iostream>
#include <string>
#include "Document.h"
#include "httpclient.h"
#include "HttpDocument.h"
#include <stdio.h>
#include <stdlib.h>
#include "HttpDatabase.h"
using namespace std;


int main(int argc, char * argv[])
{
    
    //if (argc != 3) {
    //    printf("Usage: get_title <html filename>.\n");
    //    exit(EXIT_FAILURE);
    //  }
    
    CHttpClient httpClient;
    CDocument doc;
    HttpDatabase* db;
    char const* home = getenv("HOME"),* dbFile="/.book/book.db";
    string  dbFilePath= std::string(home).append(std::string(dbFile));
    printf("============================home path=====================\n");
    printf("homePath=%s,dbFilePath=%s\n",home,dbFilePath.c_str());
     
    db=new HttpDatabase(dbFilePath.c_str());
    HttpDocument httpDocument(httpClient,doc,db);
    //httpDocument.findLink(argv[1],argv[2]);
    httpDocument.syncNewInfo();
    
    
    return 0;
}

