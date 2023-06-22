#include "driver.h"

_Function_class_(DRIVER_UNLOAD) _IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_ static void
DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    dprintf("[Driver] DriverUnload\n");
}

template < ULONG tag >
class TestClass
{
    int value = 0;
public:
    TestClass(int value_ = 0) : value(value_){};
};

template < ULONG tag >
void test()
{

#ifdef __clang__
#pragma data_seg(".data")
#endif
    static int cnt { [](int val) { return val; }(tag) };
    int *p_cnt = &cnt;

    using TestClass = TestClass<tag>;
    // 以下用llvm-msvc-kernelmode 2.6 开发驱动,必定会发生页故障, 
    //但是wdk驱动开发工具是不会发生的,
    static TestClass testClass1(tag+100);

    static  TestClass testClass2(tag + 200);

    static  TestClass testClass4 {[](int val) { return val; }(tag)};
#ifdef __clang__
    static thread_local int thr_cnt = [](auto val) { return val; }(tag + 1000);
    int *p_thr_cnt = &thr_cnt;
    static thread_local int thr_cnt2 = tag + 1000;
    int *p_thr_cnt2 = &thr_cnt2;

    static thread_local TestClass testClass3(tag + 300);
#endif

#ifdef __clang__
#pragma data_seg()
#endif
}


void
func()
{
    static int cnt = 0;
    PVOID p_cnt = &cnt;
    auto pMdl = IoAllocateMdl(p_cnt, PAGE_SIZE, FALSE, FALSE, NULL);

    MmBuildMdlForNonPagedPool(pMdl);

    // Ëø¶¨Ò³Ãæ
    MmProbeAndLockPages(pMdl, KernelMode, IoModifyAccess);

    PVOID pLockedBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);

    ProbeForWrite(pLockedBuffer, PAGE_SIZE - 16, 8);

    ProbeForWrite(p_cnt, PAGE_SIZE - 16, 8);
}

_Function_class_(DRIVER_INITIALIZE) _IRQL_requires_same_ _IRQL_requires_(PASSIVE_LEVEL)
extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    // Set callback functions
    DriverObject->DriverUnload = DriverUnload;

    dprintf("[Driver] DriverEntry\n");

    test<128>();
    
    func();

    dprintf("[Driver] I am alive\n");

    return STATUS_SUCCESS;
}
