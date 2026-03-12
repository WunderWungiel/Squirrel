
#include "QRCTools.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iconv.h>
#include <wchar.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

void SaveBMData(const char *fp, int w, int h, int bpp, void *data)
{
    stbi_write_bmp(fp, w, h, bpp, data);
}

static const char* GetEncodingString(uint32_t index)
{
    switch (index)
    {
        case QUIRC_ECI_ISO_8859_1:   return "ISO-8859-1";
        case QUIRC_ECI_IBM437:       return "IBM437";
        case QUIRC_ECI_ISO_8859_2:   return "ISO-8859-2";
        case QUIRC_ECI_ISO_8859_3:   return "ISO-8859-3";
        case QUIRC_ECI_ISO_8859_4:   return "ISO-8859-4";
        case QUIRC_ECI_ISO_8859_5:   return "ISO-8859-5";
        case QUIRC_ECI_ISO_8859_6:   return "ISO-8859-6";
        case QUIRC_ECI_ISO_8859_7:   return "ISO-8859-7";
        case QUIRC_ECI_ISO_8859_8:   return "ISO-8859-8";
        case QUIRC_ECI_ISO_8859_9:   return "ISO-8859-9";
        case QUIRC_ECI_WINDOWS_874:  return "Windows-874";
        case QUIRC_ECI_ISO_8859_13:  return "ISO-8859-13";
        case QUIRC_ECI_ISO_8859_15:  return "ISO-8859-15";
        case QUIRC_ECI_SHIFT_JIS:    return "SHIFT-JIS";
        case QUIRC_ECI_UTF_8:        return "UTF-8";
        default: break;
    }
    return "UTF-8";
}


static void Bin2HexStr(const uint8_t *bin, size_t binLen, char *outBuf)
{
    if (bin && outBuf)
    {
	char *outBufPtr = outBuf;
	for(int i=0 ; i < binLen; i++)
	{
	    outBufPtr += sprintf(outBufPtr, "\\x%02hhx", bin[i]); // ++append
	}	 
    }
}

#ifdef __SYMBIAN32__
#include <e32std.h>

static void Bin2HexWStr(const uint8_t *bin, size_t binLen, const wchar_t *outBuf, size_t outLen)
{
    if (bin && outBuf)
    {
	_LIT(KHex, "\\x%02x");
	TPtr16 p((TUint16*)outBuf, outLen/2);
	for(int i=0 ; i < binLen; i++)
	{
	    p.AppendFormat(KHex, bin[i]);
	}
	p.ZeroTerminate();
    }
}

#else 

static void Bin2HexWStr(const uint8_t *bin, size_t binLen, const wchar_t *outBuf, size_t outLen)
{
    //int resLen = 0;
    if (bin && outBuf)
    {
	wchar_t *outBufPtr = (wchar_t*)outBuf;
	for(int i=0 ; i < binLen; i++)
	{

	    outBufPtr += swprintf(outBufPtr, 5, L"\\x%02x", bin[i]);// ++append

	}	 
    }
    //return outLen;
}
#endif 

QRCDecoder::QRCDecoder()
{
    m_quirc = NULL;
    m_imgbuf = NULL;
    m_cachedPayload = NULL;
    m_cachedPayloadSize = NULL;
    m_imgbufLen = 0;
    SetError((const char*)NULL);
}

QRCDecoder::~QRCDecoder()
{
    if (m_quirc) quirc_destroy(m_quirc);
    m_quirc = NULL;

    if (m_cachedPayload) delete m_cachedPayload;
    m_cachedPayload = NULL;

    ReleaseImgBuf();
}

int QRCDecoder::Init()
{
    if (m_quirc) quirc_destroy(m_quirc);
    if (!(m_quirc = quirc_new()))
    {
	SetError(strerror(ENOMEM)); 
	return 0;
    }
    return 1;
}

int QRCDecoder::Init(int w, int h)
{
    int ret = 0;
    if (Init())
    {
	if ( quirc_resize(m_quirc, w, h) == -1)
	{
	    SetError(strerror(ENOMEM));
	}
	else { ret = 1; }
    }
       
    return ret;
}


void QRCDecoder::ReleaseImgBuf()
{

    if (m_imgbuf) stbi_image_free(m_imgbuf);
    m_imgbuf = NULL;
}


int QRCDecoder::DecodeImageData(void* data, size_t len)
{

    if (!m_quirc)
    {
	if (!Init()) return -1;
    }
   
    memcpy(quirc_begin(m_quirc, NULL, NULL), data,  len); 

    quirc_end(m_quirc);

    return (m_numCodes = quirc_count(m_quirc));
}


int QRCDecoder::DecodeImageData(void* data, int w, int h)
{

    if (!m_quirc && !Init()) return -1;

    if (quirc_resize(m_quirc, w, h) < 0) return -1;

    memcpy(quirc_begin(m_quirc, NULL, NULL), data, w*h);   
    quirc_end(m_quirc);
    return (m_numCodes = quirc_count(m_quirc));
}


