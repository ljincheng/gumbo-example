#include "HttpDatabase.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>


#include <iostream>


using namespace std;

//static char * DATABASEFILE="book.db";

HttpDatabase::HttpDatabase() {
//printf("============================ok db====================\n");
//db= sqlite3pp::database("/workspace/code/gcc/bebe-gumbo/bebe-gumbo.db");
//sqlite3pp::database* mydb=new sqlite3pp::database("/workspace/code/gcc/bebe-gumbo/bebe-gumbo.db");
  // NSString *_databaseFile=[NSString stringWithFormat:@"%@/.book/%s",NSHomeDirectory(),DATABASEFILE];
   char const* home = getenv("HOME"),* dbFile="/.book/book.db";
   string  dbFilePath= std::string(home).append(std::string(dbFile));
   printf("============================home path=====================\n");
   printf("homePath=%s,dbFilePath=%s\n",home,dbFilePath.c_str());

//sqlite3pp::database* mydb=new sqlite3pp::database("/Users/ljc/.book/book.db");
sqlite3pp::database* mydb=new sqlite3pp::database(dbFilePath.c_str());
//sqlite3pp::database* mydb=new sqlite3pp::database("/usr/local/workspace/code/bebe/gcc/bebe-gumbo/bebe-gumbo.db");
db=mydb;
//db("/workspace/code/gcc/bebe-gumbo/bebe-gumbo.db");
}
HttpDatabase::HttpDatabase(const char *dbfile){
    sqlite3pp::database* mydb=new sqlite3pp::database(dbfile);
    //sqlite3pp::database* mydb=new sqlite3pp::database("/usr/local/workspace/code/bebe/gcc/bebe-gumbo/bebe-gumbo.db");
    db=mydb;
}

HttpDatabase::~HttpDatabase(void)
{
delete db;
}


int HttpDatabase::addLink(const char *link,const char *title,const char *sitename,int discoverId)
{
//    if(strlen(title)<2)
//    {
//        return 0;
//    }
//    char * strc = new char[strlen(fullLink)+1];
//    strcpy(strc,fullLink);
//    const char s[2] = "#";
//     char *link;
//    link=strtok(strc, s);
int num=0;
sqlite3pp::command *cmd;
sqlite3pp::query qry(*db, "SELECT id,LINK FROM site_info_discover where LINK=?");
qry.bind(1,link, sqlite3pp::nocopy);
int dcount=0;
for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
dcount++;
break;
}
if(dcount==0)
{

 cmd=new sqlite3pp::command(*db, "INSERT INTO site_info_discover(LINK, TITLE,SITENAME,CREATEDATE,discover_id) VALUES (?,?,?,date('now'),?)");
//cmd.binder() << link << title << sitename;
cmd->bind(1,link , sqlite3pp::nocopy);
cmd->bind(2,title , sqlite3pp::nocopy);
cmd->bind(3,sitename , sqlite3pp::nocopy);
cmd->bind(4,discoverId);
//printf("============================ok command3====================\n");
cmd->execute();
delete cmd;
//printf("新增记录：%s,%s\n",title,link);
num++;
}
//    delete[] strc;
return num;
}

int HttpDatabase::updateInfoStatus(int infoId,int status){
    int res=0;
    sqlite3pp::command *cmd;
    cmd=new sqlite3pp::command(*db, "update site_info_discover set STATUS=? where id=?");
    //cmd.binder() << link << title << sitename;
    cmd->bind(1,status);
    cmd->bind(2,infoId);
    cmd->execute();
    delete cmd;
    return res;
}

sqlite3pp::query* HttpDatabase::findNewInfo(){

sqlite3pp::query* qry;
//sqlite3pp::query qry2(*db, "SELECT id,LINK,BASEURL,SITENAME  FROM site_info_url where STATUS=1");
//qry=&qry2;
 qry=new sqlite3pp::query(*db, "SELECT id,LINK,BASEURL,SITENAME,FILTERS,match_title,match_content,match_coverImg,match_author,match_postDate,match_summary,match_source,author,sortid  FROM site_info_url where STATUS=1");
/*
for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
cout << "====test ok" << endl;
  int id;
  char const* link, *baseUrl;
  std::tie(id, link, baseUrl) =
    (*i).get_columns<int, char const*, char const*>(0, 1, 2);
  cout << id << "\t" << link << "\t" << baseUrl << endl;
}
*/
return qry;
//return NULL;
}

sqlite3pp::query* HttpDatabase::queryNewInfo(int status,int num=100,int start=0){
    sqlite3pp::query* qry;
    //sqlite3pp::query qry2(*db, "SELECT id,LINK,BASEURL,SITENAME  FROM site_info_url where STATUS=1");
    //qry=&qry2;
    qry=new sqlite3pp::query(*db, "SELECT id,LINK,TITLE,SITENAME,CREATEDATE,STATUS,discover_id FROM site_info_discover where STATUS=? limit ?,?");
    qry->bind(1,status);
    qry->bind(2,start);
    qry->bind(3,num);
    /*
     for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
     cout << "====test ok" << endl;
     int id;
     char const* link, *baseUrl;
     std::tie(id, link, baseUrl) =
     (*i).get_columns<int, char const*, char const*>(0, 1, 2);
     cout << id << "\t" << link << "\t" << baseUrl << endl;
     }
     */
    return qry;
    //return NULL;
    
}

///////////////////////////////////////////////////////////////////////////////////////////////

 
