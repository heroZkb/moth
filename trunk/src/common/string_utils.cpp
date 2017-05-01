#include <ctype.h>
#include <limits>
#include <stdarg.h>
#include "string_utils.h"
#include "tokener.h"


// UTILS_NS_BEGIN

// 快速字符串转换成整数模板通用函数
// str: 需要被转换的字符串
// dst: 转换后的结果
// max_length: 该整数类型对应的字符串的最多字符个数，不包括结尾符
// converted_length: 需要转换的字符串长度，如果为0则表示转换整个字符串
// ignored_zero: 是否忽略开头的0
// return: 如果转换成功返回true, 否则返回false
template <typename IntType>
static bool fast_string2int(const char* str, IntType& dst, uint8_t max_length, uint8_t converted_length, bool ignored_zero)
{
    bool negative = false;
    const char* tmp_str = str;
    if (NULL == str)
	{
		return false;
    }

    // 处理负数
    if ('-' == tmp_str[0])
    {
        // 负数
        negative = true;
        ++tmp_str;
    }

    // 处理空字符串
    if ('\0' == tmp_str[0])
	{
		return false;
    }

    // 处理0打头的
    if ('0' == tmp_str[0])
    {
        // 如果是0开头，则只能有一位数字
        if (('\0' == tmp_str[1]) || (1 == converted_length))
        {
            dst = 0;
            return true;
        }
        else
        {
            if (!ignored_zero)
			{
				return false;
            }
			
            for (;;)
            {
                ++tmp_str;
                if (tmp_str - str > max_length-1)
				{
					return false;
                }
				
                if (*tmp_str != '0')
				{
					break;
                }
            }
			
            if ('\0' == *tmp_str)
            {
                dst = 0;
                return true;
            }
        }
    }

    // 检查第一个字符
    if (('0' > *tmp_str) || ('9' < *tmp_str))
	{
		return false;
    }
	
    dst = (*tmp_str - '0');

    while ((0 == converted_length) || (tmp_str - str < converted_length - 1))
    {
        ++tmp_str;
        if ('\0' == *tmp_str)
		{
			break;
        }
		
        if (tmp_str - str > max_length - 1)
		{
			return false;
        }

        if (('0' > *tmp_str) || ('9' < *tmp_str))
		{
			return false;
        }

        dst = dst * 10;
        dst += (*tmp_str - '0');
    }

	// 负数
    if (negative)
    {
        dst = -dst;
    }
	
    return true;
}

void StringUtils::remove_last(std::string& source, char c)
{
    std::string::size_type pos = source.rfind(c);
    if (pos + 1 != source.length())
    {
        source.erase(pos);
    }
}

void StringUtils::remove_last(std::string& source, const std::string& sep)
{
    std::string::size_type pos = source.rfind(sep);
    if (pos != std::string::npos)
    {
        source.erase(pos);
    }
}

void StringUtils::to_upper(char* source)
{
    char* tmp_source = source;
    while ('\0' != *tmp_source)
    {
        if (('a' <= *tmp_source) && ('z' >= *tmp_source))
        {
            *tmp_source += 'A' - 'a';
        }

        ++tmp_source;
    }
}

void StringUtils::to_lower(char* source)
{
    char* tmp_source = source;
    while ('\0' != *tmp_source)
    {
        if (('A' <= *tmp_source) && ('Z' >= *tmp_source))
        {
            *tmp_source += 'a' - 'A';
        }

        ++tmp_source;
    }
}

void StringUtils::to_upper(std::string& source)
{
    char* tmp_source = (char *)source.c_str();
    to_upper(tmp_source);
}

void StringUtils::to_lower(std::string& source)
{
    char* tmp_source = (char *)source.c_str();
    to_lower(tmp_source);
}

// 判断字符是否为空格或TAB符(\t)或回车符(\r)或换行符(\n)
bool StringUtils::is_space(char c)
{
    return (' ' == c) || ('\t' == c) || ('\r' == c) || ('\n' == c);
}

void StringUtils::trim(char* source)
{
    char* space = NULL;
    char* tmp_source = source;
	
    while (' ' == *tmp_source)
	{
		++tmp_source;
    }

    for (;;)
    {
        *source = *tmp_source;
        if ('\0' == *tmp_source)
        {
            if (NULL != space)
            {
                *space = '\0';
            }
			
            break;
        }
        else if (is_space(*tmp_source))
        {
            if (NULL == space)
            {
                space = source;
            }
        }
        else
        {
            space = NULL;
        }

        ++source;
        ++tmp_source;
    }
}

