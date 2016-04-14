#include "settings.h"
#include <iostream>

namespace Config
{
    using namespace FileSystem;

    Settings::Settings()
    {
        parser = nullptr;
    }

    Settings::Settings(IniParser* parser_, const char* activeSystem_)
    {
        parser = nullptr;
        attachParser(parser_, activeSystem_);
    }

    bool Settings::convert(int& out, const std::string& in)
    {
        out = atoi(in.c_str());
        return true;
    }
    bool Settings::convert(unsigned& out, const std::string& in)
    {
        out = (unsigned)strtoul(in.c_str(), NULL, 0);
        return true;
    }
    bool Settings::convert(long& out, const std::string& in)
    {
        out = atol(in.c_str());
        return true;
    }
    bool Settings::convert(long long& out, const std::string& in)
    {
        out = atoll(in.c_str());
        return true;
    }
    bool Settings::convert(unsigned long long& out, const std::string& in)
    {
        out = strtoull(in.c_str(), NULL, 0);
        return true;
    }
    bool Settings::convert(float& out, const std::string& in)
    {
        out = strtof(in.c_str(), NULL);
        return true;
    }
    bool Settings::convert(double& out, const std::string& in)
    {
        out = strtod(in.c_str(), NULL);
        return true;
    }
    bool Settings::convert(bool& out, std::string in)
    {
		for (char& c : in)
			c = tolower(c);

        if (in == "true" || in == "1")
        {
            out = true;
            return true;
        }
        else if (in == "false" || in == "0")
        {
            out = false;
            return true;
        }
        //If not a proper boolean value
        return false;
    }

	bool Settings::convert(string& out, string in)
	{
		out = in;
		return true;
	}

    bool Settings::attachParser(IniParser* parser_, string activeSystem_)
    {
        //Check parser integrity:
        if (parser_ == nullptr || !parser_->initialised())
        {
            std::cout << ("Could not prepare Settings: Ini parser not initialised!");
            return false;
        }

		for (char& c : activeSystem_)
			c = toupper(c);

        //Connect with the appropriate System:
        SystemList::const_iterator newSystemLoc = parser_->find(activeSystem_);
        if (newSystemLoc == parser_->end()) //If System not found
            return false;

        parser = parser_;
        activeSystem = activeSystem_;
        systemLoc = newSystemLoc;
        return true;
    }


}
