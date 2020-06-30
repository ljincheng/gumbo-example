
#ifndef HTTPSELECTOR_H_
#define HTTPSELECTOR_H_

#include <string>

class HttpSelector
{
	public:
        HttpSelector();
		HttpSelector(char const * content,char const * title,char const * coverImg,char const *  author,char const * postDate,char const * summary,char const * source,char const * sourceUrl,char const *sourceCode);
    virtual ~HttpSelector();
    
	public:
		//内容
		std::string content();
		//标题
		std::string title();
		//封面图片
		std::string coverImg();
		//作者
		std::string author();
		//发布时间
		std::string postDate();
		//摘要
		std::string summary();
		//来源
		std::string source();
		//来源地址
		std::string sourceUrl();
        //源码
        std::string sourceCode();
    
        //默认作者
        std::string defaultAuthor();
        //文章类型ID
        int sortid();
    
        void setSourceUrl(char const * sourceUrl);
    void setDefaultAuthor(char const * defaultAuthor);
    void setSortid(int sortid);

	private:
		std::string mContent;
		std::string mTitle;
		std::string mCoverImg;
		std::string mAuthor;
		std::string mPostDate;
		std::string mSummary;
		std::string mSource;
		std::string mSourceUrl;
        std::string mSourceCode;
        std::string mDefaultAuthor;
        int mSortid;
    
};

#endif
