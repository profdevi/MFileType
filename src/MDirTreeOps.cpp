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


//v1.17 copyright Comine.com 20160214U2018
#include "MStdLib.h"
#include "MBuffer.h"
#include "MDirReader.h"
#include "MStringQueue.h"
#include "MListIntStack.h"
#include "MStringStack.h"
#include "MFileOps.h"
#include "MDirOps.h"
#include "MPathNameList.h"
#include "MDirTreeOps.h"


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxDirectorySize=2048;

/////////////////////////////////////////////////////////
static bool GIsSlashTerminated(const char *directory)
	{
	if(directory==NULL || *directory==0) { return false; }
	int length=MStdStrLen(directory);
	char lastchar=directory[length-1];
	if(lastchar=='/' || lastchar=='\\') { return true; }
	return false;
	}


////////////////////////////////////////////////
static bool GDirAppend(MBuffer &dir,const char *append)
	{
	if(dir==NULL) { return false; }
	if(GIsSlashTerminated(dir)==true)
		{
		if(*append=='/' || *append=='\\')
			{ dir.StringAppend(append+1); }
		else
			{  dir.StringAppend(append);  }
		}
	else
		{
		if(*append=='/' || *append=='\\')
			{ dir.StringAppend(append); }
		else
			{
			dir.StringAppend("\\");
			dir.StringAppend(append);
			}
		}

	return true;
	}


/////////////////////////////////////////////////
static bool GConvertBackSlashes(char *str)
	{
	char *p;
	for(p=str;*p!=0;++p)
		{
		if(*p=='\\') { *p='/'; }
		}

	return true;
	}


//******************************************************
//**  MDirTreeOps class
//******************************************************
void MDirTreeOps::ClearObject(void)
	{
	}


