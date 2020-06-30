
#ifndef __BEBE_HTTPHELPER_H__
#define __BEBE_HTTPHELPER_H__

#include <string>

class HttpHelper
{
public:
	HttpHelper(void);
	~HttpHelper(void);

public:
	/**
	* @brief HTTP POST请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	char * addOrRemoveBaseHelper(const char * ref, const char * base);

  /**
   * 判断str是否以pre开头
   */
  bool startsWith(const char *pre, const char *str);

  public:
	void SetDebug(bool bDebug);

private:
	bool m_bDebug;


};

#endif
