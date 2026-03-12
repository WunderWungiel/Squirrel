#include "HistoryStore.h"
#include <s32mem.h>
#include <bautils.h>
#include <coemain.h> // CCoeEnv

// Implementation of THistoryItem


THistoryItem::THistoryItem()
{
    SetTimestamp();
}

THistoryItem::THistoryItem(const TFileName &aFileName)
{
    iFileName = aFileName;
    SetTimestamp();
}

void THistoryItem::SetTimestamp()
{
    TTime time;
    time.HomeTime();
    iTimestamp = time.Int64();
}


void THistoryItem::ExternalizeL(RWriteStream& aStream) const
{
    aStream << iFileName;
    aStream << iTimestamp;
}

void THistoryItem::InternalizeL(RReadStream& aStream)
{
    aStream >> iFileName;
    aStream >> iTimestamp;
}

void THistoryItem::GetNameAndDate(TDes &aBuf, const TDesC &aSeperator)
{
    aBuf.Append(iFileName);
    aBuf.Append(aSeperator);
    TTime time(iTimestamp);
    TDateTime dateTime = time.DateTime();
    _LIT(KFmt, "%d/%d/%d %d:%d:%d");
    aBuf.AppendFormat(KFmt,
	    dateTime.Day()+1,
	    dateTime.Month()+1,
	    dateTime.Year(),
	    dateTime.Hour(),
	    dateTime.Minute(),
	    dateTime.Second()
	    );
}


// Implementation of CItemArray

CItemArray::~CItemArray()
	{
	delete iArray;
	}

CItemArray::CItemArray(CStreamStore& aStore)
	: iStore(aStore)
	{}

TStreamId CItemArray::CreateL(CStreamStore& aStore)
// create the stream representation of the class
	{
				// use a temporary CItemArray
	CItemArray* self=new(ELeave) CItemArray(aStore);
	CleanupStack::PushL(self);
				// construct object
	self->ConstructL();
				// create new stream
	RStoreWriteStream outstream;
	TStreamId id=outstream.CreateLC(aStore);
				// write  external rep
	self->ExternalizeL(outstream);
				// commit stream
	outstream.CommitL();
				// cleanup stream and temporary self
	CleanupStack::PopAndDestroy(2);
	return id;
	}

CItemArray* CItemArray::NewLC(CStreamStore& aStore,TStreamId anId)
// construct a CItemArray from persistent storage
	{
	CItemArray* self=new(ELeave) CItemArray(aStore);
	CleanupStack::PushL(self);
				// construct object
	self->ConstructL();
				// set the stream id for StoreL/RestoreL
	self->iMyId=anId;
				// restore the internal rep.
	self->RestoreL();
	return self;
	}

void CItemArray::StoreL() const
// replace external rep. with internal one
	{
	RStoreWriteStream outstream;
	outstream.ReplaceLC(iStore,iMyId);
	ExternalizeL(outstream);
	outstream.CommitL();
	CleanupStack::PopAndDestroy();
	}

void CItemArray::RestoreL()
// replace internal rep with external one
	{
	iArray->Reset();
	RStoreReadStream instream;
	instream.OpenLC(iStore,iMyId);
	InternalizeL(instream);
	CleanupStack::PopAndDestroy();
	}

void CItemArray::AddItemL(const THistoryItem& anItem)
// add item to the collection
	{
	// write external rep of item
	RStoreWriteStream outstream;
	TStreamId id=outstream.CreateLC(iStore);
	outstream<<anItem;
	outstream.CommitL();
	CleanupStack::PopAndDestroy();
	// add new stream id to the internal array
	iArray->AppendL(id);
	}

void CItemArray::RemoveItemL(TInt anIndex)
// remove an item from the collection
	{
	/**** this does not delete the content! ***/
	// remove the stream from the store
	//iStore.DeleteL((*iArray)[anIndex]);
	/*RStoreWriteStream outstream;
	outstream.ReplaceLC(iStore, (*iArray)[anIndex]);
	outstream.CommitL();
	CleanupStack::PopAndDestroy();*/

	// remove the entry from the internal array
	iArray->Delete(anIndex);
	}

TInt CItemArray::Count() const
	{
	return iArray->Count();
	}

void CItemArray::GetItemL(THistoryItem &anItem, TInt anIndex) const
// retrieve an item from the store
	{
	RStoreReadStream instream;
	instream.OpenLC(iStore,(*iArray)[anIndex]);
	instream>>anItem;
	CleanupStack::PopAndDestroy();
	}

void CItemArray::ConstructL()
	{
	iArray=new(ELeave) CArrayFixFlat<TStreamId>(8);
	}

void CItemArray::ExternalizeL(RWriteStream& aStream) const
	{
				// stream out the array
	aStream<<*iArray;
	}

void CItemArray::InternalizeL(RReadStream& aStream)
	{
				// stream in the array
	aStream>>*iArray;
	}


void CItemArray::ResetL() const
// replace external rep. with internal one
	{
	iArray->Reset();
	RStoreWriteStream outstream;
	outstream.ReplaceLC(iStore,iMyId);
	ExternalizeL(outstream);
	outstream.CommitL();
	CleanupStack::PopAndDestroy();
	}

