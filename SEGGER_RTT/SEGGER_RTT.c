#include "SEGGER_RTT.h"
#include <string.h>

typedef struct {
    char     acID[16];
    int      MaxNumUpBuffers;
    struct {
        const char *sName;
        char       *pBuffer;
        unsigned    SizeOfBuffer;
        unsigned    Cnt;
    } aUp[SEGGER_RTT_MAX_NUM_UP_BUFFERS];
} SEGGER_RTT_CB;

static SEGGER_RTT_CB _RTT;
static char _aUpBuffer[BUFFER_SIZE_UP];

void SEGGER_RTT_Init(void)
{
    static char _done;
    int i;
    if (_done) return;

    _RTT.acID[0] = 'S'; _RTT.acID[1] = 'E';
    _RTT.acID[2] = 'G'; _RTT.acID[3] = 'G';
    _RTT.acID[4] = 'E'; _RTT.acID[5] = 'R';
    _RTT.acID[6] = ' '; _RTT.acID[7] = 'R';
    _RTT.acID[8] = 'T'; _RTT.acID[9] = 'T';
    _RTT.acID[10] = '\0';

    _RTT.MaxNumUpBuffers = SEGGER_RTT_MAX_NUM_UP_BUFFERS;

    for (i = 0; i < SEGGER_RTT_MAX_NUM_UP_BUFFERS; i++) {
        _RTT.aUp[i].sName       = "Terminal";
        _RTT.aUp[i].pBuffer     = _aUpBuffer;
        _RTT.aUp[i].SizeOfBuffer = sizeof(_aUpBuffer);
        _RTT.aUp[i].Cnt         = 0;
    }
    memset(_aUpBuffer, 0, sizeof(_aUpBuffer));
    _done = 1;
}

unsigned SEGGER_RTT_Write(unsigned BufferIndex, const void *pBuffer, unsigned NumBytes)
{
    const char *p = (const char *)pBuffer;
    char *dst;
    unsigned i;

    if (BufferIndex >= (unsigned)_RTT.MaxNumUpBuffers) return 0;
    if (!pBuffer || NumBytes == 0) return 0;

    dst = _RTT.aUp[BufferIndex].pBuffer;
    if (NumBytes >= _RTT.aUp[BufferIndex].SizeOfBuffer)
        NumBytes = _RTT.aUp[BufferIndex].SizeOfBuffer;

    SEGGER_RTT_LOCK();
    for (i = 0; i < NumBytes; i++) dst[i] = p[i];
    _RTT.aUp[BufferIndex].Cnt += NumBytes;
    SEGGER_RTT_UNLOCK();

    return NumBytes;
}
