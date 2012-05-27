// DirectMidi.cpp: implementation of the CDirectMidi class.

//Copyright Ian Parberry, 2000
//Last updated October 12, 2000

//This code is adapted from the DirectMusic Tutorial 1 that
//shipped with the DirectX 7.0a SDK. What I've done is 
//encapsulate it in a class, fix the numerous typos, and
//tidy it up.

//To use this, your game must be multithreaded. In Visual C++
//6.0, go to the Project menu, select Settings, select the C++
//tab, select Code Generation from the pulldown edit box,
//then select Multithreaded from the Use Run-time Library
//pull-down edit box.

//Include dxguid.lib in the linker settings.

#ifdef MIDIEXPER

#include <direct.h> //for _getcwd
#include "DirectMidi.h"

//DirectMusic needs wide characters - here's a conversion macro
#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar(CP_ACP,\
        MB_PRECOMPOSED,y,-1,x,_MAX_PATH);


CDirectMidi::CDirectMidi(){ //constructor
  //default values for member variables
  m_bInitialized=FALSE; //uninitialized
  m_pPerf=NULL; //performance
  m_pLoader=NULL; //loader
  m_pMIDIseg=NULL; //midi segment
  m_pSegState=NULL; //segment state
  //start DirectMusic MIDI player
  if(FAILED(CoInitialize(NULL)))return; //init COM
  if(FAILED(CreatePerformance()))return; //create performance
  if(FAILED(m_pPerf->Init(NULL,NULL,NULL)))return; //init it
  if(FAILED(m_pPerf->AddPort(NULL)))return; //add default port
  if(FAILED(CreateLoader()))return; //create MIDIloader
  //OK, we succeeded
  m_bInitialized=TRUE;
}

CDirectMidi::~CDirectMidi(){ //destructor
  if(!m_bInitialized)return; //bail if not ready
  Stop(); //stop music (paranoia)
  //unload instruments
  m_pMIDIseg->SetParam(GUID_Unload,-1,0,0,(void*)m_pPerf);
  //release the segment.
  m_pMIDIseg->Release();
  //close down and release the performance object.
  m_pPerf->CloseDown(); m_pPerf->Release();
  // release the loader object.
  m_pLoader->Release();
  //release COM
  CoUninitialize();
}

BOOL CDirectMidi::CreatePerformance(void){ //create performance
  return SUCCEEDED(CoCreateInstance(
    CLSID_DirectMusicPerformance,NULL,CLSCTX_INPROC, 
    IID_IDirectMusicPerformance2,(void**)&m_pPerf));
}

BOOL CDirectMidi::CreateLoader(void){
  return SUCCEEDED(CoCreateInstance(CLSID_DirectMusicLoader,
    NULL,CLSCTX_INPROC,IID_IDirectMusicLoader,
    (void**)&m_pLoader));
}

BOOL CDirectMidi::LoadMIDISegment(char* szMidiFileName){ 
//load MIDI segment
  DMUS_OBJECTDESC ObjDesc; 
  //get current folder
  char szDir[_MAX_PATH];
  if(_getcwd(szDir,_MAX_PATH)==NULL)return FALSE;
  //make wide version of current folder
  WCHAR wszDir[_MAX_PATH];
  MULTI_TO_WIDE(wszDir,szDir);
  //make wide version of midi file name 
  WCHAR wszMidiFileName[_MAX_PATH];
  MULTI_TO_WIDE(wszMidiFileName,szMidiFileName);
  //send loader to current folder
  HRESULT hr=m_pLoader->SetSearchDirectory(
    GUID_DirectMusicAllTypes,wszDir,FALSE);
  if(FAILED(hr))return FALSE; //bail if failed
  //describe segment object type
  ObjDesc.guidClass=CLSID_DirectMusicSegment;
  ObjDesc.dwSize=sizeof(DMUS_OBJECTDESC);
  wcscpy(ObjDesc.wszFileName,wszMidiFileName);
  ObjDesc.dwValidData=DMUS_OBJ_CLASS|DMUS_OBJ_FILENAME;
  m_pLoader->GetObject(&ObjDesc,
    IID_IDirectMusicSegment2,(void**)&m_pMIDIseg);
  m_pMIDIseg->SetParam(GUID_StandardMIDIFile,
    -1,0,0,(void*)m_pPerf);
  m_pMIDIseg->SetParam(GUID_Download,-1,0,0,(void*)m_pPerf);
  return TRUE;
}

void CDirectMidi::Load(char* filename){ //load file
  if(!m_bInitialized)return; //bail if not ready
  //release old music
  if(m_pMIDIseg){ //if there is old music, flush it
    m_pMIDIseg->Release(); m_pMIDIseg=NULL;
  }
  //load the new
  LoadMIDISegment(filename);
}

void CDirectMidi::Play(){ //play loaded music
  if(!m_bInitialized)return; //bail if not ready
  if(m_pMIDIseg&&m_pPerf){ //no bad pointers
    m_pMIDIseg->SetRepeats(0xFFFFFFFF); //repeat lotsa times
    m_pPerf->PlaySegment(m_pMIDIseg,0,0,&m_pSegState); //play
  }
}

void CDirectMidi::Stop(){ //stop playing music
  if(!m_bInitialized)return; //bail if not ready
  if(m_pPerf)m_pPerf->Stop(NULL,NULL,0,0); //stop
}

BOOL CDirectMidi::IsPlaying(){ //TRUE if playing
  if(!m_bInitialized)return FALSE; //bail if not ready
  if(m_pPerf) //if loaded
    return m_pPerf->IsPlaying(m_pMIDIseg,NULL)==S_OK;
  else return FALSE;
}

void CDirectMidi::Toggle(){ //toggle stop/play
  if(!m_bInitialized)return; //bail if not ready
  if(m_pPerf) //if loaded
    if(IsPlaying())Stop(); else Play(); //toggle
}

#endif //MIDIEXPER