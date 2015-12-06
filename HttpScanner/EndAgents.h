#pragma once
#include <memory>
#include <atomic>

using namespace std;

namespace HttpScanner_HttpScanner {
	class EndAgents
	{
	private:
		shared_ptr<atomic<bool>> _flag;
	public:
		EndAgents(shared_ptr<atomic<bool>> flag);
		~EndAgents();
		bool IsSet();
	};
}

