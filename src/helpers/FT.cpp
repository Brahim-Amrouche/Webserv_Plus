
#include "webserv.hpp"

size_t FT::strlen(const char *str)
{
    if (!str)
        return 0;
    size_t i = 0;
    while (str[i])
        ++i;
    return i;
}

void *FT::memset(void *b, int c, size_t len)
{
    size_t	i = -1;
	while (++i < len)
		((unsigned char *)b)[i] = (unsigned char)c;
	return (b);
}

void *FT::memcpy(void *dst, const void *src, size_t n)
{
    unsigned char	*c_dst;
	unsigned char	*c_src;
	size_t	i;

	if (!dst && !src && n)
		return (dst);
	c_dst = (unsigned char *)dst;
	c_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		c_dst[i] = c_src[i];
		i++;
	}
	return (dst);
}

void	*FT::memmove(void *dst, const void *src, size_t len)
{
	char	*dest;
	char	*source;

	if (!dst && !src && len)
		return (dst);
	dest = (char *)dst;
	source = (char *)src;
	if (dest > source)
		while (len-- > 0)
			dest[len] = source[len];
	else
		FT::memcpy(dst, src, len);
	return (dst);
}

void *FT::memchr(const void *src, void *cmp, size_t n, size_t m)
{
	if (!src || !cmp || !n || !m)
		return NULL;
	unsigned char *str = (unsigned char *)src;
	unsigned char *c = (unsigned char *)cmp;
	size_t i = -1;
	size_t j = -1;
	while (++i < n)
	{
		j = 0;
		while (j < m)
		{
			if (str[i + j] != c[j])
			{
				i += j;
				break;
			}
			j++;
		}
		if (j == m)
			return (str + i);
	}
	return NULL;
}

bool	FT::strIsDigit(const string &str)
{
	size_t i = -1;
	while (++i < str.size())
		if (!isdigit(str[i]))
			return false;
	return true;
}

string FT::strToLowercase(const string &str)
{
	string new_str;
	for (size_t i = 0; i< str.size(); i++)
	{
		new_str.push_back(std::tolower(str[i]));
	}
	return new_str;
}

string &FT::discardLastSlash(string &str)
{
	if (str[str.size() - 1] == '/')
		str.erase(str.size() - 1);
	return str;
}