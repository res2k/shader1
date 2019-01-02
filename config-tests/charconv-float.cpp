#include <charconv>

int main()
{
  char buf[16];
  std::to_chars (buf, buf+16, 1.23f);
  return 0;
}
