#include "HttpDocument.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Node.h"
#include "httpclient.h"
#include "HttpHelper.h"
#include <json/json.h>
#include <map>
#include <iostream>
#include "HttpSync.h"
using namespace std;

HttpDocument::HttpDocument(CHttpClient httpclient,CDocument adoc) {
httpClient=httpclient;
doc=adoc;
HttpDatabase* db=new HttpDatabase;
database=db;
}

HttpDocument::HttpDocument(CHttpClient httpclient,CDocument adoc,HttpDatabase* db){
    httpClient=httpclient;
    doc=adoc;
   // HttpDatabase* db=new HttpDatabase;
    database=db;
}

HttpDocument::~HttpDocument(void)
{
delete database;
}

string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}

int HttpDocument::findLink(const char *startUrl,const char *baseUrl,const char *sitename,int discoverId,const char * filters){
    int num=0;
    
    //CDocument doc;
    HttpHelper httpDocument;
    std::string html;
    
    int code=httpClient.Get(startUrl,html);
    
    if(code==0)
    {
        doc.parse(html.c_str());
        CSelection c = doc.find(filters);
        size_t num=c.nodeNum();
        //HttpDatabase database;
        for(size_t i=0;i<num;i++)
        {
            CNode cnode=c.nodeAt(i);
            std::string hrefstr=cnode.attribute("href");
            std::string title;
            if(cnode.find("h4").nodeNum()>0)
            {
                title=cnode.find("h4").nodeAt(0).text();
            }else if(cnode.find("h3").nodeNum()>0)
            {
                title=cnode.find("h3").nodeAt(0).text();
            }else if(cnode.find("h2").nodeNum()>0)
            {
                title=cnode.find("h2").nodeAt(0).text();
                
            }else if(cnode.find("h1").nodeNum()>0)
            {
                title=cnode.find("h1").nodeAt(0).text();
            }else{
                title=cnode.text();
            }
            //         cout << "title=" << title <<endl;
            // std::string aText=c.nodeAt(i).text();
            const char *href=hrefstr.data();
            if(!httpDocument.startsWith("#",href) && !title.empty())
            {
                
                char * newHref;
                newHref=httpDocument.addOrRemoveBaseHelper(href,startUrl);
                if(newHref != NULL && httpDocument.startsWith(baseUrl, newHref))
                {
                    //printf(newHref);
                    string   time = getTime();
                    cout <<  time << " title=" << title << ",href=" << newHref <<endl;
                    
                    
                    const char s[2] = "#";
                    char *link;
                    link=strtok(newHref, s);
                    database->addLink(link,title.c_str(),sitename,discoverId);
                    //delete[] link;
                    // printf("newhref=%s,baseUrl=%s,findUrl=%s\n",newHref,baseUrl,href);
                    free(newHref);
                }
                
            }else{
                //printf("无效href=%s\n",href);
            }
        }
        
    }else{
        cout <<"请求"<< baseUrl <<"返回code=" << code <<endl;
    }
    return num;
}


int HttpDocument::findLink(const char *startUrl,const char *baseUrl,const char *sitename,int discoverId){

    return findLink(startUrl, baseUrl, sitename,discoverId,"a");
}

