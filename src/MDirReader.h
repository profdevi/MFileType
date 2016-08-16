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

