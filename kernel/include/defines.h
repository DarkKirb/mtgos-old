#ifdef X86_64
# define SCREEN_WIDTH 64
# define SCREEN_HEIGHT 48
# define PHYS_BITS 39
# define VIRT_BITS 48
# define PAGE_SIZE 4096
#else
# ifdef I686
#  define SCREEN_WIDTH 64
#  define SCREEN_HEIGHT 48
#  define PHYS_BITS 32
#  define VIRT_BITS 32
#  define PAGE_SIZE 4096
# else
#  ifdef _3DS9
#   define SCREEN_WIDTH 20
#   define SCREEN_HEIGHT 15
#   define PHYS_BITS 32
#   define VIRT_BITS 32
#   define PAGE_SIZE 4096
#  else
#   ifdef _3DS11
#    define SCREEN_WIDTH 25
#    define SCREEN_HEIGHT 15
#    define PHYS_BITS 32
#    define VIRT_BITS 32
#    define PAGE_SIZE 4096
#   else
#    error "Unknown arch!"
#   endif
#  endif
# endif
#endif
