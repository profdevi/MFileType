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




