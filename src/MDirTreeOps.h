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
#ifndef MDirTreeOps_h
#define MDirTreeOps_h

////////////////////////////////////////////////////
#include "MStringQueue.h"

//******************************************************
//**  MDirTreeOps class
//******************************************************
class MDirTreeOps
	{
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MDirTreeOps(bool create=false);
	~MDirTreeOps(void);
	bool Create(void);
	bool Destroy(void);
	bool GetTreeFiles(const char *directory,MStringQueue &queue
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	// Find all files in tree
	bool GetTreeFilesParentRelative(const char *directory,MStringQueue &queue
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	// Find all files in tree
	bool GetTreeDirs(const char *directory,MStringQueue &queue
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	// Find all directories
	bool GetTreeDirsParentRelative(const char *directory,MStringQueue &queue
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	//  Find Dirs parent relative
	bool DelTree(const char *directory
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	// Removes a directory recursively
	bool DelSubTree(const char *directory
			,bool (*CallBack)(const char *filename,void *passedargument)=NULL
			,void *passedargument=NULL);	// Removes a  SubTree (not Parent)
	bool FindFiles(const char *directory,const char *pattern
			,MStringQueue &queue,bool (*CallBack)(const char *filename
			,void *passedargument)=NULL,void *passedargument=NULL);
	};


#endif // MDirTreeOps_h

