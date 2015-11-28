#pragma once
#include <string>
#include <vector>
#include <memory>

namespace HttpScanner_HttpScanner {
	class Result
	{
	public:
		virtual std::unique_ptr<std::vector<std::string>> Analyze() = 0;
	};
}
