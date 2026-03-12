/*
 ============================================================================
 Name		: SquirrelScannerView.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __SQUIRRELSCANNERVIEW_h__
#define __SQUIRRELSCANNERVIEW_h__

// INCLUDES
#include <aknview.h>
#include <ecam.h>
#include "SquirrelModel.h"


// CLASS DECLARATION

class CQRCScanView : public CCoeControl, public MCameraObserver
{

public: 
        // Constructors
        static CQRCScanView* NewL(const TRect& aRect , const CCoeControl* aParent=NULL);
        static CQRCScanView* NewLC(const TRect& aRect, const CCoeControl* aParent=NULL);
        virtual ~CQRCScanView();
    
private: 
        // Functions from base classes
        void Draw(const TRect& aRect ) const;
        void SizeChanged();
	//TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

public:
	TInt SetupCamera(TInt aIndex=0);
	void CleanupCamera();
	void StartViewFinder();
	void StopViewFinder();

private:

	CCamera::TFormat GetImageMaxFormat();
	TInt GetImageMaxSize();
	void Capture(CCamera::TFormat aFormat);
        void CreateBackBufferL();
        void ReleaseBackBuffer();
	void ShowError();
	void FindCode(CFbsBitmap* aBitmap);

 
private: // From MCameraObserver

	void ReserveComplete(TInt aError);
	void PowerOnComplete(TInt aError);
	void ViewFinderFrameReady(CFbsBitmap& aFrame );

	void ImageReady(CFbsBitmap* aBitmap, HBufC8* aData, TInt aError);
	void FrameBufferReady(MFrameBuffer* aFrameBuffer, TInt aError);

    
private: 
        // Constructors
        void ConstructL(const TRect& aRect, const CCoeControl* aParent);
        CQRCScanView();

private: 
	TInt iError;
        CCamera* iCamera;
	TCameraInfo iCameraInfo;
	TInt iCameraIndex;
        TSize iViewFinderSize;  
        CFbsBitmap* iBackBuffer;
        CFbsBitmapDevice* iBackBufferDevice;
        CFbsBitGc* iBackBufferContext;
	QRCScanHelper iScanHelper;
	TBool iFoundCode;
	TBool iViewFinderActivated;
};


class CSquirrelScannerView : public CAknView
{
public:
	// New methods
	static CSquirrelScannerView* NewL();
	static CSquirrelScannerView* NewLC();

	/**
	 * ~CSquirrelScannerView
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelScannerView();

public:
    
        /**
        * From CAknView.
        * Returns views id.
        *
        * @return Id for this view.
        */
        TUid Id() const;
    
        /**
        * From CAknView.
        * Handles commands
        *
        * @param aCommand A command to be handled.
        */
        void HandleCommandL( TInt aCommand );
  

	void HandleViewRectChange();

        /**
        * From CAknView.
        * Activates view
        *
        * @param aPrevViewId Specifies the view previously active.
        * @param aCustomMessageId Specifies the message type.
        * @param aCustomMessage The activation message.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        /**
        * From CAknView.
        * Deactivates view.
        */
        void DoDeactivate();

	void HandleForegroundEventL(TBool aForeground);

private:
	// Constructors
	void ConstructL();

	/**
	 * CSquirrelScannerView.
	 * C++ default constructor.
	 */
	CSquirrelScannerView();
private:
	CQRCScanView *iQRCScanView;
};

#endif // __SQUIRRELSCANNERVIEW_h__
// End of File
