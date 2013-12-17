// CCaptureVideo视频捕捉类头文件
/////////////////////////////////////////////////////////////////////
#if !defined CAPTURESOUND_H
#define CAPTURESOUND_H
/////////////////////////////////////////////////////////////////////
// CAPTURESOUND.h : header file
/////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <atlbase.h>
#include <windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <initguid.h>  // declares DEFINE_GUID to declare an EXTERN_C const.

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define NUM_REC_NOTIFICATIONS  16

// 30323449-0000-0010-8000-00AA00389B71       MEDIASUBTYPE_I420
DEFINE_GUID(MEDIASUBTYPE_I420,
			0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);


class CCaptureSound  
{

public:

	HRESULT InitDirectSound(GUID* pDeviceGuid=NULL );	//success return 0
	HRESULT RecordCapturedData(int&bufsize,char* buffer);	//success return 0
	HRESULT StartRecord();	//success return 0
	HRESULT StopRecord();	//success return 0
	//int EnumDevices(HWND hList);
	/*HRESULT SetAudioFormat(unsigned numChannels,
                              unsigned sampleRate,
                              unsigned bitsPerSample);*/

	CCaptureSound();
	virtual ~CCaptureSound();
	
private:
	LPDIRECTSOUNDCAPTURE       m_pDSCapture ;
	LPDIRECTSOUNDCAPTUREBUFFER m_pDSBCapture ;
	LPDIRECTSOUNDNOTIFY        m_pDSNotify ;
	GUID                       m_guidCaptureDevice;
	WAVEFORMATEX               m_wfxInput;
	HANDLE                     m_hNotificationEvent; 
	//BOOL                       m_abInputFormatSupported[20];
	DWORD                      m_dwCaptureBufferSize;
	DWORD                      m_dwNextCaptureOffset;
	DWORD                      m_dwNotifySize;

	
protected:
	HRESULT CreateCaptureBuffer();	//success return 0 ,S_OK
	HRESULT InitNotifications();	//success return 0


};

//INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName,
//                                     VOID* pContext );
#endif // !defined(AFX_CAPTURESOUND_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_)
