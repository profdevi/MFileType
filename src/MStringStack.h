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
#ifndef MStringStack_h
#define MStringStack_h


////////////////////////////////////////////////////
struct MStringStackNode
	{
	MStringStackNode *Next;
	char *Value;
	};

//******************************************************
//**  MStringStack class
//******************************************************
class MStringStackReader;
class MStringStack
	{
	////////////////////////////////////////////////
	MStringStackNode *mList;				// List of Nodes
	int mCount;					// Number of Elements in stack
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MStringStack(bool create=false);
	~MStringStack(void);
	bool Create(void);
	bool Destroy(void);
	bool Push(const char *string);		// Push an item onto stack
	bool Pop(void);						// Pop an item off stack
	const char *GetTop(void);			// Get Top Item in Stack
	bool IsEmpty(void);					// Check if stack is empty
	bool IsElement(const char *string);	// Check if element is of set
	bool Reverse(void);					// Reverse Stack Elements
	int GetCount(void);					// Returns Element count
	int GetElementPos(const char *str);	// =-1 on failure, 0(Top of stack)
	bool GetReader(MStringStackReader &reader);		// Init Reader
	};


/////////////////////////////////////////////////
class MStringStackReader
	{
	const MStringStackNode *mList;

	//////////////////////////////////////////////
	void ClearObject(void);

	//////////////////////////////////////////////
	public:
	MStringStackReader(void);
	~MStringStackReader(void);
	bool Create(const MStringStackNode *list);
	bool Destroy(void);
	const char *Read(void);
	};

#endif // MStringStack_h

