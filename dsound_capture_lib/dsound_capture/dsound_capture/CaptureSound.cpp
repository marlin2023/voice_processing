//-------------------------------------------------------------------
// CaptureSound.cpp
//-------------------------------------------------------------------
// CaptureSound.cpp: implementation of the CCaptureVideo class.
//
/////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "CaptureSound.h"
#include "dsound.h"
#include <stdio.h>

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCaptureSound::CCaptureSound()
{

	//COM Library Intialization
	HRESULT hr = CoInitialize(NULL);
	printf("hr = %x H \n" ,hr);
	if(FAILED(hr)) /*, COINIT_APARTMENTTHREADED)))*/
	{
		if(hr != RPC_E_CHANGED_MODE){
			printf("CoInitialize Failed!\r\n");
			CoUninitialize();
			return ;
		}

	}
	m_pDSCapture         = NULL;
	m_pDSBCapture        = NULL;
	m_pDSNotify          = NULL;
	m_guidCaptureDevice  = GUID_NULL;

	m_hNotificationEvent = CreateEvent( NULL, FALSE, FALSE, NULL ); //
}
CCaptureSound::~CCaptureSound()
{
    // Release DirectSound interfaces
    SAFE_RELEASE( m_pDSNotify );
    SAFE_RELEASE( m_pDSBCapture );
    SAFE_RELEASE( m_pDSCapture ); 
	CloseHandle( m_hNotificationEvent );

    // Release COM
    CoUninitialize();
}
//int CCaptureSound::EnumDevices(HWND hList)
//{
//
//	//// Enumerate the capture devices and place them in the combo box
// //   DirectSoundCaptureEnumerate( (LPDSENUMCALLBACK)DSoundEnumCallback,    //unresolved external symbol
// //                                (VOID*)hList );
//
// //   // Select the first device in the combo box
// //   ::SendMessage(hList, CB_SETCURSEL, 0, 0 );
//
//    return S_OK;
//}


//-----------------------------------------------------------------------------
//1----------> Name: InitDirectSound()
// Desc: Initilizes DirectSound
//-----------------------------------------------------------------------------
HRESULT CCaptureSound::InitDirectSound( GUID* pDeviceGuid )
{
    HRESULT hr;
    m_dwCaptureBufferSize = 0;
    m_dwNotifySize        = 0;

	// Initialize COM
    if( FAILED( hr = CoInitialize(NULL) ) )
	{
		printf("hr = %x H ," ,hr);

		if(hr != RPC_E_CHANGED_MODE){
			 printf( "CoInitialize failed ...FILE :%s ,LINE :%d.\n" ,__FILE__ ,__LINE__ );
			return hr ;
		}
	}

	//in here ,use DSDEVID_DefaultVoiceCapture 
	//if( FAILED( hr = DirectSoundCaptureCreate(&DSDEVID_DefaultVoiceCapture, &m_pDSCapture, NULL ) ) )  //DSDEVID_DefaultCapture?? DSDEVID_DefaultVoiceCapture
	if( FAILED( hr = DirectSoundCaptureCreate8(&DSDEVID_DefaultVoiceCapture, &m_pDSCapture, NULL ) ) )  //DSDEVID_DefaultCapture?? DSDEVID_DefaultVoiceCapture
	{
        printf("DirectSoundCaptureCreate failed\n");
		return hr;
	}
	printf("DirectSoundCaptureCreate success \n");
	 // wFormatTag, nChannels, nSamplesPerSec, mAvgBytesPerSec,
    // nBlockAlign, wBitsPerSample, cbSize
	m_wfxInput.wFormatTag= WAVE_FORMAT_PCM;
	m_wfxInput.nChannels=1;
	m_wfxInput.nSamplesPerSec=8000;
	m_wfxInput.wBitsPerSample=16;
	m_wfxInput.nBlockAlign = m_wfxInput.nChannels * ( m_wfxInput.wBitsPerSample / 8 );
    m_wfxInput.nAvgBytesPerSec = m_wfxInput.nBlockAlign * m_wfxInput.nSamplesPerSec;
	m_wfxInput.cbSize=0;

    return S_OK;
}

