//-*- Mode: C++ -*-
// @(#) $Id$
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        * 
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef ALIHLTTPCCATRACK_H
#define ALIHLTTPCCATRACK_H

#include "AliHLTTPCCADef.h"
#include "AliHLTTPCCATrackParam.h"

/**
 * @class ALIHLTTPCCAtrack
 *
 * The class describes the [partially] reconstructed TPC track [candidate].
 * The class is dedicated for internal use by the AliHLTTPCCATracker algorithm.
 * The track parameters at both ends are stored separately in the AliHLTTPCCAEndPoint class
 */
class AliHLTTPCCATrack
{
 public:
#if !defined(HLTCA_GPUCODE)
  AliHLTTPCCATrack() :fAlive(0),fFirstHitID(0),fNHits(0), fParam(){}
  ~AliHLTTPCCATrack(){}
#endif

  GPUhd() Bool_t Alive()               const { return fAlive; }
  GPUhd() Int_t  NHits()               const { return fNHits; }
  GPUhd() Int_t  FirstHitID()          const { return fFirstHitID; }
  GPUhd() const AliHLTTPCCATrackParam &Param() const { return fParam; };
  
  GPUhd() void SetAlive( Bool_t v )               { fAlive=v; }
  GPUhd() void SetNHits( Int_t v )               { fNHits=v; }
  GPUhd() void SetFirstHitID( Int_t v )          { fFirstHitID=v; }
  GPUhd() void SetParam( AliHLTTPCCATrackParam v ){ fParam=v; };
  
private:
  
  Bool_t fAlive;       // flag for mark tracks used by the track merger
  Int_t  fFirstHitID; // index of the first track cell in the track->cell pointer array
  Int_t  fNHits;      // number of track cells
  AliHLTTPCCATrackParam fParam; // track parameters 
  
private:
  //void Dummy(); // to make rulechecker happy by having something in .cxx file

  //ClassDef(AliHLTTPCCATrack,1)
};

#endif