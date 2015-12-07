#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace HttpScanner_HttpScanner {
	class HttpResult
	{
	private:
		string _resourceUrl;
		unique_ptr<vector<string>> _childLinks;
	public:
		HttpResult(string resourceUrl, unique_ptr<vector<string>> childLinks);
		~HttpResult();
		string GetResourceUrl();
		vector<string> GetChildLinks();
	};
}
