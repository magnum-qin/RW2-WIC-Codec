#pragma once

#include "Common.h"

class ClassFactory : public IClassFactory
{
public:
    ClassFactory();
    virtual ~ClassFactory();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IClassFactory methods
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    STDMETHOD(LockServer)(BOOL fLock) override;

private:
    LONG m_refCount;
};

// Global lock counter
extern LONG g_serverLocks;
