
#include <cstddef>

class FT
{
    private :
        FT();
    public:
        static void	*memset(void *b, int c, size_t len);
        static void	*memcpy(void *dst, const void *src, size_t n);
        static void *memmove(void *dst, const void *src, size_t len);
        static void *memchr(const void *src, void *chr, size_t n, size_t m);
        static size_t  strlen(const char *str);
        static bool    strIsDigit(const string &str);
        static string  strToLowercase(const string &str);
        static string &discardLastSlash(string &str);
};


