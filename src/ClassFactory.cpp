#include "../include/ClassFactory.h"
#include "../include/RW2Decoder.h"

extern LONG g_objectCount;

ClassFactory::ClassFactory() : m_refCount(1)
{
    InterlockedIncrement(&g_serverLocks);
}

ClassFactory::~ClassFactory()
{
    InterlockedDecrement(&g_serverLocks);
}

STDMETHODIMP ClassFactory::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_INVALIDARG;

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppvObject = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ClassFactory::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) ClassFactory::Release()
{
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0)
    {
        delete this;
    }
    return refCount;
}

STDMETHODIMP ClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_INVALIDARG;

    *ppvObject = nullptr;

    if (pUnkOuter != nullptr)
        return CLASS_E_NOAGGREGATION;

    RW2Decoder* pDecoder = new (std::nothrow) RW2Decoder();
    if (pDecoder == nullptr)
        return E_OUTOFMEMORY;

    HRESULT hr = pDecoder->QueryInterface(riid, ppvObject);
    pDecoder->Release();

    return hr;
}

STDMETHODIMP ClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement(&g_serverLocks);
    else
        InterlockedDecrement(&g_serverLocks);

    return S_OK;
}
