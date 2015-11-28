// HttpScannerConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HttpAgent.h"

using namespace HttpScanner_HttpScanner;

int main()
{
	HttpAgent httpAgent("https://casablanca.codeplex.com/wikipage?title=Http%20Client%20Tutorial", "C:\\Temp\\Output.txt");

	httpAgent.Run();

    return 0;
}

