#include "HttpAgent.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace HttpScanner_HttpScanner;

HttpScanner_HttpScanner::HttpAgent::HttpAgent(std::string url, std::string filePath) 
	: _rootUrl(url),
	_httpResourceAcquisitionAgents(make_unique<vector<shared_ptr<HttpResourceAcquisition>>>()),
	_httpResourceAnalysisAgents(make_unique<vector<shared_ptr<HttpResourceAnalysis>>>()),
	_dataContext(make_shared<DataContext>(url))
{
	_outputFile.open(filePath);
}

HttpScanner_HttpScanner::HttpAgent::~HttpAgent()
{
	_outputFile.close();
}

void HttpScanner_HttpScanner::HttpAgent::WriteToOutputFile(shared_ptr<EndAgents> endAgents)
{
	vector<unique_ptr<HttpResult>> resultsToOutput;
	while (endAgents->IsSet()) {
		if (_dataContext->GetHttpResults(resultsToOutput)) {
			for_each(resultsToOutput.begin(), resultsToOutput.end(), [this](unique_ptr<HttpResult>& httpResult) {
				_outputFile << httpResult->GetResourceUrl() << endl;
			});
			resultsToOutput.clear();
		}
	}
}

unique_ptr<vector<unique_ptr<thread>>> HttpScanner_HttpScanner::HttpAgent::StartAgents()
{
	auto threads = make_unique <vector<unique_ptr<thread>>>();
	
	for_each(_httpResourceAnalysisAgents->begin(), _httpResourceAnalysisAgents->end(), [&threads](shared_ptr<HttpResourceAnalysis>  httpResourceAnalysisAgent) {
		auto analysisThread = make_unique<thread>(&HttpResourceAnalysis::Run, httpResourceAnalysisAgent);
		threads->push_back(move(analysisThread));
	});

	for_each(_httpResourceAcquisitionAgents->begin(), _httpResourceAcquisitionAgents->end(), [&threads](shared_ptr<HttpResourceAcquisition> httpResourceAcquisitionAgent) {
		auto acquisitionThread = make_unique<thread>(&HttpResourceAcquisition::Run, httpResourceAcquisitionAgent);
		threads->push_back(move(acquisitionThread));
	});

	return move(threads);
}

void HttpScanner_HttpScanner::HttpAgent::Run()
{
	auto endbool = make_shared<atomic<bool>>(true);
	auto endAgents = make_shared<EndAgents>(endbool);
	
	for (auto count = 0; count < 10; count++) {
		_httpResourceAcquisitionAgents->push_back(make_shared<HttpResourceAcquisition>(endAgents, _dataContext));
	}

	for (auto count = 0; count < 1; count++) {
		_httpResourceAnalysisAgents->push_back(make_shared<HttpResourceAnalysis>(endAgents, _dataContext));
	}

	auto agentThreads = StartAgents();

	auto outputFuture = thread(&HttpAgent::WriteToOutputFile, this, endAgents);

	bool end = false;
	while (!end) {
		for_each(_httpResourceAcquisitionAgents->begin(), _httpResourceAcquisitionAgents->end(), [&end](shared_ptr<HttpResourceAcquisition>& httpResourceAcquisitionAgent) {
			if (end) {
				end = httpResourceAcquisitionAgent->IsEmpty();
			}
		});

		for_each(_httpResourceAnalysisAgents->begin(), _httpResourceAnalysisAgents->end(), [&end](shared_ptr<HttpResourceAnalysis>& httpResourceAnalysisAgent) {
			if (end) {
				end = httpResourceAnalysisAgent->IsEmpty();
			}
		});
	}

	endbool->store(true);

	std::for_each(agentThreads->begin(), agentThreads->end(), [](unique_ptr<thread>& agentThread) {
		agentThread->join();
	});

	outputFuture.join();
}
