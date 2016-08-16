/*
Copyright (C) 2011-2014, Comine.com ( profdevi@ymail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


//v1.4 copyright Comine.com 20160816T1743
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MBinFileType.h"
#include "MFileOps.h"
#include "MDirTreeOps.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MFileType";	// Used in Help
static const char *GApplicationVersion="1.3";		// Used in Help

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
				"   usage:  %s [-r|-l] <file/dir>+ [-?]\n"
				"           v%s copyright Comine.com\n"
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

	MStdPrintf("%s\n",filetype.GetFileInfo() );

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

