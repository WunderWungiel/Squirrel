/*
* ==============================================================================
*  Name        : ImageHandler.cpp
*  Part of     : OcrExample
*  Interface   :
*  Description :
*  Version     :
*
*  Copyright (c) 2006 Nokia Corporation.
*  This material, including documentation and any related
*  computer programs, is protected by copyright controlled by
*  Nokia Corporation.
* ==============================================================================
*/

#include "ImageHandler.h"

using namespace NOCRUtils;

// =============================================================================
// CImageHandler, a utility class for loading images.
// =============================================================================

// ============================= MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CImageHandler::CImageHandler
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CImageHandler::CImageHandler(CFbsBitmap* aBitmap, MImageHandlerCallback& aCallback)
    : CActive(CActive::EPriorityStandard)
    , iDecoder(NULL)
    , iScaler(NULL)
    , iCallback(aCallback)
    , iBitmap(aBitmap)
    , iScaledBitmap(NULL)
    , iSize(0,0)
    , iState(EIdle)
    {
    
    }


// -----------------------------------------------------------------------------
// CImageHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CImageHandler::ConstructL()
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CImageHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CImageHandler* CImageHandler::NewL(CFbsBitmap* aBitmap, MImageHandlerCallback& aCallback)
{
    CImageHandler* self = NewLC(aBitmap, aCallback);
    CleanupStack::Pop();
    return self;
}

// -----------------------------------------------------------------------------
// CImageHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CImageHandler* CImageHandler::NewLC(CFbsBitmap* aBitmap, MImageHandlerCallback& aCallback)
{

    CImageHandler* self = new (ELeave) CImageHandler(aBitmap, aCallback);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
}

// Destructor
CImageHandler::~CImageHandler()
{
    delete iDecoder;
    delete iScaler;
    if (iScaledBitmap) delete iScaledBitmap;
}

// -----------------------------------------------------------------------------
// CImageHandler::SetBitmap
// Set the destination bitmap.
// -----------------------------------------------------------------------------
//
TBool CImageHandler::SetBitmap(CFbsBitmap *aBitmap )
{

    if (IsActive()) return EFalse;
    iBitmap = aBitmap;
    return ETrue;

}

// -----------------------------------------------------------------------------
// CImageHandler::LoadFileL
// Loads a selected frame from a named file
// -----------------------------------------------------------------------------
//
void CImageHandler::LoadFileL(RFs& aFs, const TFileName& aFileName, TInt aSelectedFrame)
    {
    __ASSERT_ALWAYS(!IsActive(),User::Invariant());
    if ( iDecoder )
        {
        delete iDecoder;
        }


    iDecoder = NULL;
    iDecoder = CImageDecoder::FileNewL(aFs, aFileName);
    // Get image information
    iFrameInfo = iDecoder->FrameInfo(aSelectedFrame);
    // Resize to fit.
    TRect bitmapRect = iFrameInfo.iFrameCoordsInPixels;

    // release possible previous image
    iBitmap->Reset();
    iBitmap->Create(bitmapRect.Size(), EColor16M);

    // Decode as bitmap.
    iDecoder->Convert(&iStatus, *iBitmap, aSelectedFrame);
    SetActive();
    }

void CImageHandler::LoadFileAndScaleL(RFs& aFs, const TFileName& aFileName,
                                      const TSize &aSize,
                                      TInt aSelectedFrame)
{
    __ASSERT_ALWAYS(!IsActive(),User::Invariant());
    iSize = aSize;
    iState = ELoadingImage;
    LoadFileL(aFs, aFileName, aSelectedFrame);
}

void CImageHandler::ScaleL(const TSize &aSize)
{
    __ASSERT_ALWAYS(!IsActive(),User::Invariant());
    if ( iScaler )
    {
        delete iScaler;
    }

    if (!iScaledBitmap){

	iScaledBitmap = new (ELeave) CFbsBitmap;
    }
    
    else if (!iScaledBitmap->Handle())
    {
	iScaledBitmap = new (ELeave) CFbsBitmap;
    }

    iScaledBitmap->Reset();
    iScaledBitmap->Create(aSize, EColor16M);

    iScaler = NULL;
    iScaler = CBitmapScaler::NewL();
    iState = EScalingImage;
    iScaler->Scale(&iStatus, *iBitmap, *iScaledBitmap, EFalse/*ETrue*/);

    SetActive();
}

CFbsBitmap* CImageHandler::ScaledBitmap()
{
    return iScaledBitmap;
}

// -----------------------------------------------------------------------------
// CImageHandler::FrameInfo
// Get the current frame information.
// -----------------------------------------------------------------------------
//
const TFrameInfo& CImageHandler::FrameInfo() const
    {
    return iFrameInfo;
    }

// -----------------------------------------------------------------------------
// CImageHandler::RunL
// CActive::RunL() implementation. Called on image load success/failure.
// -----------------------------------------------------------------------------
//
void CImageHandler::RunL()
{
    TInt err = iStatus.Int();
    if ((iState == ELoadingImage) && !err)
    {
	ScaleL(iSize);
    }
    else
    {
        // Invoke callback. 
	iState = EIdle;
        iCallback.ImageOperationCompleteL(err);
    }
}

// -----------------------------------------------------------------------------
// CImageHandler::DoCancel
// CActive::Cancel() implementation. Stops decoding.
// -----------------------------------------------------------------------------
//
void CImageHandler::DoCancel()
    {
    if ( iDecoder )
        {
        iDecoder->Cancel();
        }
    if ( iScaler )
        {
        iDecoder->Cancel();
        }
    }



