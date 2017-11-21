#ifndef HELPER_H
#define HELPER_H
#include <ctime>                                           //we will need time for mail purposes
#include <string>
#include <sstream>                                         //converting various types to strings

namespace Helper
    {
        template <class T>
        std::string ToString (const T &);                  //This function will convert any type to string.//No need- We are passing pointers

        struct DateTime
            {

                DateTime ()                                 //Constructor for this particular struct
                    {
                        time_t ms;                          //time_t is present under the ctime library. ms is a variable short for milliseconds.
                        time (&ms);                         //takes system time and places it into variable ms
                        struct tm *info = localtime(&ms);   //pointer info will point to the value returned by the localtime function.
                        D = info->tm_mday;                  //We will use D(Day) to fetch tm_mday from the info pointer.
                        m = info->tm_mon + 1;               //use m to fetch a month. tm_mon starts from 0 so add 1 to it to get correct month.
                        y = 1900 + info->tm_year;           //1900 is our reference year
                        M = info->tm_min;                   //M to fetch minutes
                        H = info->tm_hour;                  //H to fetch hour
                        S = info->tm_sec;                   //s for sec.
                    }

                DateTime (int D, int m, int y, int H, int M, int S)           //another constructor
                        : D(D), m(m), y(y), H(H), M(M), S(S) {}


                DateTime (int D, int m, int y)                                //constructor with time set to 0
                    : D(D), m(m), y(y), H(0), M(0), S(0) {}

                DateTime Now () const                                         //Method to get the current Date and Time
                    {

                        return DateTime();
                    }

                int D, m, y, H, M, S;

                std::string GetDateString() const                          //It will return time as DD.mm.YYYY
                    {


                        return std::string(D < 10 ? "0" : "") + ToString (D) +            //concatenate strings with formatting
                               std::string (m < 10 ? ".0" : ".") + ToString (m) + "." +    //if D is less than D use 0 otherwise empty strings
                               ToString (y);                                               //same goes for all
                    }

                std::string GetTimeString(const std::string &sep = ":") const             //Default separator for time is :
                    {





                        return std::string(H < 10 ? "0" : "") + ToString (H) + sep +
                               std::string (M < 10 ? "0" : "") + ToString (M) + sep+    //Same concatenation as above and will return HH:mm:ss
                               std::string (S < 10 ? sep : "") + ToString (S);
                    }


                std::string GetDateTimeString (const std::string &sep = ":") const       //This function will bind the two functions from
                    {                                                                    // above and will return proper time as DD.mm.YYYY HH:MM:SS
                        return GetDateString () + " " + GetTimeString (sep);
                    }
            };


        template <class T>
        std::string ToString (const T &e)                           //This function will take anything that supports insertion operator
            {

                std::ostringstream s;                               //object of type output string stream

                s << e;                                             // << is the insertion operator and e is any type that supports insertion operator

                return s.str();
            }


        void WriteAppLog (const std::string &s)                     //Keylogger is not dependent on this function this is just used for debugging
            {

            std::ofstream file ("AppLog.txt", std::ios::app);         //std namespace and app stands for append

            file << "[" << Helper::DateTime().GetDateTimeString () << "]" << "\n" << s << std::endl << "\n";    //Calling from the helper header
            file.close();                                //close the file after using
            }
    }
#endif

