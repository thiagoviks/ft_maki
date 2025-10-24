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

🧩 Build Instructions
sudo make install

//test.c
#include <ft_maki.h>

int main(void){
    ft_printf("Hello from ft_maki");
    return (0);
}

to compile:
clang test.c -o test -lft_maki

Optional: Run detailed CPU profiling
valgrind --tool=callgrind ./test
kcachegrind callgrind.out.*


🧪 Example Benchmark Output

--- Benchmark Results ---
printf:     19.070 s
ft_printf:  17.504 s
Speedup:    1.09x

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