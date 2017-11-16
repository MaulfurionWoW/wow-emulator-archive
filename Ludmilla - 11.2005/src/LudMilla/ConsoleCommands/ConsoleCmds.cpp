//******************************************************************************
#include "StdAfx.h"
//==============================================================================
							// Public methods:
//------------------------------------------------------------------------------
// Get pending input and process it
void CConsoleCmds::Do()
{
	char c = 0;
nextcmd:
	// Read in single line from "stdin":
	for(c=0; (pos<80) && (c!=13 && c!=10); pos++)
	{
		if ( (c=AConsole.GetChar()) != EOF )
		{
			putchar(c);
			cmd[pos] = (char)c;
		}
		else
			return;
	}

	if (c==13 || c==10)
	{
		if (c != '\n')
			sLog.outString("");

		cmd[pos-1] = 0;
		ProcessCmd(cmd);
		cmd[0] = 0;
		pos = 0;

		sLog.outString("");

		goto nextcmd;
	}

	if (pos == 80)
	{
		sLog.outError("console command line overflow");
		cmd[0] = 0;
		pos = 0;
	}
}
//------------------------------------------------------------------------------
// Constructor
CConsoleCmds::CConsoleCmds()
{
	cmd = new char[80];
	if (cmd == NULL)
		sLog.outError("new() returns NULL");
	cmd[0] = 0;
	pos = 0;
}
//------------------------------------------------------------------------------
// Simple destructor
CConsoleCmds::~CConsoleCmds()
{
	if (cmd != NULL)
		delete[] cmd;
}
//------------------------------------------------------------------------------
							// Protected methods:
//------------------------------------------------------------------------------
// Process one command
void CConsoleCmds::ProcessCmd(char *cmd)
{
	typedef void (*PTranslater)(char *str);
	struct SCmd
	{
		char *name;
		PTranslater tr;
	};
	
	SCmd cmds[] =
	{
		{"version", CConsoleCmds::TranslateVersion}, {"ver" , CConsoleCmds::TranslateVersion},
		{"quit"   , CConsoleCmds::TranslateQuit}   , {"exit", CConsoleCmds::TranslateQuit},
		{"help"   , CConsoleCmds::TranslateHelp}   , {"?"   , CConsoleCmds::TranslateHelp},
	};

	for (int i = 0; i < sizeof(cmds)/sizeof(SCmd); i++)
		if (stricmp(cmd, cmds[i].name) == 0)
		{
			(*(cmds[i].tr)) (cmd + strlen(cmds[i].name));
			return;
		}

	sLog.outError("Unknown console command (use \"help\" for help).");
}
//------------------------------------------------------------------------------
// ver[sion]
void CConsoleCmds::TranslateVersion(char *str)
{
	ProcessVersion();
}
void CConsoleCmds::ProcessVersion()
{
	sLog.outString(FULLSERVERNAME);
}
//------------------------------------------------------------------------------
// quit | exit
void CConsoleCmds::TranslateQuit(char *str)
{
	ProcessQuit();
}
void CConsoleCmds::ProcessQuit()
{
	raise(SIGINT);
}
//------------------------------------------------------------------------------
// help | ?
void CConsoleCmds::TranslateHelp(char *str)
{
	ProcessHelp(NULL);
}
void CConsoleCmds::ProcessHelp(char *command)
{
	if (command == NULL)
	{
		sLog.outString("version, ver: print version");
		sLog.outString("quit, exit: close program");
		sLog.outString("help, ?: print this text");
	}
}
//******************************************************************************