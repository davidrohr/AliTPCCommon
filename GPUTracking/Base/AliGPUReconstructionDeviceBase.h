#ifndef ALIGPURECONSTRUCTIONDEVICEBASE_H
#define ALIGPURECONSTRUCTIONDEVICEBASE_H

#include "AliGPUReconstructionCPU.h"
#include <pthread.h>
#include "AliGPUReconstructionHelpers.h"
#include "AliGPUChain.h"

#if !(defined(__CINT__) || defined(__ROOTCINT__) || defined(__CLING__) || defined(__ROOTCLING__) || defined(G__ROOT))
extern template class AliGPUReconstructionKernels<AliGPUReconstructionCPUBackend>;
#endif

class AliGPUReconstructionDeviceBase : public AliGPUReconstructionCPU
{
public:
	virtual ~AliGPUReconstructionDeviceBase() override;

	const AliGPUParam* DeviceParam() const {return &mDeviceConstantMem->param;}
	virtual int GetMaxThreads() override;

protected:
	AliGPUReconstructionDeviceBase(const AliGPUSettingsProcessing& cfg);
    
	virtual int InitDevice() override;
	virtual int InitDevice_Runtime() = 0;
	virtual int ExitDevice() override;
	virtual int ExitDevice_Runtime() = 0;

	virtual const AliGPUTPCTracker* CPUTracker(int iSlice) {return &workers()->tpcTrackers[iSlice];}

	virtual int GPUDebug(const char* state = "UNKNOWN", int stream = -1) override = 0;
	virtual void TransferMemoryInternal(AliGPUMemoryResource* res, int stream, deviceEvent* ev, deviceEvent* evList, int nEvents, bool toGPU, void* src, void* dst) override = 0;
	virtual void WriteToConstantMemory(size_t offset, const void* src, size_t size, int stream = -1, deviceEvent* ev = nullptr) override = 0;
	
	virtual int StartHelperThreads() override;
	virtual int StopHelperThreads() override;
	virtual void RunHelperThreads(int (AliGPUReconstructionHelpers::helperDelegateBase::* function)(int, int, AliGPUReconstructionHelpers::helperParam*), AliGPUReconstructionHelpers::helperDelegateBase* functionCls, int count) override;
	virtual int HelperError(int iThread) const override {return fHelperParams[iThread].fError;}
	virtual int HelperDone(int iThread) const override {return fHelperParams[iThread].fDone;}
	virtual void WaitForHelperThreads() override;
	virtual void ResetHelperThreads(int helpers) override;
	void ResetThisHelperThread(AliGPUReconstructionHelpers::helperParam* par);

	void ReleaseGlobalLock(void* sem);

	static void* helperWrapper_static(void* arg);
	void* helperWrapper(AliGPUReconstructionHelpers::helperParam* par);
	
	int fDeviceId = -1; //Device ID used by backend
	
#ifdef GPUCA_TIME_PROFILE
	unsigned long long int fProfTimeC, fProfTimeD; //Timing
#endif

	AliGPUReconstructionHelpers::helperParam* fHelperParams = nullptr; //Control Struct for helper threads

	int fNSlaveThreads = 0;	//Number of slave threads currently active
};

#endif
