#include "HttpResourceAnalysis.h"

using namespace std;
using namespace HttpScanner_HttpScanner;


HttpResourceAnalysis::HttpResourceAnalysis(
	shared_ptr<EndAgents> endAgents,
	shared_ptr<concurrent_queue<string>> resourcesToAcquire, 
	shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> resourcesToAnalyze,
	shared_ptr<concurrent_queue<string>> processedResources)
	: _endAgents(endAgents),
	_resourcesToAcquire(resourcesToAcquire),
	_resourcesToAnalyze(resourcesToAnalyze),
	_processedResources(processedResources)
{
}

HttpResourceAnalysis::~HttpResourceAnalysis()
{
}

bool HttpScanner_HttpScanner::HttpResourceAnalysis::IsEmpty()
{
	return _empty.load();
}

void HttpResourceAnalysis::Run()
{
	shared_ptr<HttpResource> resource;

	while (_endAgents->IsSet())
	{
		if (_resourcesToAnalyze->empty()) {
			_empty.store(true);
			continue;
		}

		if (!_resourcesToAnalyze->try_pop(resource)) {
			_empty.store(true);
			continue;
		}

		_empty.store(false);

		auto result = resource->Analyze();

		auto childLinks = result->GetChildLinks();

		for_each(childLinks.begin(), childLinks.end(), [this](string & url) {
			_resourcesToAcquire->push(url);
		});

		_processedResources->push(resource->GetResourceUrl());
	}
}
