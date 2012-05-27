// DirectMidi.h: interface for the CDirectMidi class.

//Copyright Ian Parberry, 2000
//Last updated October 12, 2000

#if !defined(AFX_DIRECTMIDI_H)
#define AFX_DIRECTMIDI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <Dmusicc.h>
#include <Dmusici.h>

class CDirectMidi{ //DirectMusic MIDI class
private:
  BOOL m_bInitialized; //TRUE if initialized correctly
  IDirectMusicPerformance* m_pPerf; //performance
  IDirectMusicLoader* m_pLoader; //loader
  IDirectMusicSegment* m_pMIDIseg; //midi segment
  IDirectMusicSegmentState* m_pSegState; //segment state
  BOOL CreatePerformance(void); //create performance
  BOOL CreateLoader(void); //create MIDI loader
  BOOL LoadMIDISegment(char* wszMidiFileName); //create segment
public:
  CDirectMidi(); //constructor
  virtual ~CDirectMidi(); //destructor
  void Load(char* filename); //load MIDI from file
  void Play(); //play loaded MIDI
  void Stop(); //stop playing MIDI
  BOOL IsPlaying(); //TRUE if playing
  void Toggle(); //toggle stop/play
};

#endif // !defined(AFX_DIRECTMIDI_H)
