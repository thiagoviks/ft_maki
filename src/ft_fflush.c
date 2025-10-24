#include "../include/ft_maki.h"

// Global list of all open T_FT_FILE streams
static T_FT_FILE *open_files = FT_NULL;

// Add to global list
static void register_file(T_FT_FILE *fp) {
  fp->next = open_files;
  open_files = fp;
}

// Predefined streams
T_FT_FILE *ft_stdout = FT_NULL;
T_FT_FILE *ft_stdin =  FT_NULL;
T_FT_FILE *ft_stderr = FT_NULL;

// Init std streams
static void ft_init_std_streams() {
  static int initalized = 0;
  if (initalized)
    return;
  initalized = 1;

  ft_stdout = ft_malloc(sizeof(T_FT_FILE));
  ft_stdin = ft_malloc(sizeof(T_FT_FILE));

  if (ft_stdout) {
    ft_stdout->fd = FT_STDOUT_FILENO;
    ft_stdout->buf_pos = 0;
    ft_stdout->buf_len = 0;
    ft_stdout->mode = FT_MODE_WRITE;
    ft_stdout->buf_mode = FT_IOLBF;
    ft_stdout->error = 0;
    ft_stdout->eof = 0;
    register_file(ft_stdout);
  }

  if (ft_stdin) {
    ft_stdin->fd = FT_STDIN_FILENO;
    ft_stdin->buf_pos = 0;
    ft_stdin->buf_len = 0;
    ft_stdin->mode = FT_MODE_READ;
    ft_stdin->buf_mode = FT_IOFBF;
    ft_stdin->error = 0;
    ft_stdin->eof = 0;
    register_file(ft_stdin);
  }
}

// Remove from global list
static void unregister_file(T_FT_FILE *fp) {
  T_FT_FILE **curr = &open_files;
  while (*curr) {
    if (*curr == fp) {
      *curr = fp->next;
      break;
    }
    curr = &(*curr)->next;
  }
}

int ft_ferror(T_FT_FILE *fp) { return (fp ? fp->error : 1); }
int ft_feof(T_FT_FILE *fp) { return (fp ? fp->eof : 1); }
void ft_clearerr(T_FT_FILE *fp) {
  if (fp)
    fp->error = 0;
}

int ft_setvbuf(T_FT_FILE *fp, int mode) {
  if (!fp || mode < 0 || mode > 2)
    return -1;
  fp->buf_mode = mode;
  return (0);
}

// Open file
T_FT_FILE *ft_fopen(const char *filename, const char *mode) {
  T_FT_FILE *fp = ft_malloc(sizeof(T_FT_FILE));
  if (!fp)
    return (FT_NULL);

  int flags;
  if (ft_strcmp(mode, "w") == 0) {
    flags = FT_O_WRONLY | FT_O_CREAT | FT_O_TRUNC;
    fp->mode = FT_MODE_WRITE;
  } else if (ft_strcmp(mode, "a") == 0) {
    flags = FT_O_WRONLY | FT_O_CREAT | FT_O_APPEND;
    fp->mode = FT_MODE_WRITE;
  } else if (ft_strcmp(mode, "r") == 0) {
    flags = FT_O_RDONLY;
    fp->mode = FT_MODE_READ;
  } else {
    ft_free(fp);
    return (FT_NULL);
  }

  fp->fd = ft_open(filename, flags, 0644);
  if (fp->fd < 0) {
    ft_free(fp);
    return (FT_NULL);
  }

  fp->buf_pos = 0;
  fp->buf_len = 0;
  fp->error = 0;
  fp->eof = 0;
  fp->buf_mode = FT_IOFBF; // Default: full buffering

  register_file(fp);
  return (fp);
}

// Buffered write
int ft_fputc(char c, T_FT_FILE *fp) {
  ft_init_std_streams();
  if (!fp || fp->mode != FT_MODE_WRITE) {
    if (fp)
      fp->error = 1;
    return (FT_EOF);
  }

  if (fp->buf_mode == FT_IONBF) {
    // Immediately write
    ft_ssize_t written = ft_write(fp->fd, &c, 1);
    if (written != 1) {
      fp->error = 1;
      return (FT_EOF);
    }
    return ((unsigned char)c);
  }

  fp->buffer[fp->buf_pos++] = c;

  if ((fp->buf_mode == FT_IOLBF && c == '\n') || fp->buf_pos == FT_BUFSIZ) {
    return (ft_fflush(fp) == 0 ? (unsigned char)c : FT_EOF);
  }

  return ((unsigned char)c);
}

// Buffered read
int ft_fgetc(T_FT_FILE *fp) {
  ft_init_std_streams();
  if (!fp || fp->mode != FT_MODE_READ) {
    if (fp)
      fp->error = 1;
    return (FT_EOF);
  }

  if (fp->buf_pos >= fp->buf_len) {
    ft_ssize_t n = ft_read(fp->fd, fp->buffer, FT_BUFSIZ);
    if (n < 0) {
      fp->error = 1;
      return (FT_EOF);
    }
    if (n == 0) {
      fp->eof = 1;
      return (FT_EOF);
    }
    fp->buf_len = n;
    fp->buf_pos = 0;
  }

  return ((unsigned char)fp->buffer[fp->buf_pos++]);
}

// Flush buffer
int ft_fflush(T_FT_FILE *fp) {
  ft_init_std_streams();
  if (fp == FT_NULL) {
    // Flush all open output files
    T_FT_FILE *curr = open_files;
    int status = 0;
    while (curr) {
      if (curr->mode == FT_MODE_WRITE) {
        if (ft_fflush(curr) == FT_EOF)
          status = FT_EOF;
      } else if (curr->mode == FT_MODE_READ) {
        curr->buf_pos = 0;
        curr->buf_len = 0;
      }
      curr = curr->next;
    }
    return (status);
  }

  if (fp->mode == FT_MODE_WRITE && fp->buf_pos > 0) {
    ft_size_t total = 0;
    while (total < fp->buf_pos) {
      ft_ssize_t written =
          ft_write(fp->fd, fp->buffer + total, fp->buf_pos - total);
      if (written < 0) {
        fp->error = 1;
        return (FT_EOF);
      }
      total += written;
    }
    fp->buf_pos = 0;
  } else if (fp->mode == FT_MODE_READ) {
    // Discard input buffer
    fp->buf_pos = 0;
    fp->buf_len = 0;
  }

  return (0);
}

// Close file
int ft_fclose(T_FT_FILE *fp) {
  if (!fp)
    return (FT_EOF);

  ft_fflush(fp);
  unregister_file(fp);

  int ret = ft_close(fp->fd);
  ft_free(fp);
  return (ret == 0 ? 0 : FT_EOF);
}

int ft_fileno(T_FT_FILE *fp) {
  if (!fp) {
    ft_errno = FT_EINVAL;
    return -1;
  }
  if (fp->fd < 0) {
    fp->error = 1;
    ft_errno = FT_EBADF;
    return -1;
  }
  return fp->fd;
}