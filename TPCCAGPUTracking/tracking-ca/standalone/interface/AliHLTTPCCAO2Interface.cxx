// **************************************************************************
// This file is property of and copyright by the ALICE HLT Project          *
// ALICE Experiment at CERN, All rights reserved.                           *
//                                                                          *
// Primary Authors: Sergey Gorbunov <sergey.gorbunov@kip.uni-heidelberg.de> *
//                  Ivan Kisel <kisel@kip.uni-heidelberg.de>                *
//                  for The ALICE HLT Project.                              *
//                                                                          *
// Permission to use, copy, modify and distribute this software and its     *
// documentation strictly for non-commercial purposes is hereby granted     *
// without fee, provided that the above copyright notice appears in all     *
// copies and that both the copyright notice and this permission notice     *
// appear in the supporting documentation. The authors make no claims       *
// about the suitability of this software for any purpose. It is            *
// provided "as is" without express or implied warranty.                    *
//                                                                          *
//***************************************************************************

#include "AliHLTTPCCAO2Interface.h"
#include "AliHLTTPCCAStandaloneFramework.h"

AliHLTTPCCAO2Interface::AliHLTTPCCAO2Interface() : fInitialized(false), fHLT(NULL)
{
}

AliHLTTPCCAO2Interface::~AliHLTTPCCAO2Interface()
{
	Deinitialize();
}

int AliHLTTPCCAO2Interface::Initialize(const char* options)
{
	if (fInitialized) return(1);
	fHLT = &AliHLTTPCCAStandaloneFramework::Instance(-1);
	if (fHLT == NULL) return(1);

	/*hlt.ExitGPU(); //Possible additional options, not used now
	hlt.SetGPUDebugLevel(DebugLevel, &CPUOut, &GPUOut);
	hlt.SetEventDisplay(eventDisplay);
	hlt.SetRunMerger(merger);
	hlt.InitGPU(sliceCount, cudaDevice)
	hlt.SetGPUTracker(RUNGPU);
	hlt.SetHighQPtForward(1./0.1);
	hlt.SetNWays(nways);*/

	fHLT->SetSettings();
	fHLT->SetGPUTrackerOption("HelperThreads", 0);
	fHLT->SetGPUTrackerOption("GlobalTracking", 1);

	fInitialized = true;
	return(0);
}

void AliHLTTPCCAO2Interface::Deinitialize()
{
	if (fInitialized)
	{
		fHLT->Merger().Clear();
		fHLT->Merger().SetGPUTracker(NULL);
		fHLT->ExitGPU();
		fHLT = NULL;
	}
	fInitialized = false;
}

int AliHLTTPCCAO2Interface::RunTracking(const AliHLTTPCCAClusterData* inputClusters, const AliHLTTPCGMMergedTrack* &outputTracks, int &nOutputTracks, const unsigned int* &outputTrackClusterIDs)
{
	if (!fInitialized) return(1);
	fHLT->SetExternalClusterData((AliHLTTPCCAClusterData*) inputClusters);
	fHLT->ProcessEvent();
	outputTracks = fHLT->Merger().OutputTracks();
	nOutputTracks = fHLT->Merger().NOutputTracks();
	outputTrackClusterIDs = fHLT->Merger().OutputClusterIds();
	return(0);
}

void AliHLTTPCCAO2Interface::Cleanup()
{
	
}