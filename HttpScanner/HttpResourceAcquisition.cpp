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
	shared_ptr<DataContext> dataContext)
	: _endAgents(endAgents),
	_dataContext(dataContext)
{
}

unique_ptr<HttpResource> HttpScanner_HttpScanner::HttpResourceAcquisition::Acquire(string url)
{
	http_client client(to_string_t(url));
	http_request request(methods::GET);
	auto response = client.request(request).get();
	return make_unique<HttpResource>(url,
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
		if (!_dataContext->GetResourceToAcquire(url)) {
			_empty.store(true);
			continue;
		}

		_empty.store(false);

		try {
			_dataContext->AddHttpResourceToAnalyze(Acquire(url));
		}
		catch (http_exception httpException) {
			continue;
		}
		catch (uri_exception exception) {
			continue;
		}

	}
}

