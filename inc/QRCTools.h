#ifndef __QRCTOOLS_h__
#define __QRCTOOLS_h__

#include "quirc.h"
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
    PL_TEXT, // plain text
    PL_BINARY
} PLType;

// wraps the necessary C structures into a single container.
struct QRC
{
    struct quirc_code code;
    struct quirc_data data;
    quirc_decode_error_t error;
    PLType payload_type;
};


class QRCDecoder
{

public:
	QRCDecoder();
	~QRCDecoder();

	// setup the decoder states
	int Init();

	// create or resize the quirc buffer
	int Init(int w, int h);

	/******
	 * used for decoding fixed size images to avoid memory reallocation.
	 * the data is copied into the internal buffer that was allocated via Init(w, h)
	*****/
	int DecodeImageData(void* data, size_t len);

	// decode image data (1bpp).
	int DecodeImageData(void *data, int w, int h);
	// convert image data to (1bpp) then decode it
	int DecodeImageData(void* data, int w, int h, int bpp);

	// decode any image file
	int DecodeImage(const char *fp);
	int DecodeImage(const wchar_t *fp);

	// find qrc code
	int ExtractCode(int index, QRC *qrc);

	// parse payload type and encode its data into specific character encoding
	const char* ParseAndEncodePayload(QRC* qrc, int &outLen, const char* outEnc);

	// functions for error handling
	void SetError(const char *error);
	void SetError(char *error);
	const char* GetError();
	const wchar_t* GetWError();

private:
	// functions for dealing with cached buffers
	void ReleaseImgBuf();

	int AllocateCachedPayload(const int len);

	// iconv() wrapper		
	int ConvertPayloadData(const char *inEnc, const char *outEnc, char *inBuf, size_t inBufLen);
	
	// very generic, not an actual parser.
	void ParsePayloadType(QRC *qrc);

private:
	struct quirc *m_quirc;
	int m_numCodes;
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


#endif // __QRCTOOLS_h__
