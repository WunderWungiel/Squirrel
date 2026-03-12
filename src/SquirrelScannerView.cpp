/*
   ============================================================================
Name		: SquirrelAppView.cpp
Author	  : JigokuMaster
Copyright   : Your copyright notice
Description : Application view implementation
============================================================================
*/

// INCLUDE FILES
#include <coemain.h>
#include "SquirrelAppUi.h"
#include "SquirrelScannerView.h"
#include "Squirrel.rsg"
#include "Squirrel.hrh"
#include "uicommons.h"


/*** Some stuff here were copied from the PyS60 camera module (MisoPhotoTaker)
***/

static TInt CopyBitmap(CFbsBitmap* aFrame, CFbsBitmap* &aFrameCopy)
{
    TInt err = KErrNone;
    // CFbsBitmap copying from http://forum.newlc.com/index.php/topic,10093.0.html
  
    TRAP(err, {
    aFrameCopy = new (ELeave) CFbsBitmap();
    CleanupStack::PushL(aFrameCopy);
    aFrameCopy->Create(aFrame->SizeInPixels(), aFrame->DisplayMode());
    
    CFbsBitmapDevice* fbsDev = CFbsBitmapDevice::NewL(aFrameCopy);
    CleanupStack::PushL(fbsDev);
    CFbsBitGc* fbsGc = CFbsBitGc::NewL();
    CleanupStack::PushL(fbsGc);
    fbsGc->Activate(fbsDev);
    fbsGc->BitBlt(TPoint(0,0), aFrame);
    
    CleanupStack::PopAndDestroy(2); // fbsDev, fbsGc
    CleanupStack::Pop(1); // aFrameCopy 
    });
  return err;
}

// implementation of CQRCScanView

CQRCScanView* CQRCScanView::NewL(const TRect& aRect, const CCoeControl* aParent)
{
    CQRCScanView* self = CQRCScanView::NewLC(aRect, aParent);
    CleanupStack::Pop(self);
    return self;
}

CQRCScanView* CQRCScanView::NewLC(const TRect& aRect,const CCoeControl* aParent)
{
    CQRCScanView* self = new (ELeave) CQRCScanView;
    CleanupStack::PushL(self );
    self->ConstructL(aRect, aParent);
    return self;
}

void CQRCScanView::ConstructL(const TRect& aRect ,const CCoeControl* aParent)
{
    iViewFinderSize = /*TSize(177, 177);*/aRect.Size();

    if (aParent) CreateWindowL(aParent);
    else CreateWindowL();

    // Set the windows size
    SetRect(aRect);  
    // Activate the window, which makes it ready to be drawn
    ActivateL();

}


CQRCScanView::CQRCScanView()
{
    iCameraIndex = -1;
    iError = KErrNone;
    iFoundCode = EFalse;
    iViewFinderActivated = EFalse;
}

CQRCScanView::~CQRCScanView()
{
    CleanupCamera();
}


CCamera::TFormat CQRCScanView::GetImageMaxFormat()
{

    if (iCameraInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor16M)
    {
	return CCamera::EFormatFbsBitmapColor16M;
    }

    else if (iCameraInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor64K)
    {
	return CCamera::EFormatFbsBitmapColor64K;
    }

    else if (iCameraInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor4K)
    {
	return CCamera::EFormatFbsBitmapColor4K;
    }
}

TInt CQRCScanView::GetImageMaxSize()
{
    return iCameraInfo.iNumImageSizesSupported;
}


TInt CQRCScanView::SetupCamera(TInt aIndex)
{
    CleanupCamera();
    if (!CCamera::CamerasAvailable())
    {
	iError = KErrHardwareNotAvailable;
    }
    else
    {

	TRAP(iError, {
	    
	    iCamera = CCamera::NewL(*this, aIndex);
	        
	    iCamera->CameraInfo(iCameraInfo);

	    CreateBackBufferL();

	    iCameraIndex = aIndex;

	    });
    }

    ShowError();
    return iError;
}