// Implementation of CHistoryStore

CHistoryStore::CHistoryStore(TInt aMaxItems)
{
    iFileStore = NULL;
    iItemsArray = NULL;
    iItems = NULL;
    iItemsChanged = EFalse;
    iMaxItems = aMaxItems;
}

CHistoryStore::~CHistoryStore()
{
    TRAP_IGNORE(StoreL()); // not a good idea?
			   //
    if (iItemsArray) delete iItemsArray;
    if (iFileStore) delete iFileStore;
    if (iItems) delete iItems;
}

void CHistoryStore::OpenL(const TDesC &aFileName,  const TBool &aLoadItems)
{
    RFs& rfs = CCoeEnv::Static()->FsSession();
    TBool storeExists = BaflUtils::FileExists(rfs, aFileName);

    if (!storeExists)
    {
	iFileStore = CPermanentFileStore::CreateL(rfs, aFileName, EFileRead|EFileWrite);
    }
    else
    {
	iFileStore = CPermanentFileStore::OpenL(rfs, aFileName, EFileRead|EFileWrite);

    }
    
    iFileStore->SetTypeL(iFileStore->Layout());
    if (!storeExists)
    {
	TStreamId id = CItemArray::CreateL(*iFileStore);
	iFileStore->SetRootL(id);
	iFileStore->CommitL();
    }

    // get the root stream into memory
    
    iItemsArray = CItemArray::NewLC(*iFileStore, iFileStore->Root());

    if (aLoadItems) LoadItemsL();

    CleanupStack::Pop(iItemsArray); // iItemsArray
}

CHistoryStore* CHistoryStore::OpenLC(const TDesC &aFileName)
{
    CHistoryStore* self = new (ELeave)  CHistoryStore;
    CleanupStack::PushL(self);
    self->OpenL(aFileName, ETrue);
    CleanupStack::Pop(self);
    return self;
}

void CHistoryStore::AddItemL(const TFileName &aFileName)
{
    if (!iItems) return;
   
    TInt count = iItems->Count();
    if (count == iMaxItems) iItems->Delete(count-1);

    for (TInt i = 0; i < iItems->Count(); i++)
    {
	if ((*iItems)[i].iFileName == aFileName)
	{
	    iItems->Delete(i);
	    break;
	}
    }

    THistoryItem item(aFileName);
    iItems->InsertL(0, item); // add item to top position
    iItemsChanged = ETrue;
}

void CHistoryStore::RemoveItemL(const TInt &aIndex)
{

    if (!iItems) return;
   
    if (aIndex < 0 || aIndex >= iItems->Count()) return;

    iItems->Delete(aIndex);
    iItemsChanged = ETrue;
}


void CHistoryStore::GetItem(TFileName &aFileName, const TInt &aIndex)
{
    aFileName.Copy((*iItems)[aIndex].iFileName);
}

void CHistoryStore::ResetL(const TBool &aResetItems)
{

    if (!iItemsArray || !iFileStore || !iItems) return;
    
    TFileName fn;
    User::LeaveIfError(iFileStore->File().FullName(fn));

    delete iItemsArray;
    delete iFileStore;
    User::LeaveIfError(BaflUtils::DeleteFile(CCoeEnv::Static()->FsSession(),  fn));

    OpenL(fn, aResetItems /*do not reload the items! */);
}


// commit changed, overwrite items
void CHistoryStore::StoreL()
{
    TInt count = iItems->Count();
 
    if (!iItemsChanged || !iItems) return;

    ResetL(EFalse);
    for (TInt i = 0; i < count; i++)
    {
	iItemsArray->AddItemL((*iItems)[i]);
    }

    iItemsArray->StoreL();
    iFileStore->CommitL();   
}

void CHistoryStore::LoadItemsL()
{

    if (!iItemsArray || !iFileStore) return;
 
    if (!iItems) iItems = new (ELeave) CArrayFixFlat<THistoryItem>(15);

    else {iItems->Reset();}

    RFs& rfs = CCoeEnv::Static()->FsSession();

    for (TInt i = 0; i < iItemsArray->Count(); i++)
    {
	THistoryItem item;
	iItemsArray->GetItemL(item, i);
	if (!BaflUtils::FileExists(rfs, item.iFileName))
	{
	    iItemsArray->RemoveItemL(i);
	    iItemsChanged = ETrue;
	    continue;
	}

	iItems->AppendL(item);
    }
}

void CHistoryStore::RemoveItemsL()
{

    if (!iItemsArray || !iFileStore || !iItems) return;
   
    if (iItems->Count() == 0) return;

    iItems->Reset();
    iItemsChanged = ETrue;
}


void CHistoryStore::GetItemsL(CDesCArray &aDesArray, const TDesC &aSeperator)
{

    if (!iItems) return;
 
    TInt count = iItems->Count();
    for (TInt i = 0; i < count; i++)
    {
	TBuf<KMaxFileName+64> item(aSeperator);
	(*iItems)[i].GetNameAndDate(item, aSeperator);
	aDesArray.AppendL(item);
    }
}


const TBool CHistoryStore::IsEmpty()
{
    if (!iItemsArray || !iFileStore || !iItems) return ETrue;
    return  (iItems->Count() < 1); 
}

