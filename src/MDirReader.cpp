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
#include <windows.h>
#include "MStdLib.h"
#include "MBuffer.h"
#include "MFileOps.h"
#include "MPathNameList.h"
#include "MDirReader.h"


//******************************************************
//**  Module Elements
//******************************************************
static bool GReducePath(const char *passedarg,MBuffer &outreducedpath)
	{
	if(outreducedpath.Create(1000)==false)
		{
		return false;
		}

	outreducedpath.SetString(passedarg);
	int length=outreducedpath.GetStringLength();
	int i;
	for(i=length-1;i>=0;--i)
		{
		if(outreducedpath[i]=='/' || outreducedpath[i]=='\\')
			{
			outreducedpath[i]=0;
			return true;
			}
		}

	outreducedpath[0]=0;
	return true;
	}

//******************************************************
//**  MDirReader class
//******************************************************
void MDirReader::ClearObject(void)
	{
	mDirHandle=INVALID_HANDLE_VALUE;
	mData=NULL;
	mDataRead=false;
	mNoFiles=false;
	}


/////////////////////////////////////////////////
MDirReader::MDirReader(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MDirReader::~MDirReader(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MDirReader::Create(const char *dirpath)
	{
	Destroy();
	mData=new WIN32_FIND_DATAA;
	if(mData==NULL)
		{
		Destroy();
		return false;
		}

	MStdMemSet(mData,0,sizeof(*mData) );

	mDirHandle=FindFirstFileA(dirpath,mData);
	if(mDirHandle==INVALID_HANDLE_VALUE)
		{
		DWORD error=GetLastError();
		if(error==2)
			{
			//=We have no file that pattern match
			mNoFiles=true;

			// Init Dummy Vars
			if(mFullFilePath.Create(10)==false)
				{
				Destroy();
				return false;
				}

			if(mFullFilePath.SetString("")==false)
				{
				Destroy();
				return false;
				}


			return true;
			}

		Destroy();
		return false;
		}

	mDataRead=false;

	// Check if full path
	MFileOps fileops(true);
	MString fullfilepath;
	if(fileops.Exists(dirpath)==true)
		{
		if(fileops.IsDirectory(dirpath)==true)
			{
			if(fileops.GetAbsolutePath(dirpath,fullfilepath)==false)
				{
				Destroy();
				return false;
				}

			if(mPathList.Create(fullfilepath.Get())==false)
				{
				Destroy();
				return false;
				}

			if(mPathList.Push("__DUMMYFILE__")==false)
				{
				Destroy();
				return false;
				}
			}
		else
			{
			if(fileops.GetAbsolutePath(dirpath,fullfilepath)==false)
				{
				Destroy();
				return false;
				}

			if(mPathList.Create(fullfilepath.Get() )==false)
				{
				Destroy();
				return false;
				}
			}
		}
	else
		{
		MBuffer reducedpath;
		if(GReducePath(dirpath,reducedpath)==false)
			{
			Destroy();
			return false;
			}

		if(reducedpath[0]==0)
			{
			if(reducedpath.Create(10)==false)
				{
				Destroy();
				return false;
				}

			if(reducedpath.SetString(".")==false)
				{
				Destroy();
				return false;
				}
			}
		
		if(fileops.GetAbsolutePath(reducedpath,fullfilepath)==false)
			{
			Destroy();
			return false;
			}
		
		if(mPathList.Create(fullfilepath.Get() )==false)
			{
			Destroy();
			return false;
			}

		if(mPathList.Push("__DUMMYFILE__")==false)
			{
			Destroy();
			return false;
			}
		}

	if(mFullFilePath.Create(10)==false)
		{
		Destroy();
		return false;
		}

	if(mFullFilePath.SetString("")==false)
		{
		Destroy();
		return false;
		}
	return true;
	}


////////////////////////////////////////////////
bool MDirReader::Destroy(void)
	{
	mPathList.Destroy();
	mFullFilePath.Destroy();
	
	if(mData!=NULL)
		{
		delete mData;
		mData=NULL;
		}

	if(mDirHandle!=INVALID_HANDLE_VALUE)
		{
		FindClose(mDirHandle);
		mDirHandle=INVALID_HANDLE_VALUE;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MDirReader::Read(void)
	{
	if(mNoFiles==true)
		{  return false; }

	// First Item is now being read
	if(mDataRead==false)
		{
		mDataRead=true;

		// Place the current File into FullPath
		mPathList.Pop();
		mPathList.Push(GetFileName());
		mPathList.GetFullPath(mFullFilePath);

		return true;
		}

	// Read the next item
	if(FindNextFileA(mDirHandle,mData)==FALSE)
		{
		mFullFilePath.Create(10);
		mFullFilePath.SetString("");
		mNoFiles=true;
		return false;
		}

	// Place the current File into FullPath
	if(mPathList.GetElementCount()>0)
		{  mPathList.Pop();  }

	mPathList.Push(GetFileName());
	mPathList.GetFullPath(mFullFilePath);

	return true;
	}


/////////////////////////////////////////////////
const char *MDirReader::GetFileName(void)
	{
	return mData->cFileName;
	}


/////////////////////////////////////////////////
const char *MDirReader::GetDosFileName(void)
	{
	return mData->cAlternateFileName;
	}


///////////////////////////////////////////////
const char *MDirReader::GetFullFilePath(void)
	{
	return mFullFilePath.GetBuffer();
	}


/////////////////////////////////////////////////
bool MDirReader::IsArchived(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) { return true; }
	return false;
	}


/////////////////////////////////////////////////
bool MDirReader::IsCompressed(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) { return true; }
	return false;
	}


/////////////////////////////////////////////////
bool MDirReader::IsDirectory(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { return true; }
	return false;
	}


/////////////////////////////////////////////////
bool MDirReader::IsEncrypted(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) { return true; }
	return false;
	}


/////////////////////////////////////////////////
bool MDirReader::IsHidden(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) { return true; }
	return false;
	}



/////////////////////////////////////////////////
bool MDirReader::IsNormal(void)
	{
	if(mData->dwFileAttributes==0) { return true; }
	return false;
	}



/////////////////////////////////////////////////
bool MDirReader::IsReadOnly(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_READONLY) { return true; }
	return false;
	}


/////////////////////////////////////////////////
bool MDirReader::IsSystemFile(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) { return true; }
	return false;
	}



/////////////////////////////////////////////////
bool MDirReader::IsTempFile(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) { return true; }
	return false;
	}


/////////////////////////////////////////////////
DWORD MDirReader::GetFileSizeLow(void)
	{
	return mData->nFileSizeLow;
	}


/////////////////////////////////////////////////
DWORD MDirReader::GetFileSizeHigh(void)
	{
	return mData->nFileSizeHigh;
	}


////////////////////////////////////////////////
bool MDirReader::GetCreateTime(FILETIME &time)
	{
	time=mData->ftCreationTime;  return true;
	}


////////////////////////////////////////////////
bool MDirReader::GetLastAccessTime(FILETIME &time)
	{
	time=mData->ftLastAccessTime;  return true;
	}



////////////////////////////////////////////////
bool MDirReader::GetLastWriteTime(FILETIME &time)
	{
	time=mData->ftLastWriteTime;  return true;
	}




