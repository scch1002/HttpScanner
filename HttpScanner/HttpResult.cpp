#include "HttpResult.h"

using namespace std;
using namespace HttpScanner_HttpScanner;



HttpResult::HttpResult(string resourceUrl, unique_ptr<vector<string>> childLinks)
	: _resourceUrl(resourceUrl),
	_childLinks(move(childLinks))
{
}

HttpResult::~HttpResult()
{
}

string HttpScanner_HttpScanner::HttpResult::GetResourceUrl()
{
	return _resourceUrl;
}

vector<string> HttpResult::GetChildLinks()
{
	return *_childLinks;
}
