//-*- Mode: C++ -*-
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef ALIHLTTPCCATRACKLETCONSTRUCTOR_H
#define ALIHLTTPCCATRACKLETCONSTRUCTOR_H


#include "AliHLTTPCCADef.h"

/**
 * @class AliHLTTPCCATrackletConstructor
 *
 */
class AliHLTTPCCATrackletConstructor
{
  public:

    class   AliHLTTPCCASharedMemory
    {
        friend class AliHLTTPCCATrackletConstructor;
      public:
#if !defined(HLTCA_GPUCODE)
        AliHLTTPCCASharedMemory()
            : fItr0( 0 ), fItr1( 0 ), fNRows( 0 ), fUsedHits( 0 ), fMinStartRow( 0 ), fMaxStartRow( 0 ) {}

        AliHLTTPCCASharedMemory( const AliHLTTPCCASharedMemory& /*dummy*/ )
            : fItr0( 0 ), fItr1( 0 ), fNRows( 0 ), fUsedHits( 0 ), fMinStartRow( 0 ), fMaxStartRow( 0 ) {}
        AliHLTTPCCASharedMemory& operator=( const AliHLTTPCCASharedMemory& /*dummy*/ ) { return *this; }
#endif
      protected:
        uint4 fData[2][( 5000+5000+5000 )/4]; // temp memory
        int fItr0; // start track index
        int fItr1; // end track index
        int fNRows; // n rows
        int *fUsedHits;   // array of used hits
        int fMinStartRow; // min start row
        int fMinStartRow32[32]; // min start row for each thread in warp
        int fMaxStartRow; // max start row
        int fMaxStartRow32[32];// max start row for each thread in warp
    };

    class  AliHLTTPCCAThreadMemory
    {
        friend class AliHLTTPCCATrackletConstructor;
      public:
#if !defined(HLTCA_GPUCODE)
        AliHLTTPCCAThreadMemory()
            : fItr( 0 ), fFirstRow( 0 ), fLastRow( 0 ), fStartRow( 0 ), fEndRow( 0 ), fCurrIH( 0 ), fIsMemThread( 0 ), fGo( 0 ), fSave( 0 ), fCurrentData( 0 ), fStage( 0 ), fNHits( 0 ), fNMissed( 0 ), fLastY( 0 ), fLastZ( 0 ) {}

        AliHLTTPCCAThreadMemory( const AliHLTTPCCAThreadMemory& /*dummy*/ )
            : fItr( 0 ), fFirstRow( 0 ), fLastRow( 0 ), fStartRow( 0 ), fEndRow( 0 ), fCurrIH( 0 ), fIsMemThread( 0 ), fGo( 0 ), fSave( 0 ), fCurrentData( 0 ), fStage( 0 ), fNHits( 0 ), fNMissed( 0 ), fLastY( 0 ), fLastZ( 0 ) {}
        AliHLTTPCCAThreadMemory& operator=( const AliHLTTPCCAThreadMemory& /*dummy*/ ) { return *this; }
#endif
      protected:
        int fItr; // track index
        int fFirstRow;  // first row index
        int fLastRow; // last row index
        int fStartRow;  // first row index
        int fEndRow;  // first row index
        int fCurrIH; // indef of the current hit
        bool fIsMemThread; // is the thread used for memory taken
        bool fGo; // do fit/searching flag
        bool fSave; // save flag
        bool fCurrentData; // index of the current memory array
        int fStage; // reco stage
        int fNHits; // n track hits
        int fNMissed; // n missed hits during search
        float fLastY; // Y of the last fitted cluster
        float fLastZ; // Z of the last fitted cluster
    };

    GPUd() static int NThreadSyncPoints() { return 4 + 159*2 + 1 + 1; }

    GPUd() static void Thread( int nBlocks, int nThreads, int iBlock, int iThread,
                               int iSync, AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r,
                               AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam );

    GPUd() static void Step0
    ( int nBlocks, int nThreads, int iBlock, int iThread,
      AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam );
    GPUd() static void Step1
    ( int nBlocks, int nThreads, int iBlock, int iThread,
      AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam );
    GPUd() static void Step2
    ( int nBlocks, int nThreads, int iBlock, int iThread,
      AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam );

    GPUd() static void ReadData( int iThread, AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, int iRow );

    GPUd() static void UpdateTracklet
    ( int nBlocks, int nThreads, int iBlock, int iThread,
      AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam, int iRow );

    GPUd() static void StoreTracklet
    ( int nBlocks, int nThreads, int iBlock, int iThread,
      AliHLTTPCCASharedMemory &s, AliHLTTPCCAThreadMemory &r, AliHLTTPCCATracker &tracker, AliHLTTPCCATrackParam &tParam );

    GPUd() static bool SAVE() { return 1; }

#if defined(HLTCA_GPUCODE)
    GPUhd() static int NMemThreads() { return 128; }
#else
    GPUhd() static int NMemThreads() { return 1; }
#endif

};


#endif
