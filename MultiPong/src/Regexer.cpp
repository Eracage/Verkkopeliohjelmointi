#include <Regexer.h>

std::string regexArgPort(int argc, char * argv[], int first, std::string def)
{
	std::regex regexPort("^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");
	std::smatch regexMatch;

	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string a(argv[i]);
			if (std::regex_search(a, regexMatch, regexPort))
			{
				return regexMatch.str();
			}
		}
	}

	return def;
}

std::string regexArgIP(int argc, char * argv[], int first, std::string def)
{
	std::regex regexIp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	std::smatch regexMatch;

	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string a(argv[i]);
			if (std::regex_search(a, regexMatch, regexIp))
			{
				return regexMatch.str();
			}
		}
	}


	return def;
}
