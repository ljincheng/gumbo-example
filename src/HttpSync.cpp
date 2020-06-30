
#include "HttpSync.h"
#include "MD5.h"
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <json/json.h>

using namespace std;

HttpSync::HttpSync(CHttpClient * httpClient)
{
    mHttpClient=httpClient;
}

int HttpSync::postWithMd5(const std::string & postUrl, const std::string & md5Key,const std::string & data, std::string & strResponse)
{
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    unsigned long int ts = time(NULL);
    std::string signSP;
    std::string ts_value=std::to_string(ts);
    signSP.append(md5Key).append(ts_value).append(data);
    MD5 strMd5(signSP.c_str());
    std::string sign=strMd5.toString();
    cout << "ts=" << ts <<endl;
    cout << "sign=" << sign << endl;
    
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "sign",
                 CURLFORM_COPYCONTENTS, sign.c_str(), CURLFORM_END);
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "ts",
                 CURLFORM_COPYCONTENTS, ts_value.c_str(), CURLFORM_END);
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "data",
                 CURLFORM_COPYCONTENTS, data.c_str(), CURLFORM_END);
    
    int res= mHttpClient->HttpPost(postUrl, formpost, strResponse);
    return res;
}

int HttpSync::postJsonWithMd5(const std::string & postUrl, const std::string & md5Key,const std::string & data, std::string & strResponse)
{
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    Json::Value paramJson;
   // Json::Value jsonResValue;
    //Json::Reader jsonReader;
    Json::FastWriter writer;
    unsigned long int ts = time(NULL);
    std::string signSP;
    std::string ts_value=std::to_string(ts);
    signSP.append(md5Key).append(ts_value).append(data);
    MD5 strMd5(signSP.c_str());
    std::string sign=strMd5.toString();
    cout << "ts=" << ts <<endl;
    cout << "sign=" << sign << endl;
    
    paramJson["ts"]=Json::Value(ts_value);
    paramJson["sign"]=Json::Value(sign);
    //jsonReader.parse(data, jsonResValue);
    paramJson["data"]=Json::Value(data);;
    
     std::string jsondataStr = writer.write(paramJson);
    cout << "jsondata=" << jsondataStr << endl;
    
    
//    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "sign",
//                 CURLFORM_COPYCONTENTS, sign.c_str(), CURLFORM_END);
//    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "ts",
//                 CURLFORM_COPYCONTENTS, ts_value.c_str(), CURLFORM_END);
//    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "data",
//                 CURLFORM_COPYCONTENTS, data.c_str(), CURLFORM_END);
    
    int res= mHttpClient->postRawJson(postUrl, jsondataStr, strResponse);
    return res;
}
