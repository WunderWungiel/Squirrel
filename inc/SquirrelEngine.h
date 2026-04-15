#ifndef __SQUIRRELENGINE_H__
#define __SQUIRRELENGINE_H__


extern "C" {
#include <zbar.h>
}

#include "qrcodegen.h"

void SaveBMData(const char* fp, int w, int h, int bpp, void* data);

// payload types
typedef enum PLType
{
    PL_CONTACT = 0,
    PL_URL,
    PL_CALENDAR,
    PL_CALENDARv2,
    PL_EMAIL,
    PL_PHONE_NUMBER,
    PL_GEO_LOCATION,	
    PL_SMS,	
    PL_WIFI,	
    PL_TEXT,
    PL_BINARY
} PLType;


struct DecoderResult
{
    const char *payload; 
    int payload_len;
    PLType payload_type;
    int has_qrcode; 
    int code_pos[4][2];
};

class QRBARDecoder
{

public:
	QRBARDecoder();
	
	~QRBARDecoder();

	// setup the decoder states
	int Init();

	// create or resize the image
	int Init(int w, int h);

	// used for decoding fixed size images, to avoid memory reallocation.
	// Init(w, h) should be called before using this function
	int DecodeImageData(void* data, size_t len);

	// decode image data (1bpp).
	int DecodeImageData(void *data, int w, int h);
	// convert image data to (1bpp) then decode it
	int DecodeImageData(void* data, int w, int h, int bpp);

	// decode any image file
	int DecodeImage(const char *fp);
	int DecodeImage(const wchar_t *fp);

	/** 
	 * decode image data and extract the payload
	 * then try to detect some information
	 *
	 * if qrcode data found check the type if text or binary.

	 * those information are stored in the DecoderResult.

	 * returns the number of codes have be found , -1 if some error occurs.
	 *
	 * the function does extract only one code.
	 **/

	int ExtractCode(void* data, int w, int h, DecoderResult* res);


	/** 
	 * extract the payload from the current decoded image.
	 *
	 * returns the number of codes have be found , -1 if some error occurs.
	 *
	 **/

	int ExtractCode(int index, DecoderResult* res);

	// encode payload data into specific character encoding

	const char* EncodePayload(DecoderResult* qrc, int &outLen, const char* outEnc);


	// parse payload type and encode its data into specific character encoding
    
	const char* ParseAndEncodePayload(DecoderResult* qrc, int &outLen, const char* outEnc);

	// functions for error handling
	void SetError(const char *error);
	void SetError(char *error);
	const char* GetError();
	const wchar_t* GetWError();

private:
	
	void GetCodePosition(const zbar_symbol_t *symbol, DecoderResult* res);

	// functions for dealing with cached buffers
	void ReleaseImgBuf();

	int AllocateCachedPayload(const int len);

	// iconv() wrapper		
	int ConvertPayloadData(const char *inEnc, const char *outEnc, char *inBuf, size_t inBufLen);
	
	// very generic, not an actual parser.
	void ParsePayloadType(DecoderResult *qrc);

private:
	zbar_image_scanner_t *m_scanner;
	zbar_image_t *m_image;
	uint8_t *m_imgbuf;
	size_t m_imgbufLen;
	char* m_cachedPayload;
	size_t m_cachedPayloadSize;
	char m_error[1024];
};


class QRCEncoder
{

public:
	QRCEncoder();
	void SetParams(int ecl, int minVersion=qrcodegen_VERSION_MIN, int maxVersion=qrcodegen_VERSION_MAX);
	int EncodeText(const char *text);

	int GetModule(int x, int y);
	int MinVersion() { return m_minVersion; };	
	int MaxVersion() { return m_maxVersion; };	
	int Ecl() { return m_ecl; };
	
	static const int MaxBufferSize = qrcodegen_BUFFER_LEN_MAX;


private:
	int m_ecl;
	int m_minVersion;
	int m_maxVersion;

private:
	int m_qrcSize;
	static uint8_t QrcBuf[QRCEncoder::MaxBufferSize];

};


#endif // __SQUIRRELENGINE_H__