HRESULT CCaptureSound::CreateCaptureBuffer()
{

	HRESULT hr;
	DSCBUFFERDESC               dscbd;  // setup the DSBUFFERDESC structure
	
	// Set the notification size
    //m_dwNotifySize = MAX( 1024, m_wfxInput.nAvgBytesPerSec / 8 );
    //m_dwNotifySize -= m_dwNotifySize % m_wfxInput.nBlockAlign;
	m_dwNotifySize = 320;
	printf("m_dwNotifySize = %d \n" ,m_dwNotifySize);
    // Set the buffer sizes 
    m_dwCaptureBufferSize = m_dwNotifySize * NUM_REC_NOTIFICATIONS;



	printf("m_dwCaptureBufferSize = %d \n" ,m_dwCaptureBufferSize);
	//while(1);
	
	// Create the capture buffer
    ZeroMemory( &dscbd, sizeof(dscbd) );
	dscbd.dwSize = sizeof(DSCBUFFERDESC);
	dscbd.dwFlags = 0;
	dscbd.dwBufferBytes = m_dwCaptureBufferSize;
	dscbd.dwReserved = 0;
	dscbd.lpwfxFormat = &m_wfxInput;
	dscbd.dwFXCount = 0;
	dscbd.lpDSCFXDesc = NULL;
	
	if( FAILED( hr = m_pDSCapture->CreateCaptureBuffer( &dscbd, 	//???
		&m_pDSBCapture, 
		NULL ) ) )
        return hr;
	
    m_dwNextCaptureOffset = 0;
	
    if( FAILED( hr = InitNotifications() ) )  //set notifications
        return hr;
	
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitNotifications()
// Desc: Inits the notifications on the capture buffer which are handled
//       in WinMain()
//-----------------------------------------------------------------------------
HRESULT CCaptureSound::InitNotifications()
{

    HRESULT hr; 
	DSBPOSITIONNOTIFY    aPosNotify[ NUM_REC_NOTIFICATIONS + 1 ];  
	ZeroMemory( &aPosNotify, sizeof(DSBPOSITIONNOTIFY) * 
                               (NUM_REC_NOTIFICATIONS + 1) );

    if( NULL == m_pDSBCapture )
        return E_FAIL;

    // Create a notification event, for when the sound stops playing
    if( FAILED( hr = m_pDSBCapture->QueryInterface( IID_IDirectSoundNotify, 
                                                    (VOID**)&m_pDSNotify ) ) )
        return hr;

    // Setup the notification positions
    for( INT i = 0; i < NUM_REC_NOTIFICATIONS; i++ )
    {
        aPosNotify[i].dwOffset = (m_dwNotifySize * i) + m_dwNotifySize - 1;
        aPosNotify[i].hEventNotify = m_hNotificationEvent;             
    }
    
    // Tell DirectSound when to notify us. the notification will come in the from 
    // of signaled events that are handled in WinMain()
    if( FAILED( hr = m_pDSNotify->SetNotificationPositions( NUM_REC_NOTIFICATIONS, 
                                                            aPosNotify ) ) )
        return hr;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RecordCapturedData()
// Desc: Copies data from the capture buffer to the output buffer 
//-----------------------------------------------------------------------------
HRESULT CCaptureSound::RecordCapturedData(int&bufsize,char* buffer) 
{
    HRESULT hr;
    VOID*   pbCaptureData    = NULL;
    DWORD   dwCaptureLength;
    VOID*   pbCaptureData2   = NULL;
    DWORD   dwCaptureLength2;
    DWORD   dwReadPos;			//use this pointer
    DWORD   dwCapturePos;		//only use in function GetCurrentPosition
    LONG lLockSize;

    if( NULL == m_pDSBCapture )
        return S_FALSE;

    if( FAILED( hr = m_pDSBCapture->GetCurrentPosition( &dwCapturePos, &dwReadPos ) ) )
        return hr;

    lLockSize = dwReadPos - m_dwNextCaptureOffset;
    if( lLockSize < 0 )
        lLockSize += m_dwCaptureBufferSize;

	while(lLockSize<bufsize)
	{
		if (WaitForSingleObject(m_hNotificationEvent, INFINITE) != WAIT_OBJECT_0)
		{
			printf("wait fail");
			return S_FALSE;
		}
		
		if( FAILED( hr = m_pDSBCapture->GetCurrentPosition( &dwCapturePos, &dwReadPos ) ) )
			return hr;
		
		lLockSize = dwReadPos - m_dwNextCaptureOffset;
		if( lLockSize < 0 )
			lLockSize += m_dwCaptureBufferSize;
	}

	if(lLockSize>bufsize)
		lLockSize=bufsize;

    // Block align lock size so that we are always write on a boundary
	//lLockSize -= (lLockSize % m_dwNotifySize);

	//printf("lLockSize = %d \n" ,lLockSize);
    if( lLockSize == 0 )
        return S_FALSE;

    // Lock the capture buffer down
    if( FAILED( hr = m_pDSBCapture->Lock( m_dwNextCaptureOffset, lLockSize, //m_dwNextCaptureOffset is the offset of DSBCapture Buffer
                                          &pbCaptureData, &dwCaptureLength, 
                                          &pbCaptureData2, &dwCaptureLength2, 0L ) ) )
        return  hr;

    // copy the data into the buffer
	memcpy(buffer,pbCaptureData,dwCaptureLength);

    // Move the capture offset along
    m_dwNextCaptureOffset += dwCaptureLength; 
    m_dwNextCaptureOffset %= m_dwCaptureBufferSize; // Circular buffer

    if( pbCaptureData2 != NULL )
    {
		// copy the data into the buffer
		memcpy(buffer+dwCaptureLength,pbCaptureData2,dwCaptureLength2);

        // Move the capture offset along
        m_dwNextCaptureOffset += dwCaptureLength2; 
        m_dwNextCaptureOffset %= m_dwCaptureBufferSize; // Circular buffer
    }

    // Unlock the capture buffer
    m_pDSBCapture->Unlock( pbCaptureData,  dwCaptureLength, 
                           pbCaptureData2, dwCaptureLength2 );


    return S_OK;
}


//--->2
HRESULT CCaptureSound::StartRecord()
{
    HRESULT hr;
	// Create a capture buffer, and tell the capture 
	// buffer to start recording   
	if( FAILED( hr = CreateCaptureBuffer() ) )   //createCaptureBuffer
		return hr;
	
	if( FAILED( hr = m_pDSBCapture->Start( DSCBSTART_LOOPING ) ) )
		return hr;

	return S_OK;
}

HRESULT CCaptureSound::StopRecord()
{
    HRESULT hr;
	if( NULL == m_pDSBCapture )
		return S_OK;
	
	// Stop the buffer, and read any data that was not 
	// caught by a notification
	if( FAILED( hr = m_pDSBCapture->Stop() ) )
		return  hr;

	return S_OK;
}

///**********************************************************************************/
////-----------------------------------------------------------------------------
//// Name: DSoundEnumCallback()
//// Desc: Enumeration callback called by DirectSoundEnumerate
////-----------------------------------------------------------------------------
//INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName,
//                                  VOID* pContext )
//{
//
//    // Set aside static storage space for 20 audio drivers
//    static GUID  AudioDriverGUIDs[20];
//    static DWORD dwAudioDriverIndex = 0;
//
//    GUID* pTemp  = NULL;
//
//    if( pGUID )
//    {
//        if( dwAudioDriverIndex >= 20 )
//            return TRUE;
//
//        pTemp = &AudioDriverGUIDs[dwAudioDriverIndex++];
//        memcpy( pTemp, pGUID, sizeof(GUID) );
//    }
//
//    HWND hSoundDeviceCombo = (HWND)pContext;
//
//    // Add the string to the combo box
//    SendMessage( hSoundDeviceCombo, CB_ADDSTRING,
//                 0, (LPARAM) (LPCTSTR) strDesc );
//
//    // Get the index of the string in the combo box
//    INT nIndex = (INT)SendMessage( hSoundDeviceCombo, CB_FINDSTRING,
//                                   0, (LPARAM) (LPCTSTR) strDesc );
//
//    // Set the item data to a pointer to the static guid stored in AudioDriverGUIDs
//    SendMessage( hSoundDeviceCombo, CB_SETITEMDATA,
//                 nIndex, (LPARAM) pTemp );
//
//    return TRUE;
//}