HttpSelector * HttpDocument::extractHtml(const char *html,HttpSelector* selector,const char *link)
{
    if(html==NULL || selector ==NULL)
    {
        return NULL;
    }
    string mContent;
    string mTitle;
    string mCoverImg;
    string mSource;
    string mSourceUrl;
    string mSouceCode=string(html);
    string mAuthor;
    string mPostDate;
    string mSummary;
    CDocument doc;
    CDocument doc_content;
    
    
    doc.parse(html);
    
    //1.封面图片
    string coverImg=selector->coverImg();
    if(!coverImg.empty())
    {
        CSelection imgNodes =doc.find(coverImg.c_str());
        if(imgNodes.nodeNum()>0)
        {
            //            for(size_t imgIndex=0,imgSize=imgNodes.nodeNum();imgIndex<imgSize;imgIndex++)
            //            {
            // CNode imgNode=imgNodes.nodeAt(imgIndex);
            CNode imgNode=imgNodes.nodeAt(0);
            string src=imgNode.attribute("src");
            if(src.empty())
            {
                src=imgNode.attribute("_src");
            }
            if(!src.empty())
            {
                HttpHelper httpDocument;
                char *fullImgPath=httpDocument.addOrRemoveBaseHelper(src.c_str(),link);
                mCoverImg.append(fullImgPath);
            }
            //            }
        }
    }
    
    //2、内容
    string contentFilter=selector->content();
    if(!contentFilter.empty())
    {
        CSelection authorNodes =doc.find(contentFilter.c_str());
        if(authorNodes.nodeNum()>0)
        {
            for(size_t imgIndex=0,imgSize=authorNodes.nodeNum();imgIndex<imgSize;imgIndex++)
            {
                CNode contentNode=authorNodes.nodeAt(imgIndex);
                string contentStr= contentNode.cleanText(link);
                mContent.append(contentStr);
               // mContent.append(mSouceCode.substr(contentNode.startPos(),contentNode.endPos()-contentNode.startPos()));
                //如果找不到封面，就从内容中找一张图片
                if(mCoverImg.empty())
                {
                    CSelection imgNodes =contentNode.find("img");
                    if(imgNodes.nodeNum()>0)
                    { 
                        CNode imgNode=imgNodes.nodeAt(0);
                        string src=imgNode.attribute("src");
                        if(src.empty())
                        {
                            src=imgNode.attribute("_src");
                        }
                        if(!src.empty())
                        {
                            HttpHelper httpDocument;
                            char *fullImgPath=httpDocument.addOrRemoveBaseHelper(src.c_str(),link);
                            mCoverImg.append(fullImgPath);
                        }
                    }
                }
            }
        }
    }
   
    //3、获取作者
    string author=selector->author();
    if(!author.empty())
    {
        CSelection selectorNodes =doc.find(author.c_str());
        if(selectorNodes.nodeNum()>0)
        {
            for(size_t i=0,imgSize=selectorNodes.nodeNum();i<imgSize;i++)
            {
                CNode selectorNode=selectorNodes.nodeAt(i);
                if(!mAuthor.empty())
                {
                    mAuthor.append(";");
                }
                mAuthor.append(selectorNode.text());
            }
        }
    }
    //3、发布时间
    string postDate=selector->postDate();
    if(!postDate.empty())
    {
        CSelection selectorNodes =doc.find(postDate.c_str());
        if(selectorNodes.nodeNum()>0)
        {
            for(size_t i=0,imgSize=selectorNodes.nodeNum();i<imgSize;i++)
            {
                CNode selectorNode=selectorNodes.nodeAt(i);
                if(!mPostDate.empty())
                {
                    mPostDate.append(";");
                }
                mPostDate.append(selectorNode.text());
            }
        }
    }
    //3、发布时间
    string title=selector->title();
    if(!title.empty())
    {
        CSelection selectorNodes =doc.find(title.c_str());
        if(selectorNodes.nodeNum()>0)
        {
            for(size_t i=0,imgSize=selectorNodes.nodeNum();i<imgSize;i++)
            {
                CNode selectorNode=selectorNodes.nodeAt(i);
                mTitle=selectorNode.text();
            }
        }
    }
    
    HttpSelector* resSelector=new HttpSelector(mContent.empty()?NULL:mContent.c_str(),mTitle.empty()?NULL:mTitle.c_str(),mCoverImg.empty()?NULL:mCoverImg.c_str(),mAuthor.empty()?NULL:mAuthor.c_str(),mPostDate.empty()?NULL:mPostDate.c_str(),mSummary.empty()?NULL:mSummary.c_str(),mSource.empty()?NULL:mSource.c_str(),mSourceUrl.empty()?NULL:mSourceUrl.c_str(),mSouceCode.c_str());
    return resSelector;
}

 int  HttpDocument::uploadFlay(const char * serverUrl)
{
    int num=0;
    Json::Value root;
    Json::FastWriter writer;
    Json::Reader jsonReader;
    Json::Value jsonResValue;
    sqlite3pp::query* qry;
    std::vector<int> infoIdList;
    map<int, HttpSelector> htmlSelectorMap;
    map<int,HttpSelector>::iterator htmlSelectorItem;;
    HttpSelector htmlFilter;
    HttpSync httpSync(&httpClient);
    std::string postServerUrl=std::string(serverUrl);
    HttpSelector* htmlSelector;
    string md5Key=string("UQo9TvfqN6SD3a");
    
    //设置过滤条件
    qry=database->findNewInfo();
    for (sqlite3pp::query::iterator i = qry->begin(); i != qry->end(); ++i) {
        //cout << "====test ok2" << endl;
        int id;
        int sortid;
        char const* link, *baseUrl,*siteName,*filters,*matchTitle,*matchContent,*matchCoverImg,*matchAuthor,*matchPostDate,*matchSummary,*matchSource,*defaultAuthor;
        std::tie(id, link, baseUrl,siteName,filters,matchTitle,matchContent,matchCoverImg,matchAuthor,matchPostDate,matchSummary,matchSource,defaultAuthor,sortid) =
        (*i).get_columns<int, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*,char const*,int>(0, 1, 2,3,4,5,6,7,8,9,10,11,12,13);
        cout << id << "\t" << link << "\t" << baseUrl << endl;
        //        if(filters==NULL)
        //        {
        HttpSelector huxiuSelector=HttpSelector(matchContent, matchTitle,matchCoverImg, matchAuthor, matchPostDate, matchSummary, matchSource, NULL,NULL);
        huxiuSelector.setSortid(sortid);
        huxiuSelector.setDefaultAuthor(defaultAuthor);
        htmlSelectorMap.insert(pair<int,HttpSelector>(id,huxiuSelector));
        //        }
        
    }
    //delete qry;
    
    qry=database->queryNewInfo(0,2000,0);
    for (sqlite3pp::query::iterator i = qry->begin(); i != qry->end(); ++i) {
        num++;
        int infoId;
        int discoverId;
        char const* link, *title,*sitename,*createdate;
        //LINK,TITLE,SITENAME,CREATEDATE
        std::tie(infoId, link, title,sitename,createdate,discoverId) =
        (*i).get_columns<int, char const*, char const*, char const*, char const*,int>(0, 1, 2,3,4,6);
        //cout << id << "\t" << link << "\t" << title << "\t" << sitename << "\t" << createdate << endl;
        if(strlen(title)>0 && strlen(link) >0)
        {
            Json::Value item;
            //item["id"]=Json::Value(infoId);
            //item["link"]=Json::Value(link);
            item["title"]=Json::Value(title);
           // item["sitename"]=Json::Value(sitename);
           // item["infodate"]=Json::Value(createdate);
            
            //======= 解析网址HTML内容==========
            htmlSelectorItem=htmlSelectorMap.find(discoverId);
            if(htmlSelectorItem!=htmlSelectorMap.end())
            {
                htmlFilter=htmlSelectorItem -> second;
                //            }else{
                // htmlFilter=HttpSelector("body", NULL,NULL, NULL, NULL, NULL, NULL, NULL,NULL);
                //            }
                
                std::string html;
                int code=httpClient.Get(link,html);
                //            //std::cout << html << std::endl;
                if(code==0)
                {
                    htmlSelector=extractHtml(html.c_str(),&htmlFilter ,link);
                    if(htmlSelector!=NULL  )
                    {
                        string content=htmlSelector->content();
                        if(!content.empty())
                        {
                            item["content"]=Json::Value(htmlSelector->content());
                            if(htmlFilter.defaultAuthor().empty())
                            {
                                item["author"]=Json::Value(htmlSelector->author());
                            }else{
                                item["author"]=Json::Value(htmlFilter.defaultAuthor());
                            }
                            int sortid=htmlFilter.sortid();
                            if(sortid>0)
                            {
                                item["newsTypeID"]=Json::Value(sortid);
                            }
                            item["cover"]=Json::Value(htmlSelector->coverImg());
                            //item["postDate"]=Json::Value(htmlSelector->postDate());
                           // item["source"]=Json::Value(htmlSelector->source());
                            //item["title"]=Json::Value(htmlSelector->title());
                            item["description"]=Json::Value(htmlSelector->summary());
                            std::string postJSONData = writer.write(item);
                            cout<< "postJSONData=" << postJSONData << endl;
                            
                            //上传同步数据
                            
                            std::string responseStr;
                            httpSync.postJsonWithMd5(postServerUrl,md5Key,postJSONData,responseStr);
                            if(!responseStr.empty())
                            {
                                cout << "上传同步link="<< link << "，返回结果：" << responseStr << endl;
                                if (jsonReader.parse(responseStr, jsonResValue)){
                                    string resCode=jsonResValue["code"].asString();
                                    if("0" == resCode)
                                    {
                                        cout << "上传数据成功:" << responseStr << endl;
                                            database->updateInfoStatus(infoId,1);
                                    }else{
                                         database->updateInfoStatus(infoId,2);//上传失败
                                    }
                                }else{
                                    cout << "请求解析失败:"<< responseStr << endl;
                                }
                            }else{ 
                                cout << "上传数据失败或超时" << endl;
                            }
                             
                        }else{
                             database->updateInfoStatus(infoId,3);//解析失败
                            cout << "解析网址内容返回空值 url="<< link << endl;
                        }
                    }
                    delete htmlSelector;
                    htmlSelector=NULL;
                }
                html.clear();
            }
            
            //            //======== 解析网址HTML内容==========
            
           // root.append(item);
        }
        //infoIdList.push_back(infoId);
    }
    
//    if(num>0)
//    {
//         uploadFlay(serverUrl);
//    }
//    
    
    return num;
}