void StringUtils::trim_left(char* source)
{
    char* tmp_source = source;
    while (is_space(*tmp_source)) ++tmp_source;

    for (;;)
    {
        *source = *tmp_source;
        if ('\0' == *tmp_source)
		{
			break;
        }

        ++source;
        ++tmp_source;
    }
}

void StringUtils::trim_right(char* source)
{
    char* space = NULL;
    char* tmp_source = source;

    for (;;)
    {
        if ('\0' == *tmp_source)
        {
            if (space != NULL)
            {
                *space = '\0';
            }
			
            break;
        }
        else if (is_space(*tmp_source))
        {
            if (NULL == space)
            {
                space = tmp_source;
            }
        }
        else
        {
            space = NULL;
        }

        ++tmp_source;
    }
}

void StringUtils::trim(std::string& source)
{
    trim_left(source);
    trim_right(source);
}

void StringUtils::trim_left(std::string& source)
{
    // 不能直接对c_str()进行修改，因为长度发生了变化
    size_t length = source.length();
    char* tmp_source = new char[length + 1];
    PointGuard<char> guard(tmp_source, true);

    strncpy(tmp_source, source.c_str(), length);
    tmp_source[length] = '\0';

    trim_left(tmp_source);
    source = tmp_source;
}

void StringUtils::trim_right(std::string& source)
{
    // 不能直接对c_str()进行修改，因为长度发生了变化
    size_t length = source.length();
    char* tmp_source = new char[length + 1];
    PointGuard<char> guard(tmp_source, true);

    strncpy(tmp_source, source.c_str(), length);
    tmp_source[length] = '\0';

    trim_right(tmp_source);
    source = tmp_source;
}

bool StringUtils::string2int8(const char* source, int8_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, int8_t& dst, uint8_t converted_length, bool ignored_zero)
{
    int16_t value = 0;

    if (!string2int16(source, value, converted_length, ignored_zero))
	{
		return false;
    }
	
    if (value < std::numeric_limits<int8_t>::min() 
		|| value > std::numeric_limits<int8_t>::max())
	{
		return false;
    }
    
    dst = (int8_t)value;
	
    return true;
}

bool StringUtils::string2int16(const char* source, int16_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, int16_t& dst, uint8_t converted_length, bool ignored_zero)
{
    int32_t value = 0;

    if (!string2int32(source, value, converted_length, ignored_zero))
	{
		return false;
    }
	
    if (value < std::numeric_limits<int16_t>::min()
		|| value > std::numeric_limits<int16_t>::max())
	{
		return false;
    }

    dst = (int16_t)value;
	
    return true;
}

bool StringUtils::string2int32(const char* source, int32_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, int32_t& dst, uint8_t converted_length, bool ignored_zero)
{
    if (NULL == source)
	{
		return false;
    }

    long value;
    if (!fast_string2int<long>(source, value, sizeof("-2147483648") - 1, converted_length, ignored_zero))
	{
		return false;
    }
	
    if ((value < std::numeric_limits<int32_t>::min())
		|| (value > std::numeric_limits<int32_t>::max()))
	{
		return false;
    }

    dst = (int32_t)value;
	
    return true;
}

bool StringUtils::string2int64(const char* source, int64_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, int64_t& dst, uint8_t converted_length, bool ignored_zero)
{
    long long value;
    if (!fast_string2int<long long>(source, value, sizeof("-9223372036854775808")-1, converted_length, ignored_zero))
	{
		return false;
    }

    dst = (int64_t)value;
	
    return true;
}

bool StringUtils::string2uint8(const char* source, uint8_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, uint8_t& dst, uint8_t converted_length, bool ignored_zero)
{
    uint16_t value = 0;
	
    if (!string2uint16(source, value, converted_length, ignored_zero))
	{
		return false;
    }
	
    if (value > std::numeric_limits<uint8_t>::max())
	{
		return false;
    }

    dst = (uint8_t)value;
	
    return true;
}

bool StringUtils::string2uint16(const char* source, uint16_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, uint16_t& dst, uint8_t converted_length, bool ignored_zero)
{
    uint32_t value = 0;
	
    if (!string2uint32(source, value, converted_length, ignored_zero))
	{
		return false;
    }
	
    if (value > std::numeric_limits<uint16_t>::max())
	{
		return false;
    }

    dst = (uint16_t)value;
	
    return true;
}

