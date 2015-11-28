#include <queue>
#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include "Result.h"
#include "PrototypeScanner.h"
#include "HttpAgent.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace HttpScanner_HttpScanner;

void HttpScanner_HttpScanner::HttpAgent::WriteToOutputFile(vector<string>& outputUrlBuffer)
{
	for_each(outputUrlBuffer.begin(), outputUrlBuffer.end(), [this](string& url) {
		_outputFile << url << endl;
	});
}

HttpScanner_HttpScanner::HttpAgent::HttpAgent(std::string url, std::string filePath)
{
	_rootUrl = url;
	_outputFile.open(filePath);
}

HttpScanner_HttpScanner::HttpAgent::~HttpAgent()
{
	_outputFile.close();
}

void HttpScanner_HttpScanner::HttpAgent::Run()
{
	auto urlCount = 0;
	auto outputUrlBuffer = vector<string>();
	auto scanLocations = queue<string>();
	auto previousScanLocations = vector<string>();
	auto scanResults = queue<unique_ptr<Result>>();
	auto scanner = PrototypeScanner();

	scanLocations.push(_rootUrl);

	while (!scanLocations.empty())
	{
		auto current = scanLocations.front();

		unique_ptr<Result> result;
		try {
			result = scanner.Scan(current);
		}
		catch (http_exception httpException) {
			scanLocations.pop();
			continue;
		}
		catch (uri_exception exception) {
			scanLocations.pop();
			continue;
		}
		
		auto childLocations = result->Analyze();
		std::for_each(childLocations->begin(), childLocations->end(), [&scanLocations](std::string& current) {
			scanLocations.push(current);
		});
		if (10 == urlCount) {
			urlCount = 0;
			WriteToOutputFile(outputUrlBuffer);
			outputUrlBuffer.clear();
		}
		outputUrlBuffer.push_back(current);
		urlCount++;
		previousScanLocations.push_back(current);
		scanLocations.pop();
		while (find(previousScanLocations.begin(), previousScanLocations.end(), scanLocations.front()) != previousScanLocations.end()) {
			scanLocations.pop();
		}
	}
}
