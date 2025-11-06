#include <ft_maki.h>

// char *ft_fgets(char *str, int size, T_FT_FILE *stream) {
//   int i = 0;
//   while (i < size - 1) {
//     if (stream->buf_pos >= stream->buf_len) {
//       ft_ssize_t n = ft_read(stream->fd, stream->buffer, FT_BUFSIZ);
//       if (n <= 0) {
//         if (n == 0 || stream->eof)
//           break;
//         stream->error = 1;
//         return (FT_NULL);
//       }
//       stream->file_pos += n;
//       stream->buf_pos = 0;
//       stream->buf_len = n;
//     }

//     char c = stream->buffer[stream->buf_pos++];
//     str[i++] = c;

//     if (c == '\n')
//       break; // Stop at newline

//     if (stream->buf_pos == stream->buf_len && i < size - 1) {
//       str[i] = '\0'; // Handle partial line
//       return (str);
//     }
//   }
//   str[i] = '\0'; // Null-terminate string
//   return ((i > 0) ? str : FT_NULL);
// }

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