bool StringUtils::string2uint32(const char* source, uint32_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, uint32_t& dst, uint8_t converted_length, bool ignored_zero)
{
    unsigned long value;
	
    if (!fast_string2int<unsigned long>(source, value, sizeof("4294967295") - 1, converted_length, ignored_zero))
	{
		return false;
    }

    dst = (uint32_t)value;
	
    return true;
}

bool StringUtils::string2uint64(const char* source, uint64_t& dst, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, dst, converted_length, ignored_zero);
}

bool StringUtils::string2int(const char* source, uint64_t& dst, uint8_t converted_length, bool ignored_zero)
{
    unsigned long long value;
	
    if (!fast_string2int<unsigned long long>(source, value, sizeof("18446744073709551615")-1, converted_length, ignored_zero))
	{
		return false;
    }

    dst = (uint64_t)value;
	
    return true;
}

std::string StringUtils::int16_tostring(int16_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(int16_t source)
{
    char str[sizeof("065535")]; // 0xFFFF
    
    snprintf(str, sizeof(str), "%d", source);
	
    return str;
}

std::string StringUtils::int32_tostring(int32_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(int32_t source)
{
    char str[sizeof("04294967295")]; // 0xFFFFFFFF
    
    snprintf(str, sizeof(str), "%d", source);
	
    return str;
}

std::string StringUtils::int64_tostring(int64_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(int64_t source)
{
    char str[sizeof("018446744073709551615")]; // 0xFFFFFFFFFFFFFFFF
    // snprintf(str, sizeof(str), "%"PRId64, source);
	#if __WORDSIZE==64
		snprintf(str, sizeof(str), "%ld", source);
	#else
		snprintf(str, sizeof(str), "%lld", source);
	#endif
	
    return str;
}

std::string StringUtils::uint16_tostring(uint16_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(uint16_t source)
{
    char str[sizeof("065535")]; // 0xFFFF
    
    snprintf(str, sizeof(str), "%u", source);
	
    return str;
}

std::string StringUtils::uint32_tostring(uint32_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(uint32_t source)
{
    char str[sizeof("04294967295")]; // 0xFFFFFFFF
    
    snprintf(str, sizeof(str), "%u", source);
	
    return str;
}

std::string StringUtils::uint64_tostring(uint64_t source)
{
    return int_tostring(source);
}

std::string StringUtils::int_tostring(uint64_t source)
{
    char str[sizeof("018446744073709551615")]; // 0xFFFFFFFFFFFFFFFF
#if __WORDSIZE==64
    snprintf(str, sizeof(str), "%lu", source);
#else
    snprintf(str, sizeof(str), "%llu", source);
#endif
    return str;
}

char* StringUtils::skip_spaces(char* buffer)
{
    char* iter = buffer;
    while (' ' == *iter)
	{
		++iter;
    }

    return iter;
}

const char* StringUtils::skip_spaces(const char* buffer)
{
    const char* iter = buffer;
    while (' ' == *iter)
	{
		++iter;
    }

    return iter;
}

uint32_t StringUtils::hash(const char *str, int len)
{
    uint32_t g = 0;
    uint32_t h = 0;
    const char *p = str;

    while (p < str + len)
    {
        h = (h << 4) + *p++;
        if ((g = (h & 0xF0000000)))
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }

    return h;
}

int StringUtils::fix_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int expected = fix_vsnprintf(str, size, format, ap);
    va_end(ap);

    return expected;
}

int StringUtils::fix_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int expected = vsnprintf(str, size, format, ap);

    if (expected < static_cast<int>(size))
    {
        return expected;
    }

    return static_cast<int>(size);
}

std::string StringUtils::path2filename(const std::string& path, const std::string& join_string)
{
    std::string filename;
    std::list<std::string> tokens;
    Tokener::split(&tokens, path, "/");

    if (!tokens.empty())
    {
        filename = tokens.front();
        tokens.pop_front();
    }

    while (!tokens.empty())
    {
        filename += join_string + tokens.front();
        tokens.pop_front();
    }

    return filename;
}

int StringUtils::chr_index(const char* str, char c) 
{
    const char* c_position = strchr(str, c);
    return (NULL == c_position) ? -1 : c_position - str;
}

int StringUtils::chr_rindex(const char* str, char c) 
{
    const char* c_position = strrchr(str, c);
    return (NULL == c_position) ? -1 : c_position - str;
}

std::string StringUtils::extract_dirpath(const char* filepath)
{
    std::string dirpath;
	
    int index = chr_rindex(filepath, '/');
	
    if (-1 != index)
    {
        dirpath.assign(filepath, index);
   }

    return dirpath;
}

// 解析出文件名
std::string StringUtils::extract_filename(const std::string& filepath)
{
    std::string filename;
    const char* slash_position = strrchr(filepath.c_str(), '/');
	
    if (NULL == slash_position)
    {
        filename = filepath;
    }
    else
    {
        filename.assign(slash_position + 1);
    }

    return filename;
}

const char* StringUtils::extract_filename(const char* filepath)
{
    const char* slash_position = strrchr(filepath, '/');
	
    if (NULL == slash_position)
    {
        return filepath;
    }

    return slash_position + 1;
}

std::string StringUtils::format_string(const char* format, ...)
{
    va_list ap;
    size_t size = 8192;
    ScopedArray<char> buffer(new char[size]);

    for (;;)
    {
        va_start(ap, format);

        // vsnprintf中的第二参数大小是要求包含结尾符的
        int expected = vsnprintf(buffer.get(), size, format, ap);

        va_end(ap);
        if (expected > -1 && expected < (int)size)
            break;

        /* Else try again with more space. */
        if (expected > -1)    /* glibc 2.1 */
        {
        	size = (size_t)expected + 1; /* precisely what is needed */
        }
        else           /* glibc 2.0 */
        {
        	size *= 2;  /* twice the old size */
        }

        buffer.reset(new char[size]);
    }

    return buffer.get();
}

bool StringUtils::is_numeric_string(const char* str)
{
    const char* p = str;

    while ('\0' != *p)
    {
        if (!(*p >= '0' && *p <= '9'))
        {
            return false;
        }

        ++p;
    }

    return true;
}

bool StringUtils::is_alphabetic_string(const char* str)
{
    const char* p = str;

    while ('\0' != *p)
    {
        if (!((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')))
        {
            return false;
        }

        ++p;
    }

    return true;
}

bool StringUtils::is_variable_string(const char* str)
{
    const char* p = str;

    while ('\0' != *p)
    {
        if ('0' <= *p && '9' >= *p)
        {
            ++p;
            continue;
        }
		
        if (('a' <= *p && 'z' >= *p) || ('A' <= *p && 'Z' >= *p))
        {
            ++p;
            continue;
        }
		
        if (('_' == *p) || ('-' == *p))
        {
            ++p;
            continue;
        }

        return false;
    }

    return true;
}

bool StringUtils::is_regex_string(const char* str)
{
    const char* p = str;

    while ('\0' != *p)
    {
        if ('0' <= *p && '9' >= *p)
        {
            ++p;
            continue;
        }
		
        if (('a' <= *p && 'z' >= *p) || ('A' <= *p && 'Z' >= *p))
        {
            ++p;
            continue;
        }
		
        if (('_' == *p) || ('-' == *p))
        {
            ++p;
            continue;
        }
		
        if (('\\' == *p) || ('/' == *p))
        {
            ++p;
            continue;
        }
		
        if (('+' == *p) || ('*' == *p))
        {
            ++p;
            continue;
        }
        if (('.' == *p) || (',' == *p))
        {
            ++p;
            continue;
        }
		
        if (('<' == *p) || ('>' == *p))
        {
            ++p;
            continue;
        }
		
        if (('{' == *p) || ('}' == *p))
        {
            ++p;
            continue;
        }
		
        if (('(' == *p) || (')' == *p))
        {
            ++p;
            continue;
        }
		
        if (('[' == *p) || (']' == *p))
        {
            ++p;
            continue;
        }
		
        if (('$' == *p) || ('%' == *p) || ('^' == *p) || ('?' == *p) || ('"' == *p) || (' ' == *p))
        {
            ++p;
            continue;
        }

        return false;
    }

    return true;
}

std::string StringUtils::remove_suffix(const std::string& filename)
{
    std::string::size_type pos = filename.find('.');
	
    if (pos == std::string::npos)
    {
        return filename;
    }
    else
    {
        return filename.substr(0, pos);
    }
}

std::string StringUtils::replace_suffix(const std::string& filepath, const std::string& new_suffix)
{
    std::string::size_type pos = filepath.find('.');
	
    if (pos == std::string::npos)
    {
        if (new_suffix.empty() || new_suffix == ".")
        {
            return filepath;
        }
        else
        {
            if ('.' == new_suffix[0])
            {
                return filepath + new_suffix;
            }
            else
            {
                return filepath + std::string(".") + new_suffix;
            }
        }
    }
    else
    {
        if (new_suffix.empty() || new_suffix == ".")
        {
            // 相当于删除了后缀
            return filepath.substr(0, pos);
        }
        else
        {
            if ('.' == new_suffix[0])
            {
                return filepath.substr(0, pos) + new_suffix;
            }
            else
            {
                return filepath.substr(0, pos) + std::string(".") + new_suffix;
            }
        }
    }
}

std::string StringUtils::to_hex(const std::string& source, bool lowercase)
{
    std::string hex;
    hex.resize(source.size()*2);

    char* hex_p = const_cast<char*>(hex.data());
    for (std::string::size_type i = 0; i < source.size(); ++i)
    {
        if (lowercase)
        {
            snprintf(hex_p, 3, "%02x", source[i]);
        }
        else
        {
            snprintf(hex_p, 3, "%02X", source[i]);
        }

        hex_p += 2;
    }

    *hex_p = '\0';
	
    return hex;
}

// 一个汉字,GBK编码时占2字节,UTF8编码时占3字节
std::string StringUtils::encode_url(const std::string& url, bool space2plus)
{
    return encode_url(url.c_str(), url.size(), space2plus);
}

std::string StringUtils::encode_url(const char* url, size_t url_length, bool space2plus)
{
    static char hex[] = "0123456789ABCDEF";
    std::string result(url_length * 3 + 1, '\0');

    int i = 0;
    while ('\0' != *url)
    {
        char c = *url++;

        if (' ' == c)
        {
            if (space2plus)
            {
                result[i++] = '+';
            }
            else
            {
                // 新标准将空格替换为加号+
                result[i + 0] = '%';
                result[i + 1] = '2';
                result[i + 2] = '0';
                i += 3;
            }
        }
        else if (('0' <= c && '9' >= c) ||
                 ('a' <= c && 'z' >= c) ||
                 ('A' <= c && 'Z' >= c) ||
                 ('-' == c) || ('_' == c) ||
                 ('.' == c) || ('~' == c))
        {
            // RFC 3986标准定义的未保留字符 (2005年1月)
            result[i++] = c;
        }
        else
        {
            // 有符号的c值可能是负数
            result[i + 0] = '%';
            result[i + 1] = hex[static_cast<unsigned char>(c) / 16];
            result[i + 2] = hex[static_cast<unsigned char>(c) % 16];
            i += 3;
        }
    }

    result.resize(i);
    return result;
}

std::string StringUtils::decode_url(const std::string& encoded_url)
{
    return decode_url(encoded_url.c_str(), encoded_url.size());
}

std::string StringUtils::decode_url(const char* encoded_url, size_t encoded_url_length)
{
    std::string result(encoded_url_length+1, '\0');

    int i = 0;
    while ('\0' != *encoded_url)
    {
        char c = *encoded_url++;

        if ('+' == c)
        {
            result[i++] = ' ';
        }
        else if (c != '%')
        {
            result[i++] = c;
        }
        else
        {
            if (!isxdigit(encoded_url[0]) ||
                !isxdigit(encoded_url[1]))
            {
                result[i++] = '%';
            }
            else
            {
                char hex[3];
                hex[0] = encoded_url[0];
                hex[1] = encoded_url[1];
                hex[2] = '\0';

                char x = strtol(hex, NULL, 16);
                result[i++] = x;
                encoded_url += 2;
            }
        }
    }

    result.resize(i);
	
    return result;
}

// CR: Carriage Return
// LF: Line Feed
void StringUtils::trim_CR(char* line)
{
    if (NULL != line)
    {
        size_t len = strlen(line);
        if ('\r' == line[len - 1])
        {
            line[len - 1] = '\0';
        }
    }
}

void StringUtils::trim_CR(std::string& line)
{
    std::string::size_type tail = line.size() - 1;
	
    if ('\r' == line[tail])
    {
        line.resize(tail);
    }
}

std::string StringUtils::char2hex(unsigned char c)
{
    static unsigned char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    std::string hex(2, '\0');

    hex[0] = hex_table[(c >> 4) & 0x0F];
    hex[1] = hex_table[c & 0x0F];
	
    return hex;
}

unsigned char StringUtils::hex2char(const std::string& hex)
{
    unsigned char c = 0;

    for (int i = 0; i < std::min<int>(hex.size(), 2); ++i)
    {
        unsigned char c1 = toupper(hex[i]); // ctype.h
        unsigned char c2 = (c1 >= 'A') ? (c1 - ('A' - 10)) : (c1 - '0');
        (c <<= 4) += c2;
    }

    return c;
}

// UTILS_NS_END