int QRCDecoder::DecodeImageData(void* data, int w, int h, int bpp)
{

    size_t len = w*h;
    if (m_imgbufLen != len)
    {
	ReleaseImgBuf();
	m_imgbuf = (uint8_t*) malloc(len);
	if (!m_imgbuf)
	{
	    SetError(strerror(ENOMEM));
	    return -1;
	}
    }

    m_imgbufLen = len;
    stbi_convert_format(m_imgbuf, (stbi_uc*)data, bpp, 1, w, h);

    //SaveBMData("D:\\1bpp.bmp", w, h, 1, m_imgbuf);
    return DecodeImageData(m_imgbuf, w, h);
}


int QRCDecoder::DecodeImage(const char* fp)
{
    int w, h, bpp;

    ReleaseImgBuf();
    m_imgbuf = stbi_load(fp, &w, &h, &bpp, 1);

    if (!m_imgbuf)
    {
	SetError(stbi_failure_reason());
	return -1;
    }

    return DecodeImageData(m_imgbuf, w, h);
}


int QRCDecoder::DecodeImage(const wchar_t *fp)
{
    int w, h, bpp;
    FILE* f;

    if (!(f = wfopen(fp, L"rb")))
    {
	SetError(strerror(errno));
	return 0;
    }

    ReleaseImgBuf();
    m_imgbuf = stbi_load_from_file(f, &w, &h, &bpp, 1);

    if (!m_imgbuf)
    {
	SetError(stbi_failure_reason());
	return -1;
    }

    fclose(f);
    return DecodeImageData(m_imgbuf, w, h);
}

int QRCDecoder::ExtractCode(int index, QRC *qrc)
{
    if ( !m_quirc || (index > m_numCodes || index < 0))
    {
	SetError(strerror(EINVAL));
	return 0;
    }

    quirc_extract(m_quirc, index, &qrc->code);  
    
    qrc->error = quirc_decode(&qrc->code, &qrc->data);

    if (qrc->error  == QUIRC_ERROR_DATA_ECC)
    {
        quirc_flip(&qrc->code);
        qrc->error = quirc_decode(&qrc->code, &qrc->data);
    }

    if (qrc->error)
    {
	SetError(quirc_strerror(qrc->error));
	return 0;
    }
    return 1;
}

int QRCDecoder::AllocateCachedPayload(const int len)
{
    if (m_cachedPayload)
    {
	delete m_cachedPayload;
	m_cachedPayload = NULL;
	m_cachedPayloadSize = 0;
    }

    m_cachedPayload = new char[len];
    if(!m_cachedPayload)
    {
	SetError(strerror(errno));
	return 0;
    }
    m_cachedPayloadSize = len;
    return 1;
}

int QRCDecoder::ConvertPayloadData(const char *inEnc, const char *outEnc, char *inBuf, size_t inBufLen)
{
    int len;

    if (!m_cachedPayload) return 0;

    iconv_t cd = iconv_open(outEnc, inEnc);
    if(cd == (iconv_t)-1)
    {
	SetError(strerror(errno));
	return 0;
    }

    memset(m_cachedPayload, 0, m_cachedPayloadSize);

    const char* inBufPtr = (const char*) inBuf;
    char* outBufPtr = m_cachedPayload;
    size_t leftBytes = m_cachedPayloadSize;

    size_t nc = iconv(cd, &inBufPtr, &inBufLen, &outBufPtr, &leftBytes);
    if (nc == (size_t)-1)
    {
	SetError(strerror(errno));
	len = 0;
    }

    else
    {
	len = m_cachedPayloadSize - leftBytes;
    }

    iconv_close(cd);
    return len;
}

#define TRY_CONVERT_PAYLOAD(inEnc) do { \
	int ret = ConvertPayloadData(inEnc, outEnc, payload, payloadLen); \
	if (ret){ \
	    outLen = ret; \
	    return (const char*) m_cachedPayload;} \
    } while(0);

#define PAYLOAD2HEX() do { \
	if (!AllocateCachedPayload((payloadLen + 1)*10)) return NULL; \
	memset(m_cachedPayload, 0, m_cachedPayloadSize); \
	const wchar_t *wstr = (const wchar_t*) m_cachedPayload; \
	Bin2HexWStr((const uint8_t*) payload, payloadLen, wstr, m_cachedPayloadSize); \
	outLen = wcslen(wstr); \
	return (const char*) m_cachedPayload; \
    } while(0);


#define MATCH_PREFIX(s1, s2, l) (strncmp(s1, s2, l) == 0)

#define MATCH_PREFIX2(s1, s2, l) (strncasecmp(s1, s2, l) == 0)

// s1 must must be longer than s2 (suffix)
#define MATCH_SUFFIX(s1, s2, l) (strncmp((s1) + (l - strlen(s2)), s2, strlen(s2)) == 0)

