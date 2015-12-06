#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "HttpResourceAcquisition.h"

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace utility::conversions;
using namespace concurrency::streams;       // Asynchronous streams
using namespace HttpScanner_HttpScanner;

HttpResourceAcquisition::HttpResourceAcquisition(shared_ptr<EndAgents> endAgents, 
	shared_ptr<concurrent_queue<string>> resourcesToAcquire, 
	shared_ptr<concurrent_vector<string>> resourcesAcquired,
	shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> resourcesToAnalyze)
	: _endAgents(endAgents),
	_resourcesToAcquire(resourcesToAcquire),
	_resourcesAcquired(resourcesAcquired),
	_resourcesToAnalyze(resourcesToAnalyze)
{
}

shared_ptr<HttpResource> HttpScanner_HttpScanner::HttpResourceAcquisition::Acquire(string url)
{
	http_client client(to_string_t(url));
	http_request request(methods::GET);
	auto response = client.request(request).get();
	return make_shared<HttpResource>(url,
		make_unique<wistringstream>(response.extract_string().get())
		);
}



HttpResourceAcquisition::~HttpResourceAcquisition()
{
}

bool HttpScanner_HttpScanner::HttpResourceAcquisition::IsEmpty()
{
	return _empty.load();
}

void HttpResourceAcquisition::Run()
{
	string url;

	while (_endAgents->IsSet())
	{
		if (_resourcesToAcquire->empty()) {
			_empty.store(true);
			continue;
		}

		if (!_resourcesToAcquire->try_pop(url)) {
			_empty.store(true);
			continue;
		}

		_empty.store(false);

		while (find(_resourcesAcquired->begin(), _resourcesAcquired->end(), url) != _resourcesAcquired->end()) {
			_resourcesToAcquire->try_pop(url);
		}

		try {
			_resourcesToAnalyze->push(Acquire(url));
		}
		catch (http_exception httpException) {
			continue;
		}
		catch (uri_exception exception) {
			continue;
		}

		_resourcesAcquired->push_back(url);
	}
}

