#include <stdbool.h>

typedef struct CommonFeatures {
    bool fpu; bool vme; bool de; bool pse; bool tsc; bool msr;
    bool pae; bool mce; bool cx8; bool apic; bool sep; bool mtrr; 
    bool pge; bool mca; bool cmov; bool pat; bool pse36; bool psn;
    bool clfsh; bool ds; bool acpi; bool mmx; bool fxsr; bool sse;
    bool sse2; bool ss; bool htt; bool tm; bool ia64; bool pbe;
    bool sse3; bool pclmulqdq; bool dtes64; bool monitor; bool dscpl;
    bool vmx; bool smx; bool est; bool tm2; bool ssse3; bool cnxtid;
    bool sdbg; bool fma; bool cx16; bool xtpr; bool pdcm; bool pcid;
    bool dca; bool sse41; bool sse42; bool x2apic; bool movbe; 
    bool popcnt; bool tscdeadline; bool aes; bool xsave; bool osxsave;
    bool avx; bool f16c; bool rdrnd; bool hypervisor;
} CommonFeatures;

typedef struct ExtendedFeatures {
    bool avx2; bool vaes; bool vpclmulqdq;
} ExtendedFeatures;