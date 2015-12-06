#include "HttpResult.h"

using namespace std;
using namespace HttpScanner_HttpScanner;



HttpResult::HttpResult(unique_ptr<vector<string>> childLinks)
	: _childLinks(move(childLinks))
{
}

HttpResult::~HttpResult()
{
}

vector<string> HttpResult::GetChildLinks()
{
	return *_childLinks;
}
