#include <iostream>
#include <cstring>
#include <exception>
#include <curl/curl.h>
//#include <json/json.h>

using namespace std;
size_t callback(char *ptr, size_t size, size_t nmemb, string &stream){

  size_t sizes = size*nmemb;
  string temp(ptr,sizes);
  stream += temp;
  return sizes;
}
int main( int argv, char * argc[] ){

  CURL *curl;
  curl_slist *list;
  CURLM *multi_handle;
  CURLcode res;
  long code;
  string stream;
  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;

  try{

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if( curl ){

      curl_formadd(&formpost,&lastptr,CURLFORM_COPYNAME,"idnumber",
                  CURLFORM_COPYCONTENTS, "360481199999999",CURLFORM_END);
      curl_formadd(&formpost,&lastptr,CURLFORM_COPYNAME,"name",
                  CURLFORM_COPYCONTENTS, "王明",CURLFORM_END);
      list = curl_slist_append(NULL,"Authorization:AUTHORIZATION");//请将AUTHORIZATION替换为根据API_KEY和API_SECRET得到的签名认证串
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);

      #ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      #endif
      #ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      #endif

      res = curl_easy_perform(curl);

      if( res != CURLE_OK ){
        cout<<"curl_easy_perform() failed:"<<curl_easy_strerror(res)<<endl;
        return -1;
      }
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
      /*
      Json::Value res_data;
      Json::Reader *reader = new Json::Reader(Json::Features::strictMode());
      if(!reader->parse(stream, res_data)){
        cout<<"parse error";
        return -1;
      }
 	**/
	cout<< stream <<endl;
      cout<<"HTTP Status Code:"<<code<<endl;
    //  cout<<res_data<<endl;
      curl_easy_cleanup(curl);
  }
    curl_global_cleanup();

  }catch(exception &ex){
    cout<<"curl exception:"<<ex.what()<<endl;
  }
    return 0;
}
