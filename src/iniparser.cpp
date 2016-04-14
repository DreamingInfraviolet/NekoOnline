
#include "iniparser.h"
#include "util.h"
#include <fstream>
#include <iostream>

namespace FileSystem
{
    void IniParser::trim(std::string& str)
    {
        unsigned startPos = str.size();
        unsigned endPos = 0;
        for (unsigned i = 0; i < str.size(); ++i)
        {
            if (!isblank(str[i]))
            {
                startPos = i;
                break;
            }
        }

        for (unsigned i = str.size() - 1; i > startPos; --i)
        {
            if (!isblank(str[i]))
            {
                endPos = i + 1;
                break;
            }
        }

        if (startPos == str.size() || endPos == 0)
        {
            str.clear();
            return;
        }
        else
            str = str.substr(startPos, endPos - startPos);
    }

    bool IniParser::parseFile(const char path[])
    {
        std::fstream file(path, std::ios::in);

        if (!file)
        {
            std::cout << "Error opening " << path << "\n";
            return false;
        }

        std::string activeSystem = "DEFAULT";
        std::string temp;

        unsigned lineNo = 0;
        static unsigned ustrnpos = (unsigned)std::string::npos;

        while (getline(file, temp))
        {
            ++lineNo;

            //Discard invalid lines.
            if (temp.size() == 0) continue;

            unsigned eqPos = temp.find('=');

            if (eqPos == ustrnpos)
            {
                //Check whether it's a comment.
                trim(temp);
                if (!temp.size()) continue; //Just empty line

                //If a command to change the System:
                if (temp[0] == '[')
                {
                    unsigned endPos = temp.rfind(']');

                    if (endPos == ustrnpos) //If no ending symbol (invalid. Could guess, but best ignored)
                    {
						std::cout << "Invalid line " << lineNo << " in " << path <<
							" System declaration missing ']'.\n";
                        continue;
                    }

                    activeSystem = temp.substr(1, endPos - 1);
					for (char&c : activeSystem)
						c = toupper(c);
                    trim(activeSystem);

                    continue;
                }

                if (temp[0] != '#') //Not a comment - error
                    std::cout << "Invalid line " << lineNo << " in " << path <<
                    " : Non-comment line does not contain '='.\n";

                continue; //No need to proccess further if comment
            }

            

			for (char& c : temp)
				c = tolower(c);

            //split (into arr[0] and arr[1]) (left hand size, right hand size)
            std::string arr[2];
            arr[0] = temp.substr(0, eqPos);
            arr[1] = temp.substr(eqPos + 1);

            trim(arr[0]);
            trim(arr[1]);

            //If either side is empty:
            if (arr[0].size() == 0 || arr[1].size() == 0)
            {
                std::cout << "Invalid line " << lineNo << " in " << path <<
                    " : One or more sides of an association are empty.";
                continue;
            }

            //If still a comment:
            if ((arr[0])[0] == '#')
                continue;

            //If commands contain spaces:
            if (!(none_of(arr[0].begin(), arr[0].end(), isblank) &&
                none_of(arr[1].begin(), arr[1].end(), isblank)))
            {
                std::cout << "Invalid line " << lineNo << " in " << path << " : Name of command contains spaces.";
                continue;
            }

            //If system does not exist:
            SystemListType::iterator sPos = systems.find(activeSystem);
            if (sPos == systems.end())
            {
                std::map<std::string, std::string> entry;
                entry[arr[0]] = arr[1];
                systems[activeSystem] = entry;
            }

            //If system exists, update the internal map:
            else
            {
                sPos->second[arr[0]] = arr[1];
            }

        }

        init = true;
        return true;
    }


    void IniParser::clear()
    {
        systems.clear();
        init = false;
    }

    bool IniParser::initialised()
    {
        return init;
    }

    SystemListType::const_iterator IniParser::find(const std::string& system)
    {
        return systems.find(system);
    }

    SystemListType::const_iterator IniParser::begin()
    {
        return systems.begin();
    }
    SystemListType::const_iterator IniParser::end()
    {
        return systems.end();
    }

}//End of namespace