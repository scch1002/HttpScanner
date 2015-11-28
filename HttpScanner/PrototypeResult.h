#pragma once
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "Result.h"

using namespace std;
using namespace web::http;

namespace HttpScanner_HttpScanner {
	class PrototypeResult : public Result
	{
	private:
		unique_ptr<http_response> _response;
		unique_ptr<vector<string>> GetChildLinks();
		unique_ptr<vector<string>> CleanChildLinks(std::unique_ptr<std::vector<std::string>> childLinks);
	public:
		PrototypeResult(unique_ptr<http_response> response) : _response(move(response)) {};
		~PrototypeResult();

		// Inherited via Result
		virtual std::unique_ptr<std::vector<std::string>> Analyze() override;
	};
}
