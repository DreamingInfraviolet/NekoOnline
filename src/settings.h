#pragma once

#include "iniparser.h"
#include <map>
#include <iostream>

namespace Config
{

    using namespace FileSystem;
    using namespace std;

    typedef map<string, map<string, string> > SystemList;

    /**
    * An abstract class designed to allow a class to easily make use of .ini file initialisation.
    * A class that extends from this class is capable of attaching itself to a parser object, affiliating itself with a particular System in the parser.
    * Once attached, variables can be created inside the derived class, and initialised inside the virtual initialiseSettigns() function using the iniitaliseVar function.
    */
    class Settings
    {
    private:
        /**
        * These functions are available to perform default conversions from the ini right-hand side to the appropriate type.
        * Types that are not listed here should not be used.
        */
        bool convert(int& out, const string& in);
        bool convert(unsigned& out, const string& in);
        bool convert(long& out, const string& in);
        bool convert(long long& out, const string& in);
        bool convert(unsigned long long& out, const string& in);
        bool convert(float& out, const string& in);
        bool convert(double& out, const string& in);
		bool convert(bool& out, string in);
		bool convert(string& out, string in);

        string activeSystem;
        IniParser* parser;

    public:
        SystemList::const_iterator systemLoc; //Pointer to current system.

        /**
        * Default constructor doing next to nothing.
        */
        Settings();

        /**
        * Constructor taking two parameters that get passed into the attachParser function.
        * @param parser_ The parser to be used.
        * @param activeSystem The system to be affiliated with.
        */
        Settings(IniParser* parser_, const char* activeSystem_);

        /**
        * Attached a parser to the object, allowing its contents to be traversed indirectly when searching for initialisation information for variables.
        * @param parser_ The parser to be used.
        * @param activeSystem_ The system to be affiliated with.
        * @return
        */
        bool attachParser(IniParser* parser_, string activeSystem_);

        /**
        * Initialises a variable using the parser.
        * @param name The name that is used in the ini file to initialise this variable.
        * @param defaultValue The default value of the variable to use if none is found. This is used to determine the type of the return value, and is not optional.
        * @return value The value to be assigned to the varaible.
        * Warning: Not fully type safe! Care must be taken.
        */
        template <class T> T initialiseVar(string name, T defaultValue)
        {
            T output;

            if (parser == nullptr || !parser->initialised())
                return defaultValue;

			for (char& c : name)
				c = tolower(c);

            //Look for the command name in the parser.
            auto commandLoc = systemLoc->second.find(name);
            if (commandLoc == systemLoc->second.end())
            {
                //If command not found, set default.
                return defaultValue;
            }

            //If the command is found, parse it appropriately:


            if (!convert(output, commandLoc->second))
            {
                std::cout << "Warning: Could not parse ini value " << commandLoc->second << " for element " << commandLoc->first << ".";
                return defaultValue;
            }
            else
                return output;
        }
        /**
        * @brief initialiseSettings A function to be overriden. It is meant merely to put each variable declared in the new class equal to an appropriate initialiseVar call.
        */
        virtual void initialise() = 0;

    };

    /*
    //Example:
    class CameraSettings : public Settings
    {
    public:
    float fov;
    void init() override
    {
    fov = initialiseVar("Fov", 50.f);
    }
    };
    int main()
    {
    FileSystem::IniParser engineParser;
    gameParser.parseFile("Game/gameconfig.ini");
    CameraSettings cameraSet;
    cameraSet.attachParser(&gameParser, "CAMERA");
    cameraSet.init();
    }
    */

}