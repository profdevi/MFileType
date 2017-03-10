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

//v1.11 copyright Comine.com 20150818T0511
#include "MStdLib.h"
#include "MStringStack.h"


//******************************************************
//**  MStringStack class
//******************************************************
void MStringStack::ClearObject(void)
	{
	mList=NULL;
	mCount=0;
	}


////////////////////////////////////////////////
MStringStack::MStringStack(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MStringStack::~MStringStack(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MStringStack::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MStringStack::Destroy(void)
	{
	while(mList!=NULL)
		{
		if(mList->Value!=NULL)
			{
			delete[] mList->Value;

			mList->Value=NULL;
			}
		MStringStackNode *tmp=mList;
		mList=mList->Next;
		delete tmp;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MStringStack::Push(const char *str)		// Push an item onto stack
	{
	// Allocate new node
	MStringStackNode *node;
	node = new MStringStackNode;
	if(node==NULL)
		{
		return false;
		}

	// Init node
	node->Next=NULL;
	node->Value=NULL;

	if(str!=NULL)
		{
		const int strlength=MStdStrLen(str)+1;
		node->Value=new char[strlength];
		MStdStrCpy(node->Value,str);
		}

	node->Next=mList;
	mList=node;
	mCount=mCount+1;
	return true;
	}


////////////////////////////////////////////////
bool MStringStack::Pop(void)						// Pop an item off stack
	{
	if(mList==NULL)
		{
		return false;
		}

	MStringStackNode *tmp=mList;
	mList=mList->Next;

	if(tmp->Value!=NULL)
		{
		delete[] tmp->Value;
		tmp->Value=NULL;
		}

	delete tmp;
	mCount=mCount-1;
	return true;
	}


////////////////////////////////////////////////
const char *MStringStack::GetTop(void)			// Get Top Item in Stack
	{
	if(mList==NULL)
		{
		return NULL;
		}

	return mList->Value;
	}


////////////////////////////////////////////////
bool MStringStack::IsEmpty(void)					// Check if stack is empty
	{
	if(mList==NULL) { return true; }
	return false;
	}


////////////////////////////////////////////////
bool MStringStack::IsElement(const char *string)	// Check if element is of set
	{
	for(MStringStackNode *p=mList;p!=NULL;p=p->Next)
		{
		if(p->Value==NULL)
			{
			if(string==NULL) { return true; }
			continue;
			}
		if(string==NULL) { continue; }
		if(MStdStrCmp(p->Value,string)==0) { return true; }
		}

	return false;
	}


/////////////////////////////////////////////////
bool MStringStack::Reverse(void)		// Reverse Stack Elements
	{
	// if list is empty or one element, reverse done.
	if(mList==NULL || mList->Next==NULL) { return true; }

	// List has 2 or more elements

	// Reverse the list
	MStringStackNode *newlist=NULL;
	for(MStringStackNode *oldlist=mList;oldlist!=NULL; )
		{
		MStringStackNode *nextitem=oldlist->Next;
		oldlist->Next=newlist;
		newlist=oldlist;
		oldlist=nextitem;
		}

	mList=newlist;

	return true;
	}


/////////////////////////////////////////////////
int MStringStack::GetCount(void)	// Returns Element count
	{
	return mCount;
	}


////////////////////////////////////////////////////////
int MStringStack::GetElementPos(const char *str)	// Return position relative to top
	{
	int count;  MStringStackNode *p;
	for(count=0,p=mList;p!=NULL;p=p->Next,count=count+1)
		{
		if(str==NULL && p->Value==NULL) { return count; }
		if(str!=NULL && p->Value!=NULL && MStdStrCmp(str,p->Value)==0)
			{  return count; }
		}

	return -1;
	}


////////////////////////////////////////////////////
bool MStringStack::GetReader(MStringStackReader &reader)	// Init Reader
	{
	if(reader.Create(mList)==false)
		{
		return false;
		}

	return true;
	}

//******************************************************
//**  MStringStackReader class
//******************************************************
void MStringStackReader::ClearObject(void)
	{
	mList=NULL;
	}

////////////////////////////////////////////////////////
MStringStackReader::MStringStackReader(void)
	{  ClearObject();  }


////////////////////////////////////////////////////////
MStringStackReader::~MStringStackReader(void)
	{  Destroy();  }


////////////////////////////////////////////////////////
bool MStringStackReader::Create(const MStringStackNode *list)
	{
	Destroy();
	mList=list;
	return true;
	}


////////////////////////////////////////////////////////
bool MStringStackReader::Destroy(void)
	{
	ClearObject();
	return true;
	}


////////////////////////////////////////////////////////
const char *MStringStackReader::Read(void)
	{
	if(mList==NULL) { return NULL; }
	const MStringStackNode *retlist=mList;
	mList=mList->Next;  return retlist->Value;
	}



