/*    
    MFileType.exe : Shows the type of a file
    Copyright (C) 2017  Comine.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//v1.5 copyright Comine.com 20170310F0658
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MBinFileType.h"
#include "MFileOps.h"
#include "MDirTreeOps.h"
#include "MLicenseGPL.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MFileType";	// Used in Help
static const char *GApplicationVersion="1.5";		// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);
static bool GPrintFileType(const char *filename);
static bool GProcessingFile(const char *filename,void *);
static void GPrintFileTypes(void);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	///////////////////////////////////////////////
	if(args.GetArgCount()<2)
		{
		GDisplayHelp();
		return 0;
		}

	if(args.CheckRemoveArg("-gpl")==true)
		{
		MLicenseGPL gpl(true);
		gpl.Print();
		}
	
	if(args.CheckRemoveArg("-l")==true)
		{
		GPrintFileTypes();
		return 0;
		}

	bool recursive=false;
	if(args.CheckRemoveArg("-r")==true)
		{
		recursive=true;
		}

	MFileOps fileops;
	if(fileops.Create()==false)
		{
		MStdPrintf("**System Failure\n");
		return 1;
		}

	MDirTreeOps dirops(true);
	
	int i;
	for(i=1;i<args.GetArgCount();++i)
		{
		const char *filename=args.GetArg(i);
		
		if(fileops.Exists(filename)==true)
			{
			GPrintFileType(filename);	
			}
		else if(fileops.IsDirectory(filename)==true)
			{
			if(recursive==false)
				{
				MStdPrintf("%-40s : <Dir>\n",filename);
				continue;
				}

			MStringQueue queue;
			if(dirops.GetTreeFiles(filename,queue,GProcessingFile)==false)
				{
				MStdPrintf(" ** %s: Failed getting dir files\n",filename);
				continue;
				}

			while(queue.GetQueueLength()>0)
				{
				GPrintFileType(queue.GetFront());
				queue.Dequeue();
				}

			continue;
			}
		else
			{
			MStdPrintf(" ** Unable to process file %s\n",filename);
			}
		}

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-r|-l|-gpl] <file/dir>+ [-?]\n"
				"           v%s copyright Comine.com\n"
				"           The software is release under the Gnu Public License 3.0\n"
				"           Passing the -gpl command line argument will display the license\n"
				"\n"
				"      Displays Information about the file type.\n"
				"\n"
				"   options:\n"
				"          -r : recursively go through all the directories\n"
				"          -l : list recognized file types\n"
				"\n"
				,GApplicationName,GApplicationVersion);
	}


////////////////////////////////////////////////////
static bool GPrintFileType(const char *filename)
	{
	MFileOps fileops(true);
	MStdPrintf("%-40s : ",filename);

	MBinFileType filetype;
	
	if(filetype.Create(filename)==false)
		{
		MStdPrintf("**OPEN FAILED**\n");
		return false;
		}

	if(filetype.GetFileType()==MBFTE_UNKNOWN)
		{
		if(fileops.IsDosTextFile(filename)==true)
			{
			MStdPrintf("DOS Text File\n");
			return true;
			}
		else if(fileops.IsUnixTextFile(filename)==true)
			{
			MStdPrintf("Unix Text File\n");
			return true;
			}
		else
			{
			MStdPrintf("Unknown Binary\n");
			return true;
			}
		
		filetype.Destroy();
		return true;
		}

	const char *fileinfo=filetype.GetFileInfo();
	const char *extrainfo=filetype.GetExtraInfo();
	if(extrainfo==0)
		{
		MStdPrintf("%s\n",fileinfo );
		}
	else
		{
		MStdPrintf("%s(%s)\n",fileinfo,extrainfo);
		}

	filetype.Destroy();
	
	return true;
	}


///////////////////////////////////////////
static bool GProcessingFile(const char *filename,void *)
	{
	// MStdPrintf("%79s\r%s\r","",filename);
	return true;
	}


///////////////////////////////////////////
static void GPrintFileTypes(void)
	{
	MBinFileTypeInfo fileinfo(true);
	const int count=fileinfo.GetCount();

	MStdPrintf("%-40s%-10s%-29s\n"," Discription"," Ext"," Signature");
	MStdPrintf("%-40s%-10s%-29s\n"," -----------"," ---"," ---------");
	int i;
	for(i=0;i<count;++i)
		{
		const char *info=fileinfo.GetFileInfo(i);
		const char *ext=fileinfo.GetFileExtension(i);
		const char *signature=fileinfo.GetSignature(i);
		
		MStdPrintf("%-40s%-10s%-29s\n",info,ext,signature);
		}
	}

