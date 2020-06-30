
#include "HttpSelector.h"

using namespace std;

HttpSelector::HttpSelector()
{
    
}
HttpSelector::HttpSelector(char const * content,char const * title,char const * coverImg,char const *  author,char const * postDate,char const * summary,char const * source,char const * sourceUrl,char const* sourceCode)
{
    if(content!=NULL)
        mContent=string(content);
    if(title!=NULL)
        mTitle=string(title);
    if(coverImg!=NULL)
        mCoverImg=string(coverImg);
    if(author!=NULL)
        mAuthor=string(author);
    if(postDate!=NULL)
        mPostDate=string(postDate);
    if(summary!=NULL)
        mSummary=string(summary);
    if(source!=NULL)
        mSource=string(source);
    if(sourceUrl!=NULL)
        mSourceUrl=string(sourceUrl);
    if(sourceCode!=NULL)
        mSourceCode=string(sourceCode);
}

HttpSelector::~HttpSelector()
{
//    delete mTitle;
//    delete mContent;
//    delete mCoverImg;
//    delete mAuthor;
//    delete mSourceUrl;
//    delete mSourceCode;
//    delete mSource;
//    delete mPostDate;
//    delete mSummary;
    if(!mTitle.empty())
    {
        mTitle.clear();
    }
    if(!mSourceCode.empty())
    {
        mSourceCode.clear();
    }
    if(!mContent.empty())
    {
        mContent.clear();
    }
    if(!mSummary.empty())
    {
        mSummary.clear();
    }
    if(!mAuthor.empty())
    {
        mAuthor.clear();
    }
    if(!mPostDate.empty())
    {
        mPostDate.clear();
    }
    if(!mSource.empty())
    {
        mSource.clear();
    }
    if(!mCoverImg.empty())
    {
        mCoverImg.clear();
    }
    if(!mDefaultAuthor.empty())
    {
        mDefaultAuthor.clear();
    } 

}

string HttpSelector::content()
{
	return mContent;
}

string HttpSelector::title()
{
    return  mTitle;
}

string HttpSelector::coverImg()
{
    return mCoverImg;
}

string HttpSelector::author()
{
    return mAuthor;
}

string HttpSelector::postDate()
{
    return mPostDate;
}

string HttpSelector::summary()
{
    return mSummary;
}

string HttpSelector::source()
{
    return mSource;
}

string HttpSelector::sourceUrl()
{
    return mSourceUrl;
}


string HttpSelector::sourceCode()
{
    return mSourceCode;
}

void HttpSelector::setSourceUrl(char const * sourceUrl)
{
    if(sourceUrl==NULL)
    {
        mSourceUrl.clear();
    }else{
        mSourceUrl=string(sourceUrl);
    }
}

//默认作者
std::string HttpSelector::defaultAuthor(){
    return mDefaultAuthor;
    
}
//文章类型ID
int HttpSelector::sortid(){
    return mSortid;
}

void HttpSelector::setDefaultAuthor(char const * defaultAuthor){
    if(defaultAuthor==NULL)
    {
        mDefaultAuthor.clear();
    }else{
        mDefaultAuthor=string(defaultAuthor);
    }
}

void HttpSelector::setSortid(int sortid){
    mSortid=sortid;
}
