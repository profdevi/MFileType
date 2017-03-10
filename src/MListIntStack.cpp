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

//v1.7 copyright Comine.com 20160214U2014
#include "MStdLib.h"
#include "MListIntStack.h"


//******************************************************
//**  MListIntStack class
//******************************************************
void MListIntStack::ClearObject(void)
	{
	mList=NULL;
	}


////////////////////////////////////////////////
MListIntStack::MListIntStack(bool construct)
	{
	ClearObject();
	if(construct==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MListIntStack::~MListIntStack(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MListIntStack::Create(void)
	{
	Destroy();

	return true;
	}


////////////////////////////////////////////////
bool MListIntStack::Destroy(void)
	{
	// Destroy linked list
	while(mList!=NULL)
		{
		Node *tmp=mList;
		mList=mList->Next;
		delete tmp;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MListIntStack::Push(int value)			// Push another element
	{
	Node *ptr;
	ptr=new Node;
	if(ptr==NULL)
		{
		return false;
		}

	ptr->Next=mList;
	ptr->Value=value;
	mList=ptr;
	return true;
	}


////////////////////////////////////////////////
bool MListIntStack::Pop(void)					// Pop top element
	{
	if(mList==NULL)
		{
		return false;
		}

	Node *tmp=mList;
	mList=mList->Next;
	delete tmp;
	return true;
	}


////////////////////////////////////////////////
bool MListIntStack::IsEmpty(void)				// Checks if Stack is empty
	{
	if(mList==NULL) { return true; }

	return false;
	}


////////////////////////////////////////////////
int MListIntStack::GetTop(void)					// Get Top Element of stack
	{
	if(mList==NULL)
		{
		return 0;
		}

	return mList->Value;
	}