/////////////////////////////////////////////////////////
MDirTreeOps::MDirTreeOps(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MDirTreeOps::~MDirTreeOps(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MDirTreeOps::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MDirTreeOps::Destroy(void)
	{
	ClearObject();
	return true;
	}


/////////////////////////////////////////////////
bool MDirTreeOps::GetTreeFiles(const char *directory,MStringQueue &queue
			,bool (*CallBack)(const char *filename,void *passedargument)
			,void *passedargument)
	{
	MDirOps dirops(true);

	//init queue object
	if(queue.Create()==false)
		{
		return false;
		}

	MStringQueue openlist;
	if(openlist.Create()==false)
		{
		return false;
		}

	// Get FileOps
	MFileOps fileops(true);
	MString absdirpath;
	if(fileops.GetAbsolutePath(directory,absdirpath)==false)
		{
		return false;
		}

	bool ret=openlist.Enqueue(absdirpath.Get() );
	if(ret==false)
		{
		return false;
		}

	MBuffer tmpstring(GMaxDirectorySize); 
	MDirReader dirreader;
	while(openlist.GetQueueLength()>0)
		{
		// Calc Search path
		ret=tmpstring.SetString(openlist.GetFront());
		if(ret==false)
			{
			return false;
			}

		if(dirops.IsReadable(openlist.GetFront())==false)
			{
			openlist.Dequeue();
			continue;
			}

		GDirAppend(tmpstring,"*.*");

		if(dirreader.Create(tmpstring)==false)
			{
			openlist.Dequeue();  continue;
			}

		while(dirreader.Read()==true)
			{
			// Skip . and ..
			if(MStdStrCmp(dirreader.GetFileName(),".")==0
					|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
				{ continue; }

			tmpstring.SetString(openlist.GetFront());
			GDirAppend(tmpstring,dirreader.GetFileName() );

			if(dirreader.IsDirectory()==true)
				{	openlist.Enqueue(tmpstring);  continue;  }

			//=Process a file
			GConvertBackSlashes(tmpstring.GetBuffer() );
			queue.Enqueue(tmpstring);

			// Handle Callback
			if(CallBack!=NULL && CallBack(tmpstring,passedargument)==false)
				{  return true;  }
			}

		openlist.Dequeue();
		}
	
	return true;
	}


/////////////////////////////////////////////////////////////////
bool MDirTreeOps::GetTreeFilesParentRelative(const char *directory,MStringQueue &queue
		,bool (*CallBack)(const char *filename,void *passedargument)
		,void *passedargument)
	{
	//init queue object
	if(queue.Create()==false)
		{
		return false;
		}

	MStringQueue openlist;
	if(openlist.Create()==false)
		{
		return false;
		}

	bool ret=openlist.Enqueue("/");
	MDirOps dirops(true);

	MBuffer tmpstring(GMaxDirectorySize);  MDirReader dirreader;
	while(openlist.GetQueueLength()>0)
		{
		// Construct search path
		tmpstring.SetString(directory);
		GDirAppend(tmpstring,openlist.GetFront());
		
		if(dirops.IsReadable(openlist.GetFront())==false)
			{
			openlist.Dequeue();
			continue;
			}

		GDirAppend(tmpstring,"*.*");		// Search path

		if(dirreader.Create(tmpstring)==false)
			{
			openlist.Dequeue();  continue;
			}

		while(dirreader.Read()==true)
			{
			// Skip . and ..
			if(MStdStrCmp(dirreader.GetFileName(),".")==0
					|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
				{ continue; }

			// Construct Real FileName
			tmpstring.SetString(openlist.GetFront());
			GDirAppend(tmpstring,dirreader.GetFileName());

			if(dirreader.IsDirectory()==true)
				{	openlist.Enqueue(tmpstring);  continue;  }

			//=Process a file
			GConvertBackSlashes(tmpstring.GetBuffer() );
			queue.Enqueue(tmpstring);

			// Handle Callback
			if(CallBack!=NULL && CallBack(tmpstring,passedargument)==false)
				{  return true;  }
			}

		openlist.Dequeue();
		}
	
	return true;
	}


////////////////////////////////////////////////////////////////
bool MDirTreeOps::GetTreeDirs(const char *directory,MStringQueue &queue
		,bool (*CallBack)(const char *filename,void *passedargument)
		,void *passedargument)
	{
	MDirOps dirops(true);

	//init queue object
	if(queue.Create()==false)
		{
		return false;
		}

	MStringQueue openlist;
	if(openlist.Create()==false)
		{
		return false;
		}

	bool ret=openlist.Enqueue(directory);

	MBuffer tmpstring(GMaxDirectorySize);  MDirReader dirreader;
	while(openlist.GetQueueLength()>0)
		{
		// Calc Search path
		ret=tmpstring.SetString(openlist.GetFront());
		if(dirops.IsReadable(openlist.GetFront())==false)
			{
			openlist.Dequeue();
			continue;
			}

		GDirAppend(tmpstring,"*.*");

		if(dirreader.Create(tmpstring)==false)
			{
			openlist.Dequeue();  continue;
			}

		while(dirreader.Read()==true)
			{
			// Skip . and ..
			if(MStdStrCmp(dirreader.GetFileName(),".")==0
					|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
				{ continue; }

			tmpstring.SetString(openlist.GetFront());

			GDirAppend(tmpstring,dirreader.GetFileName() );

			if(dirreader.IsDirectory()==true)
				{
				openlist.Enqueue(tmpstring);

				GConvertBackSlashes(tmpstring.GetBuffer() );
				queue.Enqueue(tmpstring);

				// Handle Callback
				if(CallBack!=NULL && CallBack(tmpstring,passedargument)==false)
					{  return true;  }

				continue;
				}

			}

		openlist.Dequeue();
		}

	return true;
	}


//////////////////////////////////////////////////////////////////
bool MDirTreeOps::GetTreeDirsParentRelative(const char *directory,MStringQueue &queue
		,bool (*CallBack)(const char *filename,void *passedargument)
		,void *passedargument)
	{
	//init queue object
	if(queue.Create()==false)
		{
		return false;
		}

	MStringQueue openlist;
	if(openlist.Create()==false)
		{
		return false;
		}

	bool ret=openlist.Enqueue("/");

	MDirOps dirops(true);
	MBuffer tmpstring(GMaxDirectorySize);  MDirReader dirreader;
	while(openlist.GetQueueLength()>0)
		{
		// Construct search path
		if(dirops.IsReadable(openlist.GetFront())==false)
			{
			openlist.Dequeue();
			continue;	
			}

		tmpstring.SetString(directory);
		GDirAppend(tmpstring,openlist.GetFront());

		GDirAppend(tmpstring,"*.*");		// Search path

		if(dirreader.Create(tmpstring)==false)
			{
			openlist.Dequeue();  continue;
			}

		while(dirreader.Read()==true)
			{
			// Skip . and ..
			if(MStdStrCmp(dirreader.GetFileName(),".")==0
					|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
				{ continue; }

			// Construct Real FileName
			tmpstring.SetString(openlist.GetFront());
			GDirAppend(tmpstring,dirreader.GetFileName());

			if(dirreader.IsDirectory()==true)
				{
				openlist.Enqueue(tmpstring);

				GConvertBackSlashes(tmpstring.GetBuffer() );
				queue.Enqueue(tmpstring);

				// Handle Callback
				if(CallBack!=NULL && CallBack(tmpstring,passedargument)==false)
					{  return true;  }

				continue;
				}
			}

		openlist.Dequeue();
		}
	
	return true;	
	}


//////////////////////////////////////////////////////////////
bool MDirTreeOps::DelTree(const char *directory
			,bool (*CallBack)(const char *filename,void *passedargument)
			,void *passedargument) // Removes a directory recursively
	{
	// Find All tree files
	MStringQueue treefiles;
	if(GetTreeFiles(directory,treefiles)==false)
		{
		return false;
		}

	// Init MFileOps
	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	// Delete all tree files
	while(treefiles.GetQueueLength()>0)
		{
		if(treefiles.GetFront()==NULL)
			{
			return false;
			}

		// Handle Callback for file deletion
		if(CallBack!=NULL && CallBack(treefiles.GetFront(),passedargument)==false)
			{  return true;  }

		// set file attribute readonly to false
		if(fileops.SetReadOnly(treefiles.GetFront(),false)==false)
			{
			return false;
			}

		// set file attribute system to false
		if(fileops.SetSystemFile(treefiles.GetFront(),false)==false)
			{
			return false;
			}

		// set file attribute hidden to false
		if(fileops.SetHiddenFile(treefiles.GetFront(),false)==false)
			{
			return false;
			}
		
		if(MStdFileRemove(treefiles.GetFront())==false)
			{
			return false;
			}

		if(treefiles.Dequeue()==false)
			{
			return false;
			}
		}
	
	// Get Depth First Directories and start deleteing
	MStringStack dirstack;
	if(dirstack.Create()==false)
		{
		return false;
		}

	MListIntStack intstack;
	if(intstack.Create()==false)
		{
		return false;
		}
	
	// Push First Element on to stack
	if(dirstack.Push(directory)==false)
		{
		return false;
		}

	// Push associated color
	if(intstack.Push(0)==false)
		{
		return false;
		}

	// Go through main loop
	while(dirstack.IsEmpty()==false) // While not empty
		{
		// Get Color for first item
		int color=intstack.GetTop(); 

		
		if(color==0)
			{
			//=First time the directory has been seen

			// Push back the directory as seen(1)
			intstack.Pop();
			intstack.Push(1);	// Have Seen the directory

			// Find All the children and place into stack for DFS
			MBuffer currentdir(GMaxDirectorySize);

			// Save Current dir in temp variable
			currentdir.SetString(dirstack.GetTop());

			// Create search path
			MBuffer searchpath(GMaxDirectorySize);
			searchpath.SetString(dirstack.GetTop());
			GDirAppend(searchpath,"*.*");

			MDirReader dirreader;
			if(dirreader.Create(searchpath)==false)
				{
				return false;
				}


			while(dirreader.Read()==true)
				{
				// Skip . and ..
				if(MStdStrCmp(dirreader.GetFileName(),".")==0
						|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
					{ continue; }

				// Build up a directory path
				MBuffer tmpstring(GMaxDirectorySize);
				tmpstring.SetString(currentdir.GetBuffer());

				GDirAppend(tmpstring,dirreader.GetFileName());

				// Push new directory into dir stack
				dirstack.Push(tmpstring.GetBuffer());

				// Mark color as unseen
				intstack.Push(0);
				}
			
			continue; // Done Processing all the files
			}

		//=The color must be one
		if(color!=1)
			{
			}

		// Check if user wants to stop
		if(CallBack!=NULL && CallBack(dirstack.GetTop(),passedargument)==false)
			{ return true; }

		// Remove the directory
		if(::RemoveDirectoryA(dirstack.GetTop())==FALSE)
			{
			return false;
			}

		// Pop Item of both stacks
		dirstack.Pop();  intstack.Pop();
		}
	
	return true;
	}


//////////////////////////////////////////////////////////////
bool MDirTreeOps::DelSubTree(const char *directory
			,bool (*CallBack)(const char *filename,void *passedargument)
			,void *passedargument) // Removes a directory recursively
	{
	// Find All tree files
	MStringQueue treefiles;
	if(GetTreeFiles(directory,treefiles)==false)
		{
		return false;
		}

	// Init MFileOps
	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	// Delete all tree files
	while(treefiles.GetQueueLength()>0)
		{
		if(treefiles.GetFront()==NULL)
			{
			return false;
			}

		// Handle Callback for file deletion
		if(CallBack!=NULL && CallBack(treefiles.GetFront(),passedargument)==false)
			{  return true;  }

		// set file attribute readonly to false
		if(fileops.SetReadOnly(treefiles.GetFront(),false)==false)
			{
			return false;
			}

		// set file attribute system to false
		if(fileops.SetSystemFile(treefiles.GetFront(),false)==false)
			{
			return false;
			}

		// set file attribute hidden to false
		if(fileops.SetHiddenFile(treefiles.GetFront(),false)==false)
			{
			return false;
			}
		
		const char *delfilename=treefiles.GetFront();
		if(MStdFileRemove(delfilename)==false)
			{
			// return false;
			}

		if(treefiles.Dequeue()==false)
			{
			return false;
			}
		}
	
	// Get Depth First Directories and start deleteing
	MStringStack dirstack;
	if(dirstack.Create()==false)
		{
		return false;
		}

	MListIntStack intstack;
	if(intstack.Create()==false)
		{
		return false;
		}

	//Init MFileOps to find abspath of directory
	MString absdirectory;
	if(fileops.GetAbsolutePath(directory,absdirectory)==false)
		{
		return false;
		}
	
	// Push First Element on to stack
	if(dirstack.Push(absdirectory.Get() )==false)
		{
		return false;
		}

	// Push associated color
	if(intstack.Push(0)==false)
		{
		return false;
		}

	// Go through main loop
	while(dirstack.IsEmpty()==false) // While not empty
		{
		// Get Color for first item
		int color=intstack.GetTop(); 

		
		if(color==0)
			{
			//=First time the directory has been seen

			// Push back the directory as seen(1)
			intstack.Pop();
			intstack.Push(1);	// Have Seen the directory

			// Check if current entry is a real directory
			if(fileops.IsDirectory(dirstack.GetTop() )==false)
				{  continue; }

			// Find All the children and place into stack for DFS
			MBuffer currentdir(GMaxDirectorySize);

			// Save Current dir in temp variable
			currentdir.SetString(dirstack.GetTop());
			

			// Create search path
			MBuffer searchpath(GMaxDirectorySize);
			searchpath.SetString(dirstack.GetTop());
			GDirAppend(searchpath,"*.*");

			MDirReader dirreader;
			if(dirreader.Create(searchpath)==false)
				{
				dirstack.Pop();
				intstack.Pop();
				continue;
				}


			while(dirreader.Read()==true)
				{
				// Skip . and ..
				if(MStdStrCmp(dirreader.GetFileName(),".")==0
						|| MStdStrCmp(dirreader.GetFileName(),"..")==0 )
					{ continue; }

				// Build up a directory path
				MBuffer tmpstring(GMaxDirectorySize);
				tmpstring.SetString(currentdir.GetBuffer());

				GDirAppend(tmpstring,dirreader.GetFileName());

				// Push new directory into dir stack
				dirstack.Push(tmpstring.GetBuffer());

				// Mark color as unseen
				intstack.Push(0);
				}
			
			continue; // Done Processing all the files
			}

		//=The color must be one
		if(color!=1)
			{
			}

		// Check if user wants to stop
		if(CallBack!=NULL && CallBack(dirstack.GetTop(),passedargument)==false)
			{ return true; }

		// Check if directory is the root parent
		if(absdirectory.Compare(dirstack.GetTop())==0)
			{
			dirstack.Pop();
			intstack.Pop();
			continue;
			}

		// Remove the directory
		if(::RemoveDirectoryA(dirstack.GetTop())==FALSE)
			{
			// Continue anyway
			// return false;
			}

		// Pop Item of both stacks
		dirstack.Pop();
		intstack.Pop();
		}
	
	return true;
	}


///////////////////////////////////////////////////////
bool MDirTreeOps::FindFiles(const char *directory,const char *pattern
		,MStringQueue &queue,bool (*CallBack)(const char *filename
		,void *passedargument),void *argument)
	{
	MFileOps fileops(true);
	MString absdirpath;
	if(fileops.GetAbsolutePath(directory,absdirpath)==false)
		{
		return false;
		}

	//**Find First all subdirectories
	MStringQueue dirlist;
	if(GetTreeDirs(absdirpath.Get(),dirlist,CallBack,argument)==false)
		{
		return false;
		}

	// Add the directory
	if(dirlist.Enqueue(absdirpath.Get() )==false)
		{
		return false;
		}

	// Now Process every file in directory
	MDirOps dirops(true);

	// Init output
	if(queue.Create()==false)
		{
		return false;
		}
	
	while(dirlist.GetQueueLength()>0)
		{
		MPathNameList pathnamelist;	
		if(pathnamelist.Create(dirlist.GetFront())==false)
			{
			return false;
			}

		// Skip non readable directories
		if(dirops.IsReadable(dirlist.GetFront())==false)
			{
			dirlist.Dequeue();
			continue;
			}

		if(pathnamelist.Push(pattern)==false)
			{
			return false;
			}

		// Get Full Path of files
		MBuffer fullsearchpath;
		if(pathnamelist.GetFullPath(fullsearchpath)==false)
			{
			return false;
			}

		MDirReader reader;
		if(reader.Create(fullsearchpath.GetBuffer())==false)
			{
			dirlist.Dequeue();
			continue;
			}

		while(reader.Read()==true)
			{
			MBuffer tmpstring(GMaxDirectorySize);
			tmpstring.SetString(reader.GetFullFilePath() );
			GConvertBackSlashes(tmpstring.GetBuffer() );
			if(queue.Enqueue(tmpstring)==false)
				{
				return false;
				}
			}

		dirlist.Dequeue();
		}

	return true;
	}




