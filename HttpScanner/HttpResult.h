#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace HttpScanner_HttpScanner {
	class HttpResult
	{
	private:
		unique_ptr<vector<string>> _childLinks;
	public:
		HttpResult(unique_ptr<vector<string>> childLinks);
		~HttpResult();
		vector<string> GetChildLinks();
	};
}
