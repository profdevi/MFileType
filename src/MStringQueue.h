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

