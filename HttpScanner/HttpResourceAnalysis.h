#pragma once
#include <string>
#include "EndAgents.h"
#include "DataContext.h"
#include "HttpResource.h"

using namespace std;
using namespace Concurrency;

namespace HttpScanner_HttpScanner {
	class HttpResourceAnalysis
	{
	private:
		atomic<bool> _empty;
		shared_ptr<EndAgents> _endAgents;
		shared_ptr<DataContext> _dataContext;
	public:
		HttpResourceAnalysis(
			shared_ptr<EndAgents> endAgents,
			shared_ptr<DataContext> dataContext
			);
		~HttpResourceAnalysis();
		bool IsEmpty();
		void Run();
	};
}

