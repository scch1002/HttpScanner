#include "HttpResourceAnalysis.h"

using namespace std;
using namespace HttpScanner_HttpScanner;


HttpResourceAnalysis::HttpResourceAnalysis(
	shared_ptr<EndAgents> endAgents,
	shared_ptr<DataContext> dataContext
	)
	: _endAgents(endAgents),
	_dataContext(dataContext)
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
	unique_ptr<HttpResource> resource;

	while (_endAgents->IsSet())
	{
		if (!_dataContext->GetHttpResourceToAnalyze(resource)) {
			_empty.store(true);
			continue;
		}

		_empty.store(false);

		auto result = resource->Analyze();

		_dataContext->AddResourcesToAcquire(make_shared<vector<string>>(result->GetChildLinks()));

		_dataContext->AddHttpResult(move(result));
	}
}
