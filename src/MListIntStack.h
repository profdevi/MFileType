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
#ifndef MListIntStack_h
#define MListIntStack_h

//******************************************************
//**  MListIntStack class
//******************************************************
class MListIntStack
	{
	////////////////////////////////////////////////
	struct Node { int Value; Node *Next; };

	////////////////////////////////////////////////
	Node *mList;

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MListIntStack(bool construct=false);
	~MListIntStack(void);
	bool Create(void);
	bool Destroy(void);
	bool Push(int value);			// Push another element
	bool Pop(void);					// Pop top element
	bool IsEmpty(void);				// Checks if Stack is empty
	int GetTop(void);				// Get Top Element of stack
	};

#endif // MListIntStack_h

