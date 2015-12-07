#include <regex>
#include <pugixml.hpp>
#include "HttpResource.h"

using namespace std;
using namespace utility::conversions;
using namespace concurrency::streams;
using namespace HttpScanner_HttpScanner;

HttpResource::HttpResource(string resourceUrl, unique_ptr<wistringstream> resourceData)
	: _resourceUrl(resourceUrl),
	_resourceData(move(resourceData))
{
}

HttpResource::~HttpResource()
{
}

string HttpResource::GetResourceUrl()
{
	return _resourceUrl;
}

std::unique_ptr<std::vector<std::string>> HttpResource::GetChildLinks()
{
	pugi::xml_document doc;
	doc.load(*_resourceData);

	auto nodes = doc.select_nodes("//a");

	auto links = make_unique<std::vector<std::string>>();;

	for (auto& node : nodes)
	{
		auto value = node.node().attribute("href").value();
		links->push_back(value);
	}

	return move(links);
}

unique_ptr<vector<string>> HttpResource::CleanChildLinks(std::unique_ptr<std::vector<std::string>> childLinks)
{
	auto regexString = R"(^https?:\/\/.*)";

	auto newEnd = remove_if(childLinks->begin(), childLinks->end(), [&regexString](string& link) {

		regex http_regex(regexString);
		cmatch http_match;

		bool match = !regex_match(link.c_str(), http_match, http_regex);

		return match;
	});

	if (newEnd != childLinks->end())
	{
		childLinks->erase(newEnd, childLinks->end());
	}

	return move(childLinks);
}

unique_ptr<HttpResult> HttpScanner_HttpScanner::HttpResource::Analyze()
{
	auto resultLinks = GetChildLinks();

	auto cleanLinks = CleanChildLinks(move(resultLinks));

	return make_unique<HttpResult>(_resourceUrl, move(cleanLinks));
}
