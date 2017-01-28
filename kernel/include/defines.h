#ifdef I686
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 96
#define PHYS_BITS 39
#define VIRT_BITS 48
#define PAGE_SIZE 2097152
#else
#ifdef X86_64
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 96
#define PHYS_BITS 32
#define VIRT_BITS 32
#define PAGE_SIZE 4096
#else
#ifdef _3DS9
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 30
#define PHYS_BITS 32
#define VIRT_BITS 32
#define PAGE_SIZE 4096
#else
#error "Unknown arch!"
#endif
#endif
#endif
