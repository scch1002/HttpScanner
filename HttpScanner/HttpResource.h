#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "HttpResult.h"

using namespace std;

namespace HttpScanner_HttpScanner {
	class HttpResource
	{
	private:
		string _resourceUrl;
		unique_ptr<wistringstream> _resourceData;
		unique_ptr<std::vector<std::string>> GetChildLinks();
		unique_ptr<vector<string>> CleanChildLinks(std::unique_ptr<std::vector<std::string>> childLinks);
	public:
		HttpResource(string resourceUrl,
			unique_ptr<wistringstream> resourceData);
		~HttpResource();
		string GetResourceUrl();
		unique_ptr<HttpResult> Analyze();
	};
}
