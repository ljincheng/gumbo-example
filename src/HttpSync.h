#ifndef HTTPSYNC_H
#define HTTPSYNC_H

#include "httpclient.h"

class HttpSync {

	public:
		HttpSync(CHttpClient * httpClient);

	public:
		
		int postWithMd5(const std::string & postUrl, const std::string & md5Key,const std::string & data, std::string & strResponse);
    
        int postJsonWithMd5(const std::string & postUrl, const std::string & md5Key,const std::string & data, std::string & strResponse);

	private:

		CHttpClient * mHttpClient;

};

#endif

