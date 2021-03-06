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

//v1.9 copyright Comine.com 20160214U2010
#ifndef MStringQueue_h
#define MStringQueue_h

//******************************************************
//**  MStringQueue class
//******************************************************
class MStringQueue
	{
	////////////////////////////////////////////////
	int mQueueSize;							// Number of Queued Items
	struct GNode *mFirstNode;				// Always points to a dummy node
	struct GNode *mLastNode;				// Last Node in list
	struct GNode *mReadNode;				// Current Read Node

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MStringQueue(void);
	explicit MStringQueue(bool create);
	~MStringQueue(void);
	bool Create(void);
	bool Destroy(void);
	bool Enqueue(const char *string);
	bool Enqueue(const MStringQueue &queue);
	bool Dequeue(void);
	int GetQueueLength(void);
	const char *GetFront(void);					// Returns first item in List
	bool Print(void);							// Print out the Queue

	// Reader Code
	bool ReadReset(void);
	bool Read(void);
	const char *ReadGet(void);

	// Extra Operations
	bool Sort(void);							// Sort in Alphabetical Order + Will Reset the read
	};

#endif // MStringQueue_h

