typedef class WaitList  *WaitListPtr, **WaitListHdl;class WaitList {	private:		userNode 	theWaitListHead;		userNodePtr	listPtr;							OSErr	 	WriteListToFile();				Boolean		AddUser(userNode theUser);		Boolean		CheckForUser(userNode theUser);			public:					WaitList();					~WaitList();				Boolean 	WaitListAdd(userNode theUser);		Boolean 	CheckInFromWaitList();		Boolean 	IsThereAWaitList();		void		PutAtFrontOfList(userNode theUser);		void		WaitlistBoxSetup(void);		void		WaitlistBoxEvent(EventRecord *theEvent);		OSErr 		ReadListFromFile();		};//Boolean WaitList (userNodePtr theUser);