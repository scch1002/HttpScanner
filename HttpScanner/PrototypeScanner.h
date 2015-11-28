#pragma once
#include "Scanner.h"

namespace HttpScanner_HttpScanner {
	class PrototypeScanner : public Scanner
	{
	public:
		PrototypeScanner();
		~PrototypeScanner();

		// Inherited via Scanner
		virtual std::unique_ptr<Result> Scan(std::string url) override;
	};
}

