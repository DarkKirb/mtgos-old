#ifdef X86_64
# define SCREEN_WIDTH 128
# define SCREEN_HEIGHT 48
# define PHYS_BITS 39
# define VIRT_BITS 48
# define PAGE_SIZE 4096
#else
# ifdef I686
#  define SCREEN_WIDTH 128
#  define SCREEN_HEIGHT 48
#  define PHYS_BITS 32
#  define VIRT_BITS 32
#  define PAGE_SIZE 4096
# else
#  ifdef _3DS9
#   define SCREEN_WIDTH 40
#   define SCREEN_HEIGHT 15
#   define PHYS_BITS 32
#   define VIRT_BITS 32
#   define PAGE_SIZE 4096
#  else
#   ifdef _3DS11
#    define SCREEN_WIDTH 50
#    define SCREEN_HEIGHT 15
#    define PHYS_BITS 32
#    define VIRT_BITS 32
#    define PAGE_SIZE 4096
#   else
#    warning "Unknown arch!"
#    define SCREEN_WIDTH 1
#    define SCREEN_HEIGHT 1
#    define PHYS_BITS 1
#    define VIRT_BITS 1
#    define PAGE_SIZE 2
#   endif
#  endif
# endif
#endif