void CQRCScanView::CleanupCamera()
{
    iCameraIndex = -1;
    iFoundCode = EFalse;
    if (iCamera)
    {

	StopViewFinder();
	delete iCamera;
	iCamera = NULL;
    }

    ReleaseBackBuffer();
    iError = KErrNone;
}


void CQRCScanView::StartViewFinder()
{

    if(iCamera && !iError && !iViewFinderActivated/*!iCamera->ViewFinderActive()*/){
	iCamera->Reserve();
	iViewFinderActivated = ETrue;
    }
}


void CQRCScanView::StopViewFinder()
{

    if(iCamera/* && iCamera->ViewFinderActive()*/)
    {
	if (iViewFinderActivated) iCamera->StopViewFinder();
	iCamera->PowerOff();                   
	iCamera->Release();
    }
    iViewFinderActivated = EFalse; 
}

void CQRCScanView::Draw(const TRect& aRect) const
{

    CWindowGc& gc = SystemGc();
  
    if(iBackBuffer)
    {
	// Draw backbuffer that has camera picture
	gc.BitBlt(TPoint(0, 0), iBackBuffer);
    }
    else
    {
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbWhite);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
    }

}


void CQRCScanView::SizeChanged()
{


    TSize size = Size(); 
    if (size.iWidth > size.iHeight)
    {
        iViewFinderSize = TSize(size.iWidth, size.iWidth);
    }
    else
    {
        iViewFinderSize = TSize(size.iHeight, size.iHeight);
    }

    if(iCameraIndex > (-1))
    {
	// reset  
	SetupCamera(iCameraIndex);
    }
}

void CQRCScanView::CreateBackBufferL()
{
    // create back buffer bitmap
    iBackBuffer = new (ELeave) CFbsBitmap;
    
    User::LeaveIfError(iBackBuffer->Create(Size(),EColor16M));
    
    // create back buffer graphics context
    iBackBufferDevice = CFbsBitmapDevice::NewL(iBackBuffer);
    User::LeaveIfError(iBackBufferDevice->CreateContext(iBackBufferContext));
    iBackBufferContext->SetPenStyle(CGraphicsContext::ESolidPen);
    
    iBackBufferContext->SetBrushColor(KRgbBlack);
    iBackBufferContext->Clear();
    iBackBufferContext->UseFont(iEikonEnv->TitleFont());

    iScanHelper.iGc = iBackBufferContext;
}

void CQRCScanView::ReleaseBackBuffer()
{
    if (iBackBufferContext)
    {
        delete iBackBufferContext;
        iBackBufferContext = NULL;
    }
    if (iBackBufferDevice)
    {
        delete iBackBufferDevice;
        iBackBufferDevice = NULL;
    }

    if (iBackBuffer)
    {
        delete iBackBuffer;
        iBackBuffer = NULL;
    }
}


void CQRCScanView::ShowError()
{
   
    if(iError != KErrNone)
    {
	TRAPD(err, ShowErrorL(iError));
	if (err != KErrNone){
	    CleanupCamera();
	}
    }
}

void CQRCScanView::FindCode(CFbsBitmap* aBitmap)
{

    TInt ret = iScanHelper.FindCode(aBitmap);
    if (ret == -1)
    {
	// memory error probably ...
	const TUint16* error = iScanHelper.GetError();
	if (error)
	{
	    TPtrC errorPtr(error);
	    ErrorNoteL(&errorPtr);
	}
 
    }

    iFoundCode = (ret > 0);
    if (iFoundCode)
    {

	CFbsBitmap* bitmapCopy = NULL;
	iError = CopyBitmap(aBitmap, bitmapCopy);
	if (iError == KErrNone)
	{
	    // send bitmap to the decoder.
	    TRAP(iError, STATIC_CAST(CSquirrelAppUi*, iEikonEnv->EikAppUi())->ActivateDecoderViewL(bitmapCopy));
	}
	ShowError();
    }	
}


