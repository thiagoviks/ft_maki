#include "../include/ft_maki.h"

static void discard_buffer(T_FT_FILE *stream);

static void discard_buffer(T_FT_FILE *stream) {
  stream->buf_len = 0;
  stream->buf_pos = 0;
  stream->buf_dirty = 0;
}

ft_off_t ft_lseek(int fd, ft_off_t offset, int whence) {
  /* This invokes the kernel via software interrupt/syscall instruction */
  return (ft_syscall(FT_SYS_lseek, fd, offset, whence));

  /* On x86_64, this compiles to something like:
   * mov rax, 8        ; syscall number
   * mov rdi, fd       ; arg1
   * mov rsi, offset   ; arg2
   * mov rdx, whence   ; arg3
   * syscall           ; trap to kernel
   */
}

/**
 * ft_fseek - Reposition stream position
 * @stream: File stream pointer
 * @offset: Offset value
 * @whence: How to interpret offset (FT_SEEK_SET, FT_SEEK_CUR, FT_SEEK_END)
 *
 * Returns: 0 on success, -1 on error
 *
 * IMPORTANT: Unlike lseek which returns the new position, fseek returns 0/-1
 */
int ft_fseek(T_FT_FILE *stream, ft_off_t offset, int whence) {
  ft_off_t new_pos;
  ft_off_t current_actual_pos;

  /* Validate stream pointer */
  if (stream == FT_NULL) {
    ft_errno = FT_EINVAL;
    return (-1);
  }

  /* Clear EOF flag - seeking clears EOF */
  stream->eof = 0;

  /*
   * CRITICAL: Handle buffered data before seeking
   *
   * If we have dirty ft_write data, we MUST flush it first.
   * If we have buffered read data, we need to account for it.
   */

  if (stream->buf_dirty) {
    /* Flush any pending writes */
    if (ft_fflush(stream) == -1) {
      return (-1);
    }
  }

  /*
   * Calculate the actual current file position
   *
   * The file descriptor's position might be ahead of where we logically are
   * because we've read ahead into the buffer.
   */
  if (stream->mode == FT_MODE_READ && stream->buf_len > 0) {
    /* Actual position = buffer start + how far we've read into it */
    current_actual_pos = stream->file_pos + stream->buf_pos;
  } else {
    /* For ft_write mode or empty buffer, file_pos is accurate */
    current_actual_pos = stream->file_pos;
  }

  /*
   * Special optimization for FT_SEEK_CUR with small offsets
   *
   * If seeking within the current buffer, we can just adjust buf_pos
   * without doing an actual lseek() syscall!
   */
  if (whence == FT_SEEK_CUR && stream->mode == FT_MODE_READ) {
    ft_off_t target_pos = current_actual_pos + offset;

    /* Check if target is within current buffer */
    if (target_pos >= stream->file_pos &&
        target_pos < stream->file_pos + (ft_off_t)stream->buf_len) {

      /* Yes! Just adjust buffer position */
      stream->buf_pos = target_pos - stream->file_pos;
      return (0);
    }
  }

  /*
   * Can't optimize - need to do actual seek
   * Discard buffer since it will be invalid after seek
   */
  discard_buffer(stream);

  /*
   * Adjust whence for FT_SEEK_CUR
   *
   * Since we've discarded the buffer, we need to seek from where
   * the file descriptor actually is, not where we appeared to be.
   */
  if (whence == FT_SEEK_CUR) {
    /* The actual seek needs to account for our logical position */
    offset = current_actual_pos + offset;
    whence = FT_SEEK_SET;
  }

  /*
   * Perform the actual lseek system call
   */
  new_pos = ft_lseek(stream->fd, offset, whence);

  if (new_pos == (ft_off_t)-1) {
    stream->error = 1;
    return (-1);
  }

  /*
   * Update our tracking of file position
   */
  stream->file_pos = new_pos;
  stream->buf_pos = 0;
  stream->buf_len = 0;

  /*
   * Reset operation tracking for read/ft_write transitions
   */
  stream->last_op_write = 0;

  return (0);
}

/**
 * ft_ftell - Get current file position
 * @stream: File stream pointer
 *
 * Returns: Current file position, or -1 on error
 */
ft_off_t ft_ftell(T_FT_FILE *stream) {
  ft_off_t current_pos;

  if (stream == FT_NULL) {
    ft_errno = FT_EINVAL;
    return (-1);
  }

  /* Calculate logical position based on buffer state */
  if (stream->buf_dirty) {
    /* ft_write mode: position is where we'll ft_write next */
    current_pos = stream->file_pos + stream->buf_len;
  } else if (stream->buf_len > 0) {
    /* Read mode: position is buffer start + how far we've read */
    current_pos = stream->file_pos + stream->buf_pos;
  } else {
    /* No buffer data */
    current_pos = stream->file_pos;
  }

  return (current_pos);
}

/**
 * ft_rewind - Reset file position to beginning
 * @stream: File stream pointer
 *
 * Equivalent to fseek(stream, 0, FT_SEEK_SET) but also clears error flag
 */
void ft_rewind(T_FT_FILE *stream) {
  if (stream == FT_NULL) {
    return;
  }

  ft_fseek(stream, 0, FT_SEEK_SET);
  stream->error = 0; /* rewind also clears error indicator */
}

ft_size_t ft_fread(void *ptr, ft_size_t size, ft_size_t nmemb,
                   T_FT_FILE *stream) {
  ft_size_t bytes_to_read = size * nmemb;
  ft_size_t bytes_read = 0;
  char *dest = (char *)ptr;

  while (bytes_to_read > 0) {
    /* Need to fill buffer? */
    if (stream->buf_pos >= stream->buf_len) {
      ft_ssize_t n = ft_read(stream->fd, stream->buffer, FT_BUFSIZ);
      if (n <= 0) {
        if (n == 0)
          stream->eof = 1;
        break;
      }
      stream->buf_len = n;
      stream->buf_pos = 0;
      stream->file_pos = ft_lseek(stream->fd, 0, FT_SEEK_CUR) - n;
    }

    /* Copy from buffer */
    ft_size_t available = stream->buf_len - stream->buf_pos;
    ft_size_t to_copy = (bytes_to_read < available) ? bytes_to_read : available;

    ft_memcpy(dest, stream->buffer + stream->buf_pos, to_copy);
    stream->buf_pos += to_copy;
    dest += to_copy;
    bytes_read += to_copy;
    bytes_to_read -= to_copy;
  }

  return bytes_read / size;
}