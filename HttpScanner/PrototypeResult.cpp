#include <regex>
#include <pugixml.hpp>
#include "PrototypeResult.h"

using namespace std;

using namespace utility::conversions;
using namespace concurrency::streams;

HttpScanner_HttpScanner::PrototypeResult::~PrototypeResult()
{
}

std::unique_ptr<std::vector<std::string>> HttpScanner_HttpScanner::PrototypeResult::GetChildLinks()
{
	wstring body;
	try {
		body = _response->extract_string().get();
	}
	catch (http_exception exception) {
		return make_unique<vector<string>>();
	}
	
	wistringstream ss(body);

	pugi::xml_document doc;
	doc.load(ss);

	auto nodes = doc.select_nodes("//a");
	
	auto links = make_unique<std::vector<std::string>>();;

	for (auto& node : nodes)
	{
		auto value = node.node().attribute("href").value();
		 links->push_back(value);
	}

	return move(links);
}

unique_ptr<vector<string>> HttpScanner_HttpScanner::PrototypeResult::CleanChildLinks(std::unique_ptr<std::vector<std::string>> childLinks)
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

std::unique_ptr<std::vector<std::string>>  HttpScanner_HttpScanner::PrototypeResult::Analyze()
{
	auto resultLinks = GetChildLinks();
	
	auto cleanLinks = CleanChildLinks(move(resultLinks));

	return move(cleanLinks);
}
