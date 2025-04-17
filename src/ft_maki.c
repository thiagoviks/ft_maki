#include "../include/ft_maki.h"

static t_block *head = NULL;

static size_t align(size_t size)
{
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

ssize_t ft_write(int fd, const void *buf, size_t len)
{
    ssize_t ret;
    __asm__ volatile (
		"movq $1, %%rax\n\t"        // syscall number for write
		"movq %1, %%rdi\n\t"        // file descriptor
		"movq %2, %%rsi\n\t"        // buffer
		"movq %3, %%rdx\n\t"        // count
		"syscall\n\t"
		"movq %%rax, %0\n\t"
		: "=r" (ret)
		: "r" ((long)fd), "r" (buf), "r" ((long)len)
		: "%rax", "%rdi", "%rsi", "%rdx"
	);
    return ret;
}

ssize_t ft_read(int fd, void *buf, size_t count)
{
    ssize_t ret;
    asm volatile (
        "mov $0, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return ret;
}

int ft_open(const char *pathname, int flags, int mode)
{
    int ret;
    asm volatile (
        "mov $2, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(pathname), "S"(flags), "d"(mode)
        : "rcx", "r11", "memory"
    );
    return ret;
}

int ft_close(int fd)
{
    int ret;
    asm volatile (
        "mov $3, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(fd)
        : "rcx", "r11", "memory"
    );
    return ret;
}

void ft_exit(long code)
{
    asm volatile (
        "mov $60, %%rax\n"
        "mov %0, %%rdi\n"
        "syscall"
        : : "r"(code) : "rax", "rdi"
    );
    __builtin_unreachable();
}

size_t ft_strlen(const char *s)
{
    size_t len = 0;
    while (s[len])
        len++;
    return len;
}

void ft_putstr(const char *str)
{
    while (*str)
    {
        ft_write(1, str++, 1);
    }
}

char    *ft_strdup(const char *src)
{
    char *str;
    int i = 0;

    while (src[i] != '\0')
        i++;

    str = ft_malloc(sizeof(char) * (ft_strlen((char *)src) + 1));
    if (!str)
        return (NULL);
    i = 0;
    while (src[i])
    {
        str[i] = src[i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

static t_block *find_free_block(size_t size)
{
    t_block *current = head;
    while (current)
    {
        if (current->free && current->size >= size)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static t_block *request_space(size_t size)
{
    void *addr;
    t_block *block;

    if (size >= MMAP_THRESHOLD) {
        size_t total = sizeof(t_block) + size;

        asm volatile(
            "mov $9, %%rax\n"     // syscall number for mmap
            "mov $0, %%rdi\n"     // addr = NULL
            "mov %1, %%rsi\n"     // length
            "mov $0x22, %%rdx\n"  // PROT_READ | PROT_WRITE
            "mov $0x21, %%r10\n"  // MAP_PRIVATE | MAP_ANONYMOUS
            "mov $-1, %%r8\n"     // fd = -1
            "mov $0, %%r9\n"      // offset = 0
            "syscall\n"
            "mov %%rax, %0\n"
            : "=r"(addr)
            : "r"(total)
            : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
        );

        if (addr == (void *)-1)
        return NULL;

        block = (t_block *)addr;
        block->size = size;
        block->free = 0;
        block->is_mmap = 1;
        block->next = NULL;
        block->prev = NULL;

        return block;
    }
    // syscall brk
    void *old_brk;
    asm volatile(
        "mov $12, %%rax\n"
        "syscall\n"
        "mov %%rax, %0\n"
        : "=r"(old_brk)
        :
        : "rax"
    );

    void *new_brk = (char *)old_brk + sizeof(t_block) + size;

    void *result;
    asm volatile(
        "mov $12, %%rax\n"
        "mov %1, %%rdi\n"
        "syscall\n"
        "mov %%rax, %0\n"
        : "=r"(result)
        : "r"(new_brk)
        : "rax", "rdi"
    );
    if (result != new_brk)
        return NULL;

    block = (t_block *)old_brk;
    block->size = size;
    block->free = 0;
    block->is_mmap = 0;
    block->next = NULL;
    block->prev = NULL;

    if (!head)
        head = block;
    else {
        t_block *current = head;
        while (current->next)
            current = current->next;
        current->next = block;
        block->prev = current;
    }

    return block;
}

void *ft_malloc(size_t size) {
    if (size == 0) return NULL;

    size = align(size);
    t_block *block = find_free_block(size);

    if (block) {
        block->free = 0;
        return (void *)(block + 1);
    }

    block = request_space(size);
    if (!block)
        return NULL;

    return (void *)(block + 1);
}

static void coalesce(t_block *block) {
    if (block->next && block->next->free && !block->next->is_mmap) {
        block->size += sizeof(t_block) + block->next->size;
        block->next = block->next->next;
        if (block->next)
            block->next->prev = block;
    }
    if (block->prev && block->prev->free && !block->prev->is_mmap) {
        block->prev->size += sizeof(t_block) + block->size;
        block->prev->next = block->next;
        if (block->next)
            block->next->prev = block->prev;
    }
}

void ft_free(void *ptr)
{
    if (!ptr) return;
    t_block *block = ((t_block *)ptr) - 1;
    if (block->is_mmap)
    {
        // syscall: munmap
        asm volatile (
            "mov $11, %%rax\n"       // munmap
            "mov %0, %%rdi\n"
            "mov %1, %%rsi\n"
            "syscall\n"
            :
            : "r"(block), "r"(block->size + sizeof(t_block))
            : "rax", "rdi", "rsi"
        );
        return;
    }
    block->free = 1;
    coalesce(block);
}

// Custom memset implementation
void *ft_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;  // Treat the pointer as a byte pointer
    while (num--) {
        *p = (unsigned char)value;  // Set each byte to the given value
        p++;  // Move to the next byte
    }
    return ptr;  // Return the original pointer
}

// Custom memcpy implementation
void *ft_memcpy(void *dest, const void *src, size_t num) {
    unsigned char *d = dest;  // Destination pointer
    const unsigned char *s = src;  // Source pointer
    while (num--) {
        *d = *s;  // Copy each byte from source to destination
        d++;  // Move to the next byte in the destination
        s++;  // Move to the next byte in the source
    }
    return dest;  // Return the destination pointer
}


// Custom calloc implementation
void *ft_calloc(size_t nmemb, size_t size) {
    size_t total_size = nmemb * size; // Calculate total size required
    void *ptr = ft_malloc(total_size); // Allocate memory using malloc
    if (ptr) ft_memset(ptr, 0, total_size); // Initialize the allocated memory to 0
    return ptr;
}

// Custom realloc implementation
void *ft_realloc(void *ptr, size_t size) {
    if (!ptr) return ft_malloc(size); // If the pointer is NULL, behave like malloc()
    if (size == 0) { ft_free(ptr); return NULL; } // If size is 0, free the memory and return NULL

    t_block *block = (t_block*)ptr - 1;
    if (block->size >= size) return ptr; // If the current block is already large enough, return the same pointer

    // If the block cannot be expanded, allocate a new block with the requested size
    void *new_ptr = ft_malloc(size);
    if (!new_ptr) return NULL; // If malloc fails, return NULL

    // Copy the data from the old block to the new block
    ft_memcpy(new_ptr, ptr, block->size);
    ft_free(ptr); // Free the old block
    return new_ptr; // Return the new block
}

void	ft_reverse(char *str, int length)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = length - 1;
	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

int	ft_itoa_str(int num, char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	is_negative = 0;
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	if (num == 0)
		str[i++] = '0';
	while (num > 0)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		str[i++] = '-';
	str[i] = '\0';
	ft_reverse(str, i);
	return (i);
}

void	ft_map(int *arr, size_t size, int (*func)(int))
{
	size_t	i;
	int		result;
	char	output[20];
	int		length;

	i = 0;
	while (i < size)
	{
		result = func(arr[i]);
		length = ft_itoa_str(result, output);
		ft_write(1, output, length);
		ft_write(1, "\n", 1);
		i++;
	}
}