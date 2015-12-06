#pragma once
#include <string>
#include <concurrent_queue.h>
#include <concurrent_vector.h>
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
		shared_ptr<concurrent_queue<string>> _resourcesToAcquire;
		shared_ptr<concurrent_vector<string>> _resourcesAcquired;
		shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> _resourcesToAnalyze;
		shared_ptr<HttpResource> Acquire(string url);
	public:
		HttpResourceAcquisition(
			shared_ptr<EndAgents> endAgents,
			shared_ptr<concurrent_queue<string>> resourcesToAcquire,
			shared_ptr<concurrent_vector<string>> resourcesAcquired,
			shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> resourcesToAnalyze
			);
		~HttpResourceAcquisition();
		bool IsEmpty();
		void Run();
	};
}
