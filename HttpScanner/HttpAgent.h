#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace HttpScanner_HttpScanner {
	class HttpAgent
	{
	private:
		string _rootUrl;
		fstream _outputFile;
		void WriteToOutputFile(vector<string>& outputUrlBuffer);
	public:
		HttpAgent(string url, string filePath);
		~HttpAgent();
		void Run();
	};
}

