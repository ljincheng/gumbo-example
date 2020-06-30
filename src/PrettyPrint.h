

#ifndef PRETTYPRINT_H_
#define PRETTYPRINT_H_

#include <gumbo.h>
#include <string>

class PrettyPrint
{

	public:
		PrettyPrint(GumboNode* apNode = NULL);
		virtual ~PrettyPrint();
	
	public:
		std::string prettyPrintNode();
    
        std::string prettyPrintNode(const char * baseUrl);

	private:
	
		GumboNode* mpNode;

};

#endif
