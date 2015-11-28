#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "PrototypeResult.h"
#include "PrototypeScanner.h"

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace utility::conversions;
using namespace concurrency::streams;       // Asynchronous streams
using namespace HttpScanner_HttpScanner;

PrototypeScanner::PrototypeScanner()
{
}

PrototypeScanner::~PrototypeScanner()
{
}

unique_ptr<Result> PrototypeScanner::Scan(string url)
{
	http_client client(to_string_t(url));
	http_request request(methods::GET);
	auto response = make_unique<http_response>(client.request(request).get());

	return (unique_ptr<Result>)make_unique<PrototypeResult>(move(response));
}
