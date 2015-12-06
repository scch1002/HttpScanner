#include "EndAgents.h"

HttpScanner_HttpScanner::EndAgents::EndAgents(shared_ptr<atomic<bool>> flag)
{
	_flag = flag;
}

HttpScanner_HttpScanner::EndAgents::~EndAgents()
{
}

bool HttpScanner_HttpScanner::EndAgents::IsSet()
{
	return _flag->load();
}





