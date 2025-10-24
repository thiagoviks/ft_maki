## 🚀 Quick Start

### Installation
```bash
git clone https://github.com/thiagoviks/ft_maki.git
cd ft_maki
sudo make install
```

### Hello World
```c
#include <ft_maki.h>

int main(void) {
    ft_printf("Hello from ft_maki!\n");
    return 0;
}
```

**Compile:**
```bash
clang hello.c -o hello -lft_maki
./hello
```

### Memory Allocation Example
```c
#include <ft_maki.h>

int main(void) {
    // Allocate memory
    char *str = ft_malloc(100);
    ft_strcpy(str, "ft_maki allocator");
    
    ft_printf("Allocated string: %s\n", str);
    ft_printf("String length: %zu\n", ft_strlen(str));
    
    // Free memory
    ft_free(str);
    return 0;
}
```

### Buffered File I/O Example
```c
#include <ft_maki.h>

int main(void) {
    // Write to file
    T_FT_FILE *file = ft_fopen("output.txt", "w");
    ft_fprintf(file, "Line 1: %d\n", 42);
    ft_fprintf(file, "Line 2: %s\n", "ft_maki");
    ft_fclose(file);
    
    // Read from file
    file = ft_fopen("output.txt", "r");
    char buffer[100];
    while (ft_fgets(buffer, sizeof(buffer), file)) {
        ft_printf("Read: %s", buffer);
    }
    ft_fclose(file);
    
    return 0;
}
```

### Working with Lists
```c
#include <ft_maki.h>

int main(void) {
    // Create linked list
    t_list *head = ft_lstnew(ft_strdup("first"));
    ft_lstadd_back(&head, ft_lstnew(ft_strdup("second")));
    ft_lstadd_back(&head, ft_lstnew(ft_strdup("third")));
    
    // Print list
    ft_printf("List size: %d\n", ft_lstsize(head));
    ft_lstiter(head, ft_lst_print_str_wrapper);
    
    // Clean up
    ft_lstclear(&head, ft_lst_del);
    return 0;
}
```

### Advanced: Direct Syscalls
```c
#include <ft_maki.h>

int main(void) {
    const char *msg = "Direct syscall write!\n";
    
    // Write directly using syscall wrapper
    ft_write(FT_STDOUT_FILENO, msg, ft_strlen(msg));
    
    // Get current time
    t_timeval tv;
    ft_gettimeofday(&tv, FT_NULL);
    ft_printf("Seconds since epoch: %ld\n", tv.ft_tv_sec);
    
    return 0;
}
```

### Compilation Options

**Basic compilation:**
```bash
clang program.c -o program -lft_maki
```

**With debugging symbols:**
```bash
clang -g program.c -o program -lft_maki
```

**With optimizations:**
```bash
clang -O3 program.c -o program -lft_maki
```

**Performance profiling:**
```bash
clang -g program.c -o program -lft_maki
valgrind --tool=callgrind ./program
kcachegrind callgrind.out.*
```

---

**Ready to explore?** Check out the [Core Features](#⚙️-core-features) section below to see everything ft_maki can do.

## 📖 The Story

ft_maki started as a response to 42 School's piscine challenge, where implementing 
libc functions from scratch is essential. After the experience, I decided to go deeper — not just implementing required functions, but building a complete, production-ready system programming framework.

Over 6 months, ft_maki grew from basic string functions to a full libc replacement 
with custom memory allocation, buffered I/O, and serves as the foundation for higher-level projects like ft_matplotlib, ft_mini_numpy, and ft_mini_sql.

**Philosophy:** Understand systems by building them. No black boxes, no magic — just C, syscalls, and deep technical work.

## 🎯 Why ft_maki?

**Learning by rebuilding:** Understanding systems from first principles by implementing libc functionality without black boxes.

**Performance control:** Direct syscall interface and custom memory allocation strategies for predictable behavior.

**Foundation for advanced projects:** Powers ft_matplotlib, ft_mini_numpy, ft_mini_sql, ft_mini_pandas, ft_scipy, ft_tqdm and other data science tools written purely in C.

**No external dependencies:** Everything from malloc to printf built from scratch using only Linux syscalls.


## 🧠 ft_maki — A Modular System Programming Framework in C

ft_maki is a low-level system programming framework written in C.
It reimplements essential parts of the C standard library (libc), provides a custom memory allocator, I/O subsystem, and builds the foundation for higher-level modules such as ft_printf, ft_matplotlib, and ft_mini_numpy.

Designed for performance, control, and extensibility, it is built from scratch without relying on external libraries — interfacing directly with syscalls, custom heap management, and buffered I/O.

### 📂 Complete Module List

**Memory Management**
- Custom allocator (brk/mmap based)
- Block coalescing and splitting
- Memory visualization tools

**I/O System**
- Buffered FILE streams (fopen, fread, fwrite, fflush)
- Formatted output (printf family with precision)
- Direct syscall wrappers (read, write, open, close)

**String Operations**
- Complete string.h (strcpy, strcmp, strdup, split, etc.)
- String manipulation (trim, join, substr)
- Multi-base number parsing (atoi_base, atoi_auto)

**Data Structures**
- Generic linked lists (map, filter, foreach)
- List wrappers for common operations

**System Interface**
- Signal handling (sigaction wrapper)
- Time operations (gettimeofday, time)
- Error handling (errno, perror, strerror with i18n)

**Character & Math**
- Complete ctype.h (isalpha, isdigit, isspace, etc.)
- Number conversion (atoi, atof, itoa)
- Float utilities (isnan, isinf)


## ⚙️ Core Features
## 🧮 Custom ft_printf

Reimplemented formatted output system using internal buffer management

Inline helper functions for string, memory, and numeric operations

Optimized I/O path (ft_write faster than libc write)

Benchmarked via valgrind --tool=callgrind and kcachegrind

## 💾 Memory Allocator (ft_malloc)

Custom allocator using brk / mmap depending on allocation size

Block splitting, merging, and re-use (coalescing)


## 🧰 Internal Utilities

ft_memset, ft_memcpy, ft_strlen, ft_strdup, etc.

All core functions implemented without the standard C library

Carefully designed as static inline for performance


🧩 Design Decisions
1. Static Inline Functions

All helper routines used exclusively by ft_printf or similar modules are declared static inline.
This prevents symbol pollution, improves instruction locality, and reduces call overhead.

// Technical Decision:
// Internal functions are declared `static inline` to enable compiler inlining
// and to ensure that internal logic remains self-contained within the module.
// This choice avoids external linkage overhead and improves cache efficiency.

2. Internal Function Centralization

Common system-level utilities (ft_memcpy, ft_memset, ft_isnan, ft_isinf)
are centralized within ft_utils.c to promote code reuse and uniformity
across the ft_printf, allocator, and numerical modules.

3. Direct Syscall Layer

ft_write and ft_read wrap the Linux syscalls directly, bypassing libc buffering.
This ensures deterministic performance and behavior during benchmarking.


## 🚧 Roadmap

- [x] Core libc functions (malloc, printf, strings, I/O)
- [x] Buffered FILE streams
- [x] Custom memory allocator with visualization
- [ ] **Futex-based mutex implementation** (in progress)
- [ ] Thread-safe FILE operations
- [ ] fseek/ftell implementation
- [ ] Advanced memory allocator strategies (buddy, slab)
- [ ] Network socket wrappers (TCP/IP integration)

🧰 License

MIT License © Maki — 2025
Educational and experimental use encouraged.