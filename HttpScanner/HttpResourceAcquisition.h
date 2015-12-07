#pragma once
#include <string>
#include "DataContext.h"
#include "EndAgents.h"
#include "HttpResource.h"

using namespace std;
using namespace Concurrency;

namespace HttpScanner_HttpScanner {
	class HttpResourceAcquisition
	{
	private:
		atomic<bool> _empty;
		shared_ptr<EndAgents> _endAgents;
		shared_ptr<DataContext> _dataContext;
		unique_ptr<HttpResource> Acquire(string url);
	public:
		HttpResourceAcquisition(
			shared_ptr<EndAgents> endAgents,
			shared_ptr<DataContext> dataContext
			);
		~HttpResourceAcquisition();
		bool IsEmpty();
		void Run();
	};
}
