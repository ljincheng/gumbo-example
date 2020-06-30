
#ifndef __BEBE_HTTPDOCUEMNT_H__
#define __BEBE_HTTPDOCUEMNT_H__


#include "Object.h"
#include "Document.h"
#include "httpclient.h"
#include "HttpDatabase.h"
#include "HttpSelector.h"


class HttpDocument: public CObject
{

public:
	HttpDocument(CHttpClient httpClient,CDocument doc);
    HttpDocument(CHttpClient httpClient,CDocument doc,HttpDatabase* database);
	virtual ~HttpDocument(void);

public:


  /**
   * 查找链接a
   */
  int findLink(const char *href,const char *baseUrl,const char *sitename,int discoverId);
    
   int findLink(const char *href,const char *baseUrl,const char *sitename,int discoverId,const char * filters);

  int uploadNewInfo(const char * serverUrl);
    
    void extracted(const std::string &html, HttpSelector &htmlFilter, HttpSelector *&htmlSelector, const char *link);
    
    int uploadFlay(const char * serverUrl);
    
    /** 提现HTML内容 **/
   HttpSelector * extractHtml(const char *html,HttpSelector* selector,const char *link);
    
  int syncNewInfo();

private:
	CHttpClient httpClient;
  CDocument doc;
  HttpDatabase *database;


};

#endif
