#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <memory>
#include <concurrent_queue.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include "concurrent_vector.h"
#include "EndAgents.h"
#include "HttpResourceAcquisition.h"
#include "HttpResourceAnalysis.h"

using namespace std;
using namespace Concurrency;

namespace HttpScanner_HttpScanner {
	class HttpAgent
	{
	private:
		string _rootUrl;
		fstream _outputFile;
		shared_ptr<concurrent_queue<string>> _processedResourses;
		shared_ptr<concurrent_queue<string>> _resourcesToAcquire;
		shared_ptr<concurrent_vector<string>> _resourcesAcquired;
		shared_ptr<concurrent_queue<shared_ptr<HttpResource>>> _resourcesToAnalyze;
		unique_ptr<vector<shared_ptr<HttpResourceAcquisition>>> _httpResourceAcquisitionAgents;
		unique_ptr<vector<shared_ptr<HttpResourceAnalysis>>> _httpResourceAnalysisAgents;
		void WriteToOutputFile(shared_ptr<EndAgents> endAgents);
		unique_ptr<vector<unique_ptr<thread>>> StartAgents();
	public:
		HttpAgent(std::string url, std::string filePath);
		~HttpAgent();

		void Run();
	};
}
