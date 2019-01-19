#include <charconv>
#include <string.h>

int main()
{
  const char* conv_string = "42";
  int x;
  std::from_chars (conv_string, conv_string + strlen (conv_string), x);
  return 0;
}
