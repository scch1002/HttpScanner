#pragma once
#include <concurrent_queue.h>
#include <concurrent_vector.h>
#include "HttpResource.h"
#include "HttpResult.h"

using namespace std;
using namespace Concurrency;

namespace HttpScanner_HttpScanner {
	class DataContext
	{
	private:
		mutex _resourcesToAcquireLock;
		mutex _resourcesAcquiredLock;
		mutex _resourcesToAnalyzeLock;
		mutex _httpResultsToProcessLock;
		deque<string> _resourcesToAcquire;
		deque<string> _resourcesAcquired;
		deque<unique_ptr<HttpResource>> _resourcesToAnalyze;
		vector<unique_ptr<HttpResult>> _httpResultsToProcess;
	public:
		DataContext(string urlRoot);
		~DataContext();
		bool GetHttpResults(vector<unique_ptr<HttpResult>>& httpResults);
		void AddHttpResult(unique_ptr<HttpResult> result);
		bool GetResourceToAcquire(string& resourceUrl);
		void AddResourcesToAcquire(shared_ptr<vector<string>> resourcesToAcquire);
		bool GetHttpResourceToAnalyze(unique_ptr<HttpResource>& resourceToAnalyze);
		void AddHttpResourceToAnalyze(unique_ptr<HttpResource> httpResource);
	};
}

