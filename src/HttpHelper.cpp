#include "HttpHelper.h"
#include <string>
#include <uriparser/Uri.h>
#include <stdio.h>
#include <stdlib.h>

HttpHelper::HttpHelper(void) :
m_bDebug(false)
{

}

HttpHelper::~HttpHelper(void)
{

}

static char * testAddOrRemoveBaseHelper(const char * ref, const char * base,
	 bool add = true, bool domainRootMode = false) {
	UriParserStateA stateA;

	// Base
	UriUriA baseUri;
	stateA.uri = &baseUri;
	int res = uriParseUriA(&stateA, base);
	if (res != 0) {
		return NULL;
	}

	// Rel
	UriUriA relUri;
	stateA.uri = &relUri;
	res = uriParseUriA(&stateA, ref);
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		return NULL;
	}

	// Expected result
	/*
	UriUriA expectedUri;
	stateA.uri = &expectedUri;
	res = uriParseUriA(&stateA, expected);
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		uriFreeUriMembersA(&relUri);
		uriFreeUriMembersA(&expectedUri);
		return false;
	}
	*/

	// Transform
	UriUriA transformedUri;
	if (add) {
		res = uriAddBaseUriA(&transformedUri, &relUri, &baseUri);
	} else {
		res = uriRemoveBaseUriA(&transformedUri, &relUri, &baseUri,
				domainRootMode ? URI_TRUE : URI_FALSE);
	}
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		uriFreeUriMembersA(&relUri);
		//uriFreeUriMembersA(&expectedUri);
		uriFreeUriMembersA(&transformedUri);
		return NULL;
	}

/*
	const bool equal = (URI_TRUE == uriEqualsUriA(&transformedUri, &expectedUri));
	if (!equal) {
		char transformedUriText[1024 * 8];
		char expectedUriText[1024 * 8];
		uriToStringA(transformedUriText, &transformedUri, 1024 * 8, NULL);
		uriToStringA(expectedUriText, &expectedUri, 1024 * 8, NULL);
		printf("\n\n\nExpected: \"%s\"\nReceived: \"%s\"\n\n\n", expectedUriText, transformedUriText);
	}
*/


int charsRequired;
if(uriToStringCharsRequiredA(&transformedUri,&charsRequired)!=URI_SUCCESS)
{
/** Failure **/
    uriFreeUriMembersA(&baseUri);
	uriFreeUriMembersA(&relUri);
	uriFreeUriMembersA(&transformedUri);
	return NULL;
}

/*
char transformedUriText[1024 * 8];
		uriToStringA(transformedUriText, &transformedUri, 1024 * 8, NULL);
		printf("\nReceived: \"%s\"\n\n\n", transformedUriText);
*/

charsRequired++;
char * expected = (char *)malloc( charsRequired * sizeof(char));

if(expected== NULL)
{
/** Failure **/
    uriFreeUriMembersA(&baseUri);
	uriFreeUriMembersA(&relUri);
	uriFreeUriMembersA(&transformedUri);
	return NULL;
}

if(uriToStringA(expected,&transformedUri,charsRequired,NULL)!= URI_SUCCESS)
	{

	 uriFreeUriMembersA(&baseUri);
	uriFreeUriMembersA(&relUri);
	uriFreeUriMembersA(&transformedUri);
	return NULL;
	}
    uriFreeUriMembersA(&baseUri);
	uriFreeUriMembersA(&relUri);
	uriFreeUriMembersA(&transformedUri);
	return expected;
}



char * HttpHelper::addOrRemoveBaseHelper(const char * ref, const char * base)
{
	char * res;
	res=testAddOrRemoveBaseHelper(ref,base);
	return res;
}


bool HttpHelper::startsWith(const char *pre, const char *str){
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void HttpHelper::SetDebug(bool bDebug)
{
	m_bDebug = bDebug;
}
