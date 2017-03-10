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