void CQRCScanView::ReserveComplete(TInt aError)
{
    iError = aError;
    iViewFinderActivated = !aError;
    if (!aError)
    {
	iCamera->PowerOn();
    }
    else{

	ShowError();
    }
}

void CQRCScanView::PowerOnComplete(TInt aError)
{
    iError = aError;
    iViewFinderActivated = !aError;
    if(!aError)
    {
    
	TRAP(iError, iCamera->StartViewFinderBitmapsL(iViewFinderSize));

    }  
    ShowError();
}


void CQRCScanView::ImageReady(CFbsBitmap* aBitmap, HBufC8* aData, TInt aError)
{
    iError = aError;
}

void CQRCScanView::ViewFinderFrameReady(CFbsBitmap& aFrame)
{

    if (iBackBufferContext && !iFoundCode)
    {
        TSize bmpSizeInPixels = aFrame.SizeInPixels();
        TInt xDelta = (Rect().Width() - bmpSizeInPixels.iWidth) / 2;
        TInt yDelta = (Rect().Height() - bmpSizeInPixels.iHeight) / 2;
        TPoint pos( xDelta, yDelta );

        // Copy received viewfinder picture to back buffer
        iBackBufferContext->BitBlt( pos, &aFrame, TRect(TPoint( 0, 0 ), bmpSizeInPixels ));

	FindCode(&aFrame);
        // Update backbuffer into screen 
        DrawNow();	

    }
}

void CQRCScanView::FrameBufferReady(MFrameBuffer* aFrameBuffer, TInt aError){}




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelScannerView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelScannerView* CSquirrelScannerView::NewL()
{
    CSquirrelScannerView* self = CSquirrelScannerView::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelScannerView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelScannerView* CSquirrelScannerView::NewLC()
{
    CSquirrelScannerView* self = new (ELeave) CSquirrelScannerView;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelScannerView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelScannerView::ConstructL()
{
    BaseConstructL(R_SCANNER_VIEW);
}

// -----------------------------------------------------------------------------
// CSquirrelScannerView::SquirrelScannerView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelScannerView::CSquirrelScannerView()
{

    iQRCScanView = NULL;

}

// -----------------------------------------------------------------------------
// CSquirrelScannerView::~SquirrelScannerView()
// Destructor.
// -----------------------------------------------------------------------------
//
CSquirrelScannerView::~CSquirrelScannerView()
{
}

TUid CSquirrelScannerView::Id() const 
{ 
    return TUid::Uid(EScannerView); 
}

void CSquirrelScannerView::HandleCommandL( TInt aCommand )
{
    switch( aCommand )
    {
	default:
	    AppUi()->HandleCommandL(aCommand);  
	    break;
    }
}

void CSquirrelScannerView::HandleViewRectChange()
{
    if (iQRCScanView)
    {
	iQRCScanView->SetRect(ClientRect());
    }   
}

void CSquirrelScannerView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
	TUid /*aCustomMessageId*/,
	const TDesC8& /*aCustomMessage*/ )
{


    iQRCScanView = CQRCScanView::NewL(ClientRect());
    iQRCScanView->SetMopParent(this);

    if(iQRCScanView->SetupCamera() == KErrNone)
    {
	iQRCScanView->StartViewFinder();
    }

    AppUi()->AddToStackL(*this, iQRCScanView);  

    Cba()->MakeCommandVisible(EAknSoftkeyOptions,  EFalse);

}

void CSquirrelScannerView::DoDeactivate()
{

    if (iQRCScanView)
    {
	AppUi()->RemoveFromViewStack(*this, iQRCScanView);
	delete iQRCScanView;
	iQRCScanView = NULL;
    }   
}

void CSquirrelScannerView::HandleForegroundEventL(TBool aForeground)
{
    if (!iQRCScanView) return;

    if (aForeground)
    {
	iQRCScanView->StartViewFinder();
    }
    else
    {
	iQRCScanView->StopViewFinder();
    }
}

// End of File
