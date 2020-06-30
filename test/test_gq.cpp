#include <iostream>
#include <string>
#include "Document.h"
#include "Node.h"
#include "httpclient.h"
#include <map>
#include "HttpSelector.h"
#include "MD5.h"
#include <sys/time.h>
#include "HttpSync.h"
#include "HttpHelper.h"

using namespace std;

HttpSelector htmlFilterNodeName(string url)
{
    map<string, HttpSelector> htmlSelectorMap;//HTML选择集
    map<string,HttpSelector>::iterator htmlSelectorItem;;
    HttpSelector htmlFilter;
    //==== MD5 签名==========
    MD5 strMd5("123456789");
    printf("%s\r\n", strMd5.toString().c_str());
    //======系统时间==============
     unsigned long int sec1 = time(NULL);
    std::cout << sec1 << std::endl;
    
    HttpSelector huxiuSelector=HttpSelector("div#article-content", "h1.article__title","div.top-img>img", "span.text-remarks:contains('作者：')", "span.article__time", NULL, NULL, NULL,NULL);
    
    htmlSelectorMap.insert(pair<string,HttpSelector>("www.huxiu.com",huxiuSelector));
    htmlSelectorItem=htmlSelectorMap.find(url);
    if(htmlSelectorItem!=htmlSelectorMap.end())
    {
        htmlFilter=htmlSelectorItem -> second;
    }else{
        htmlFilter=HttpSelector("body", NULL,NULL, NULL, NULL, NULL, NULL, NULL,NULL);
    }
    return htmlFilter;
}

HttpSelector * extractNodeText(string html,HttpSelector* selector,const char * baseUrl)
{
    if(html.empty() || selector ==NULL)
    {
        return NULL;
    }
    string mContent;
    string mTitle;
    string mCoverImg;
    string mSource;
    string mSourceUrl;
    string mSouceCode;
    string mAuthor;
    string mPostDate;
    string mSummary;
    CDocument doc;
    
     doc.parse(html.c_str());
    
    //1.封面图片
    string coverImg=selector->coverImg();
    if(!coverImg.empty())
    {
        CSelection imgNodes =doc.find(coverImg.c_str());
        if(imgNodes.nodeNum()>0)
        {
            for(size_t imgIndex=0,imgSize=imgNodes.nodeNum();imgIndex<imgSize;imgIndex++)
            {
                CNode imgNode=imgNodes.nodeAt(imgIndex);
                string src=imgNode.attribute("src");
                if(src.empty())
                {
                    src=imgNode.attribute("_src");
                }
                if(!src.empty())
                {
                    mCoverImg.append(src);
                }
            }
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
                cout << "美化HTML ：" << contentNode.cleanText(baseUrl) << endl;
                mContent.append(contentNode.cleanText(baseUrl) );
                
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
                            char *fullImgPath=httpDocument.addOrRemoveBaseHelper(src.c_str(),baseUrl);
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
    
    HttpSelector* resSelector=new HttpSelector(mContent.empty()?NULL:mContent.c_str(),mTitle.empty()?NULL:mTitle.c_str(),mCoverImg.empty()?NULL:mCoverImg.c_str(),mAuthor.empty()?NULL:mAuthor.c_str(),mPostDate.empty()?NULL:mPostDate.c_str(),mSummary.empty()?NULL:mSummary.c_str(),mSource.empty()?NULL:mSource.c_str(),mSourceUrl.empty()?NULL:mSourceUrl.c_str(),html.c_str());
    return resSelector;
}

int main(int argc, char * argv[])
{

//    CDocument doc;
    std::string html;
//    std::string content;
    CHttpClient httpClient;
    HttpSelector htmlFilter;
    HttpSelector* htmlSelector;
   
    char const* testUrl="http://www.chinadaily.com.cn/a/202002/03/WS5e3771e1a310128217274451.html";

    int code=httpClient.Get(testUrl,html);
    //std::cout << html << std::endl;
    if(code==0)
    {
//        doc.parse(html.c_str());

        htmlFilter=htmlFilterNodeName("www.chinadaily.com.cn");

        cout<< "htmlFilter=" << htmlFilter.content() << endl;

        htmlSelector=extractNodeText(html, &htmlFilter,testUrl);
        if(htmlSelector!=NULL)
        {
            cout<< "title=" << htmlSelector->title() << endl;
            cout<< "author=" << htmlSelector->author() << endl;
            cout<< "coverImg=" << htmlSelector->coverImg() << endl;
            cout<< "postDate=" << htmlSelector->postDate() << endl;
            cout<< "source=" << htmlSelector->source() << endl;
            cout<< "summary=" << htmlSelector->summary() << endl;
            cout<< "sourceUrl=" << htmlSelector->sourceUrl() << endl;
            cout<< "content="   << htmlSelector->content() << endl;
           cout<< "sourceCode=" << htmlSelector->sourceCode() << endl;

        }
    }
    
   
  return 0;
}

