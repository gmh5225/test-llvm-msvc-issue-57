#include "driver.h"

_Function_class_(DRIVER_UNLOAD) _IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_ static void
DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    dprintf("[Driver] DriverUnload\n");
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

    func();

    dprintf("[Driver] I am alive\n");

    return STATUS_SUCCESS;
}
