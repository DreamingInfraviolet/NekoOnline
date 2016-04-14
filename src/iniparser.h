#pragma once
#include <map>

namespace FileSystem
{
    /*
       # : comments [STUFF] : subsystem '=' operator needed for validity. */

    /**
     *Parses a loosely defined .ini file.
     *Reads a .ini file, searching for a '=' character. If one is not found, the line is discarded.
     *It reads the file on per-line basis, so each command must have its own line.
     *If the comment '#' character is found at the beginning, the line is ignored altogether.
     *
     *Results in a list of string associations. For each valid line found containing '=',
     *an entry is added to an internal list associating the string before '=' with the one after it.
     *No spaces anywhere are guaranteed (unless there is a bug).
     *
     *If an associations was already seen before, it is overwritten. Only the latest association in the file counts.
     *
     *[<SYSTEM>] format in the file indicates a particular system to be associated with the following lines.
     *Every association is placed into a particular system. For example, the FOV, Brightness, etc. might all be placed into a [VIDEO] system.
     *To access those associations, one would have to recover them from the "VIDEO" system in the class. If no system is defined in the file for a variable,
     *it is placed into the DEFAULT system.
     */
    typedef std::map<std::string, std::map<std::string, std::string> > SystemListType;
    class IniParser
    {
    public:


        /**
        * Default constructor initialising a single boolean.
        */
        IniParser() : init(false){}

        /**
        * Parses a file, filling the class with retrievable information.
        * @param path The path of the file to parse.
        * @return success Whether the operation succeeded or not.
        */
        bool parseFile(const char path[]);

        /**
        * Resets the state of the parser, removing all information on parsed files.
        */
        void clear();

        /**
        * If the class is initialised, then it is ready for use by other objects.
        * @return Whether the class is fully initialised, having parsed a file.
        */
        bool initialised();

        /**
        * Searches for a system in the generated list.
        * @param system The system to be searched for.
        * @return A const_iterator to the appropriate systems entry, or an end() iterator if not found.
        */
        SystemListType::const_iterator find(const std::string& system);

        /**
        * The start iterator of the map of systems.
        * @return A const_iterator to the first element.
        */
        SystemListType::const_iterator begin();

        /**
        * The end iterator of the map of systems.
        * @return A const_iterator to the element beyond last.
        */
        SystemListType::const_iterator end();



    private:
        /**
         * Removes the blanks from both the left and right of a string.
         * @param str The input string to be modified, passed by reference.
         */
        void trim(std::string& str);

        bool init;

        //map<system, map<key,value> >
        SystemListType systems;

    };
}