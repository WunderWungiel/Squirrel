#ifndef __HISTORYSTORE_H__
#define __HISTORYSTORE_H__

#include <s32file.h>
#include <s32strm.h>
#include <badesca.h>

class THistoryItem
{
    public:

	THistoryItem();
	THistoryItem(const TFileName &aFileName);
	void SetTimestamp();
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	void GetNameAndDate(TDes &aBuf, const TDesC &aSeperator);

    public:
	TFileName iFileName;
	TInt64 iTimestamp;
};

// CItemArray copied from examples/syslibs/filestores/writeperm/WritePermFS2.cpp 
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.

// Example to demonstrate use of CPermanentFileStore


// The main object's in-memory representation
class CItemArray : public CBase
{
public:
	static TStreamId CreateL(CStreamStore& aStore);

	~CItemArray();
	static CItemArray* NewLC(CStreamStore& aStore,TStreamId anId);
	void RestoreL();
	void StoreL() const;
	void ExternalizeL(RWriteStream& aStream) const;

	void AddItemL(const THistoryItem &anItem);
	void RemoveItemL(TInt anIndex);
	TInt Count() const;
	void GetItemL(THistoryItem &anItem,TInt anIndex) const;
	void CItemArray::ResetL() const;

protected:
	CItemArray(CStreamStore& aStore);
	void ConstructL();
	void InternalizeL(RReadStream& aStream);
private:
	CStreamStore& iStore;
	TStreamId iMyId;
	CArrayFixFlat<TStreamId> *iArray;
};


// a simple file history manager based on FileStore.

class CHistoryStore: public CBase

{
    public:
	CHistoryStore(TInt aMaxItems=40);
	~CHistoryStore();
   
    public:
	void OpenL(const TDesC &aFileName, const TBool &aLoadItems);
	static CHistoryStore* OpenLC(const TDesC &aFileName);
	void AddItemL(const TFileName &aFileName);
	void RemoveItemL(const TInt &aIndex);
	void GetItem(TFileName &aFileName, const TInt &aIndex);
	void ResetL(const TBool &aResetItems);
	// rewrite the filestore.
	void StoreL();
	// reset items array
	void RemoveItemsL();
	// get items in format suitable for displaying on listboxs
	void GetItemsL(CDesCArray &aDesArray, const TDesC &aSeperator);
	const TBool IsEmpty();

    private:
	void LoadItemsL();
    private:
	CFileStore *iFileStore;
	CItemArray *iItemsArray; // Array in FileStore holding Indexes.
	CArrayFixFlat<THistoryItem> *iItems; // Array in memory.
	TBool iItemsChanged; // changes when an item is added or removed.
	TInt iMaxItems;
};

#endif //__HISTORYSTORE_H__

// End of file