int HttpDocument::uploadNewInfo(const char * serverUrl){

    int num=0;
    Json::Value root;
    Json::FastWriter writer;
    Json::Reader jsonReader;
    Json::Value jsonResValue;
    sqlite3pp::query* qry;
    std::vector<int> infoIdList;
    map<int, HttpSelector> htmlSelectorMap;
    map<int,HttpSelector>::iterator htmlSelectorItem;;
    HttpSelector htmlFilter;
    
    HttpSelector* htmlSelector;
    
    //设置过滤条件
     qry=database->findNewInfo();
    for (sqlite3pp::query::iterator i = qry->begin(); i != qry->end(); ++i) {
        //cout << "====test ok2" << endl;
        int id;
        char const* link, *baseUrl,*siteName,*filters,*matchTitle,*matchContent,*matchCoverImg,*matchAuthor,*matchPostDate,*matchSummary,*matchSource;
        std::tie(id, link, baseUrl,siteName,filters,matchTitle,matchContent,matchCoverImg,matchAuthor,matchPostDate,matchSummary,matchSource) =
        (*i).get_columns<int, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*, char const*>(0, 1, 2,3,4,5,6,7,8,9,10,11);
        cout << id << "\t" << link << "\t" << baseUrl << endl;
//        if(filters==NULL)
//        {
            HttpSelector huxiuSelector=HttpSelector(matchContent, matchTitle,matchCoverImg, matchAuthor, matchPostDate, matchSummary, matchSource, NULL,NULL);
            htmlSelectorMap.insert(pair<int,HttpSelector>(id,huxiuSelector));
//        }
        
    }
    //delete qry;
    
     qry=database->queryNewInfo(0,20,0);
    for (sqlite3pp::query::iterator i = qry->begin(); i != qry->end(); ++i) {
      num++;
      int infoId;
        int discoverId;
      char const* link, *title,*sitename,*createdate;
        //LINK,TITLE,SITENAME,CREATEDATE
      std::tie(infoId, link, title,sitename,createdate,discoverId) =
        (*i).get_columns<int, char const*, char const*, char const*, char const*,int>(0, 1, 2,3,4,6);
      //cout << id << "\t" << link << "\t" << title << "\t" << sitename << "\t" << createdate << endl;
        if(strlen(title)>0 && strlen(link) >0)
        {
            Json::Value item;
            item["id"]=Json::Value(infoId);
            item["link"]=Json::Value(link);
            item["title"]=Json::Value(title);
            item["sitename"]=Json::Value(sitename);
            item["infodate"]=Json::Value(createdate);
            
            //======= 解析网址HTML内容==========
            htmlSelectorItem=htmlSelectorMap.find(discoverId);
            if(htmlSelectorItem!=htmlSelectorMap.end())
            {
                htmlFilter=htmlSelectorItem -> second;
//            }else{
               // htmlFilter=HttpSelector("body", NULL,NULL, NULL, NULL, NULL, NULL, NULL,NULL);
//            }
             
                std::string html;
                int code=httpClient.Get(link,html);
    //            //std::cout << html << std::endl;
                if(code==0)
                {
                    htmlSelector=extractHtml(html.c_str(),&htmlFilter,link );
                    if(htmlSelector!=NULL)
                    {
                        item["content"]=Json::Value(htmlSelector->content());
                        item["author"]=Json::Value(htmlSelector->author());
                        item["coverImg"]=Json::Value(htmlSelector->coverImg());
                        item["postDate"]=Json::Value(htmlSelector->postDate());
                        item["source"]=Json::Value(htmlSelector->source());
                        item["subtitle"]=Json::Value(htmlSelector->title());
                        item["summary"]=Json::Value(htmlSelector->summary());
                        cout<< "title=" << htmlSelector->title()  << ";author=" << htmlSelector->author() <<  ";coverImg=" << htmlSelector->coverImg() <<   ";postDate=" << htmlSelector->postDate() <<   ";source=" << htmlSelector->source() << endl;
                    }
                    delete htmlSelector;
                    htmlSelector=NULL;
                }
                html.clear();
            }
            
//            //======== 解析网址HTML内容==========
            
            root.append(item);
        }
         infoIdList.push_back(infoId);
    }
    if(num>0)
    {
        // JSON转换为JSON字符串（已格式化）
        //std::string jsondataStr = root.toStyledString();
        //  JSON转换为JSON字符串（未格式化）
       
        
        std::string jsondataStr = writer.write(root);

        cout << jsondataStr << endl << endl;
        delete qry;
        std::string strUrl=std::string(serverUrl);// "http://www.stay86.com/jcpt/digger/syncInfo.do";
       // std::string strPost="menu=info&data=";
        std::string responseStr;
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "menu",
                     CURLFORM_COPYCONTENTS, "info", CURLFORM_END);
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "data",
                     CURLFORM_COPYCONTENTS, jsondataStr.c_str(), CURLFORM_END);
        
        httpClient.HttpPost(strUrl, formpost, responseStr);
        if(!responseStr.empty())
        {
            if (jsonReader.parse(responseStr, jsonResValue)){
                int resCode=jsonResValue["code"].asInt();
                if(1 == resCode)
                {
                    cout << "上传数据成功:" << responseStr << endl;
                    for(int i=0;i<=infoIdList.size()-1;i++)
                    {
                        cout << "开始更新记录状态：id= " << infoIdList[i] << endl;
                        database->updateInfoStatus(infoIdList[i],1);
                    }
                }
            }else{
                cout << "请求解析失败:"<< responseStr << endl;
            }
        }else{
            cout << "上传数据失败或超时" << endl;
        }
        infoIdList.clear();
         uploadNewInfo(serverUrl);
    }
    
    return num;
}


int HttpDocument::syncNewInfo(){
    int num=0;
    //HttpDatabase database;
    sqlite3pp::query* qry;
    
    qry=database->findNewInfo();
  
    //qry=database.findNewInfo();
    //qry->begin();
    //printf("======================ok================\n");
    //qry.begin();
    //cout << "====test ok" << endl;
    for (sqlite3pp::query::iterator i = qry->begin(); i != qry->end(); ++i) {
        //cout << "====test ok2" << endl;
        int id;
        char const* link, *baseUrl,*siteName,*filters;
        std::tie(id, link, baseUrl,siteName,filters) =
        (*i).get_columns<int, char const*, char const*, char const*, char const*>(0, 1, 2,3,4);
        cout << id << "\t" << link << "\t" << baseUrl << endl;
        if(filters==NULL)
        {
            findLink(link,baseUrl,siteName,id);
        }else{
            findLink(link,baseUrl,siteName,id,filters);
        }
        
    }
    delete qry;
    
    return num;
    
}
///////////////////////////////////////////////////////////////////////////////////////////////

 
