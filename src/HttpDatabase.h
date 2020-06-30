

#ifndef __BEBE_HTTPDATABASE_H__
#define __BEBE_HTTPDATABASE_H__


#include "Object.h"
#include "sqlite3pp.h"


class HttpDatabase: public CObject
{

public:
	HttpDatabase();
    HttpDatabase(const char *dbfile);
	virtual ~HttpDatabase(void);

public:


  int addLink(const char *link,const char *title,const char *sitename,int discoverId);
    
    /**
    *修改信息记录状态
     * infoId: 记录ID
     * status: 记录状态
     */
   int updateInfoStatus(int infoId,int status);

    /**
     *查找要爬取的信息链接地址记录
     *
     */
  sqlite3pp::query* findNewInfo();
    
    /**
     * 根据记录状态，返回记录数，开始位置工查询言已爬取的信息
     */
  sqlite3pp::query* queryNewInfo(int status,int num,int start);

private:
	 sqlite3pp::database* db;


};

#endif