struct PLTypeCheck {
    const char* prefix;
    const char* suffix;
    PLType t;
};

#define URL_TC(p) {p, NULL, PL_URL}

void QRCDecoder::ParsePayloadType(QRC *qrc)
{
    struct quirc_data data = qrc->data;
    size_t payloadLen = (size_t) data.payload_len;
    const char* payload = (const char*) data.payload;
    qrc->payload_type = PL_TEXT;

    static PLTypeCheck typeChecks[] = {
	
	/*{"MECARD:", NULL, PL_CONTACT},*/
	{"BEGIN:VCARD", "END:VCARD", PL_CONTACT},	
	URL_TC("http://"),
	URL_TC("https://"),
	URL_TC("ftp://"),
	URL_TC("ftps://"),
	URL_TC("file://"),
	{"BEGIN:VCALENDAR", NULL/*"END:VCALENDAR"*/, PL_CALENDAR},
	{"mailto:", NULL, PL_EMAIL},
	{"tel:", NULL, PL_PHONE_NUMBER},
	{"geo:", NULL, PL_GEO_LOCATION},
	{"sms:", NULL, PL_SMS},
	{"WIFI:", NULL, PL_WIFI}
    };

    const int count = sizeof(typeChecks) / sizeof(PLTypeCheck);

    for (int i = 0; i < count; i++)
    {
	PLTypeCheck tc = typeChecks[i];
	const char* p = tc.prefix;
	const char* s = tc.suffix;
	size_t pLen = (p ? strlen(p) : 0);
	size_t sLen = (s ? strlen(s) : 0);

	if ((payloadLen >= pLen) && MATCH_PREFIX(payload, p, pLen))
	{
	    if (s && (payloadLen >= sLen) && !MATCH_SUFFIX(payload, s, sLen)) continue;

	    qrc->payload_type = tc.t;
	    break;
	}
    }
}

const char* QRCDecoder::ParseAndEncodePayload(QRC *qrc, int &outLen, const char* outEnc)
{
    struct quirc_data data = qrc->data;
    size_t payloadLen = (size_t) data.payload_len;
    char* payload = (char*) data.payload;

    ParsePayloadType(qrc);

    if (!AllocateCachedPayload((payloadLen + 1)*4)) return NULL;

    const char* cs =  GetEncodingString(data.eci);
 
    if (data.data_type == QUIRC_DATA_TYPE_BYTE)
    {
	// could be text.
	TRY_CONVERT_PAYLOAD(cs)
	// maybe binary ? try convert to hex string.
	qrc->payload_type = PL_BINARY;
#ifdef __SYMBIAN32__
	
	PAYLOAD2HEX()
#endif
    }

    else if (data.data_type == QUIRC_DATA_TYPE_KANJI)
    {
	TRY_CONVERT_PAYLOAD("SHIFT-JIS")
	TRY_CONVERT_PAYLOAD(cs)
    }

    if ( !strcasecmp(cs, "UTF-8") && !strcasecmp(outEnc, cs)) return payload; // no conversation required
   

    TRY_CONVERT_PAYLOAD(cs)
    return NULL;
}


void QRCDecoder::SetError(const char* error)
{
    memset(m_error, 0, sizeof(m_error));   
    if (error) strncpy(m_error, error, sizeof(m_error)-1);
}

void QRCDecoder::SetError(char* error)
{
    SetError((const char*)error);
}

const char* QRCDecoder::GetError()
{
    if (strlen(m_error) == 0) return NULL;
    return m_error;
}

const wchar_t* QRCDecoder::GetWError()
{
    const char* error = GetError();
    if (!error) return NULL;

    static wchar_t werror[512];
    memset(werror, 0, sizeof(werror));

    if (mbstowcs(werror, error, strlen(error)) == (size_t)-1)
    {
	return NULL;
    }
    return werror;
}


uint8_t QRCEncoder::QrcBuf[qrcodegen_BUFFER_LEN_MAX];

QRCEncoder::QRCEncoder()
{
    SetParams(qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX); 
}

void QRCEncoder::SetParams(int ecl, int minVersion, int maxVersion)
{
    m_qrcSize = 0;
    m_ecl = ecl;
    m_minVersion = minVersion;
    m_maxVersion = maxVersion;
}

int QRCEncoder::EncodeText(const char *text)
{

    uint8_t tempBuf[qrcodegen_BUFFER_LEN_MAX] = {0};

    bool ok = qrcodegen_encodeText(text, tempBuf, QRCEncoder::QrcBuf, static_cast<qrcodegen_Ecc>(m_ecl), m_minVersion, m_maxVersion, qrcodegen_Mask_AUTO, true);
   
    m_qrcSize = ok ? qrcodegen_getSize(QRCEncoder::QrcBuf) :0;
    return m_qrcSize;
}

int QRCEncoder::GetModule(int x, int y)
{
    if (m_qrcSize == 0) return -1;
    return qrcodegen_getModule(QRCEncoder::QrcBuf, x, y);
}

