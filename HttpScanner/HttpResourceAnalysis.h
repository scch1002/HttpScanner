#pragma once
#include <string>
#include "concurrent_queue.h"
#include "EndAgents.h"
#include "HttpResource.h"

using namespace std;
using namespace Concurrency;

namespace HttpScanner_HttpScanner {
	class HttpResourceAnalysis
	{
	private:
		atomic<bool> _empty;
		shared_ptr<EndAgents> _endAgents;
		shared_ptr<concurrent_queue<string>> _resourcesToAcquire;
		shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> _resourcesToAnalyze;
		shared_ptr<concurrent_queue<string>> _processedResources;
	public:
		HttpResourceAnalysis(
			shared_ptr<EndAgents> endAgents,
			shared_ptr<concurrent_queue<string>> resourcesToAcquire,
			shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> resourcesToAnalyze,
			shared_ptr<concurrent_queue<string>> processedResources
			);
		~HttpResourceAnalysis();
		bool IsEmpty();
		void Run();
	};
}

