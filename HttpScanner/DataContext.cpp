#include "DataContext.h"

using namespace std;
using namespace HttpScanner_HttpScanner;

DataContext::DataContext(string urlRoot)
{
	_resourcesToAcquire.push_back(urlRoot);
}

DataContext::~DataContext()
{
}

bool DataContext::GetHttpResults(vector<unique_ptr<HttpResult>>& httpResults)
{
	lock_guard<mutex> httpResultsToProcessGuard(_httpResultsToProcessLock);

	if (_httpResultsToProcess.empty()) {
		return false;
	}

	for_each(_httpResultsToProcess.begin(), _httpResultsToProcess.end(), [&httpResults](unique_ptr<HttpResult>& httpResult) {
		httpResults.push_back(move(httpResult));
	});
	_httpResultsToProcess.clear();

	return true;
}

void DataContext::AddHttpResult(unique_ptr<HttpResult> result)
{
	lock_guard<mutex> httpResultsToProcessGuard(_httpResultsToProcessLock);

	_httpResultsToProcess.push_back(move(result));
}

bool DataContext::GetResourceToAcquire(string& resourceUrl)
{
	lock_guard<mutex> resourceToAcquireGuard(_resourcesToAcquireLock);

	if (_resourcesToAcquire.empty()) {
		return false;
	}

	resourceUrl = _resourcesToAcquire.front();
	_resourcesAcquired.push_back(_resourcesToAcquire.front());
	_resourcesToAcquire.pop_front();
	
	return true;
}

void DataContext::AddResourcesToAcquire(shared_ptr<vector<string>> resourcesToAcquire)
{
	lock_guard<mutex> resourcesToAcquireGuard(_resourcesToAcquireLock);

	while(!resourcesToAcquire->empty())
	{
		while (find(_resourcesAcquired.begin(), _resourcesAcquired.end(), resourcesToAcquire->back()) != _resourcesAcquired.end()) {
			resourcesToAcquire->pop_back();
			if (resourcesToAcquire->empty()) {
				return;
			}
		}

		while (find(_resourcesToAcquire.begin(), _resourcesToAcquire.end(), resourcesToAcquire->back()) != _resourcesToAcquire.end()) {
			resourcesToAcquire->pop_back();
			if (resourcesToAcquire->empty()) {
				return;
			}
		}

		_resourcesToAcquire.push_back(resourcesToAcquire->back());
		resourcesToAcquire->pop_back();
	}
}

bool DataContext::GetHttpResourceToAnalyze(unique_ptr<HttpResource>& resourceToAnalyze)
{
	lock_guard<mutex> _resourcesToAcquireGuard(_resourcesToAnalyzeLock);

	if (_resourcesToAnalyze.empty()) {
		return false;
	}

	resourceToAnalyze = move(_resourcesToAnalyze.front());
	_resourcesToAnalyze.pop_front();

	return true;
}

void DataContext::AddHttpResourceToAnalyze(unique_ptr<HttpResource> httpResource)
{
	lock_guard<mutex> _resourcesToAcquireGuard(_resourcesToAnalyzeLock);

	_resourcesToAnalyze.push_back(move(httpResource));
}
