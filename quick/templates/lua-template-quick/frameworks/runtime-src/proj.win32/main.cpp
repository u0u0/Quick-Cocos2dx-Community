#include "main.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "cocos2d.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <Commdlg.h>
#include <Shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <shellapi.h>

#include "ProjectConfig/ProjectConfig.h"
USING_NS_CC;

extern void register_runtime_override_function(lua_State* tolua_S);
extern string& replaceAll(string& str, const string& old_value, const string& new_value);

// uncomment below line, open debug console
//#define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LPWSTR *szArgList=nullptr;
    int argCount=0;

    szArgList = CommandLineToArgvW(GetCommandLine(),&argCount);
    if (argCount >=2 )
    {
        int iLen = 2*wcslen(szArgList[1]);    
        char* chRtn = new char[iLen+1];    
        wcstombs(chRtn,szArgList[1],iLen+1);
        delete [] chRtn;
    }
    LocalFree(szArgList);

    ProjectConfig project;
    HWND hwndConsole;

	project.setDebuggerType(kCCLuaDebuggerNone);
    // load project config from command line args
    vector<string> args;
    for (int i = 0; i < __argc; ++i)
    {
        wstring ws(__wargv[i]);
        string s;
        s.assign(ws.begin(), ws.end());
        args.push_back(s);
    }
    project.parseCommandLine(args);

	// set environments for debug mode
	if (project.getProjectDir().empty()) {
		string projectDir = "../../..";
		project.setProjectDir(projectDir);
		FileUtils::getInstance()->addSearchPath(projectDir);
		FileUtils::getInstance()->addSearchPath(projectDir + "/res");
	}

#ifndef USE_WIN32_CONSOLE
    project.setShowConsole(false);
#endif

    // create console window
    if (project.isShowConsole())
    {
        AllocConsole();
        hwndConsole = GetConsoleWindow();
        if (hwndConsole != NULL)
        {
            ShowWindow(hwndConsole, SW_SHOW);
            BringWindowToTop(hwndConsole);
            freopen("CONOUT$", "wt", stdout);
            freopen("CONOUT$", "wt", stderr);

            HMENU hmenu = GetSystemMenu(hwndConsole, FALSE);
            if (hmenu != NULL) DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
        }
    }
    project.dump();

    // create the application instance
    AppDelegate app;
    int ret = Application::getInstance()->run();

#ifdef USE_WIN32_CONSOLE
    if (!ret)
    {
        system("pause");
    }
    FreeConsole();
#endif

    return ret;
}
std::string getCurAppPath(void)
{
    TCHAR szAppDir[MAX_PATH]={0};
    if (!GetModuleFileName(NULL,szAppDir,MAX_PATH))
        return "";
    int nEnd=0;
    for (int i=0;szAppDir[i];i++)
    {
        if(szAppDir[i]=='\\')
            nEnd = i;
    }
    szAppDir[nEnd] = 0;
    int iLen = 2*wcslen(szAppDir);
    char* chRtn = new char[iLen+1];
    wcstombs(chRtn,szAppDir,iLen+1);
    std::string strPath = chRtn;
    delete [] chRtn;
    chRtn=NULL;
    char fuldir[MAX_PATH]={0};
    _fullpath(fuldir,strPath.c_str(),MAX_PATH);
    return fuldir;
}
