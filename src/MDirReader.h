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

//v1.13 copyright Comine.com 20160214U2024
#ifndef MDirReader_h
#define MDirReader_h

////////////////////////////////////////////////////
#include <windows.h>
#include "MPathNameList.h"
#include "MBuffer.h"

//******************************************************
//**  MDirReader class
//******************************************************
class MDirReader
	{
	////////////////////////////////////////////////
	HANDLE mDirHandle;
	WIN32_FIND_DATAA *mData;
	bool mDataRead;
	MPathNameList mPathList;
	MBuffer mFullFilePath;
	bool mNoFiles;						//=true if no more files

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MDirReader(void);
	~MDirReader(void);
	bool Create(const char *dirpath);
	bool Destroy(void);
	bool Read(void);	// Returns True for each new entry

	// Directory Info Functions
	const char *GetFileName(void);
	const char *GetDosFileName(void);
	const char *GetFullFilePath(void);
	bool IsArchived(void);
	bool IsCompressed(void);
	bool IsDirectory(void);
	bool IsEncrypted(void);
	bool IsHidden(void);
	bool IsNormal(void);
	bool IsReadOnly(void);
	bool IsSystemFile(void);
	bool IsTempFile(void);
	DWORD GetFileSizeLow(void);
	DWORD GetFileSizeHigh(void);
	bool GetCreateTime(FILETIME &time);
	bool GetLastAccessTime(FILETIME &time);
	bool GetLastWriteTime(FILETIME &time);
	};

#endif // MDirReader_h

