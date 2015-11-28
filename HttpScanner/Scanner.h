#pragma once
#include <string>
#include <memory>
#include "Result.h"

namespace HttpScanner_HttpScanner {
	class Scanner
	{
	public:
		virtual std::unique_ptr<Result> Scan(std::string url) = 0;
	};
}

