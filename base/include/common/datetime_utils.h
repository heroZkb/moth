#ifndef _DATETIME_UTILS_H_
#define _DATETIME_UTILS_H_

#include <time.h>
#include <string>
#include "define.h"

UTILS_NS_BEGIN

class CDatetimeUtils
{
public:
    // 判断是否为同一天
    static bool is_same_day(time_t t1, time_t t2);

    // 判断指定年份是否为闰年
    static bool is_leap_year(int year);
    
    // 将time_t值转换成类似于20160114这样的整数
    static uint32_t time2date(time_t t);

    // 得到当前日期和时间，返回格式由参数format决定，默认为: YYYY-MM-DD HH:SS:MM
	// datetime_buffer:用来存储当前日期和时间的缓冲区
	// datetime_buffer_size:datetime_buffer的字节大小，不应当于小“YYYY-MM-DD HH:SS:MM”
    static void get_current_datetime(char* datetime_buffer, size_t datetime_buffer_size, const char* format = "%04d-%02d-%02d %02d:%02d:%02d");
    static std::string get_current_datetime(const char* format = "%04d-%02d-%02d %02d:%02d:%02d");

	// 得到当前日期，返回格式由参数format决定，默认为: YYYY-MM-DD
	// date_buffer: 用来存储当前日期的缓冲区
	// date_buffer_size: date_buffer的字节大小，不应当于小“YYYY-MM-DD”
    static void get_current_date(char* date_buffer, size_t date_buffer_size, const char* format = "%04d-%02d-%02d");
    static std::string get_current_date(const char* format = "%04d-%02d-%02d");

	// 得到当前时间，返回格式由参数format决定，默认为: HH:SS:MM
	// time_buffer: 用来存储当前时间的缓冲区
	// time_buffer_size: time_buffer的字节大小，不应当于小“HH:SS:MM”
    static void get_current_time(char* time_buffer, size_t time_buffer_size, const char* format = "%02d:%02d:%02d");
    static std::string get_current_time(const char* format = "%02d:%02d:%02d");

    // 得到当前日期和时间结构
    static void get_current_datetime_struct(struct tm* current_datetime_struct);

    // 日期和时间
    static void to_current_datetime(struct tm* current_datetime_struct, char* datetime_buffer, size_t datetime_buffer_size, const char* format = "%04d-%02d-%02d %02d:%02d:%02d");
    static std::string to_current_datetime(struct tm* current_datetime_struct, const char* format = "%04d-%02d-%02d %02d:%02d:%02d");

    // 仅日期
    static void to_current_date(struct tm* current_datetime_struct, char* date_buffer, size_t date_buffer_size, const char* format = "%04d-%02d-%02d");
    static std::string to_current_date(struct tm* current_datetime_struct, const char* format = "%04d-%02d-%02d");

    // 仅时间
    static void to_current_time(struct tm* current_datetime_struct, char* time_buffer, size_t time_buffer_size, const char* format = "%02d:%02d:%02d");
    static std::string to_current_time(struct tm* current_datetime_struct, const char* format = "%02d:%02d:%02d");

    // 仅年份
    static void to_current_year(struct tm* current_datetime_struct, char* year_buffer, size_t year_buffer_size);
    static std::string to_current_year(struct tm* current_datetime_struct);

    // 仅月份
    static void to_current_month(struct tm* current_datetime_struct, char* month_buffer, size_t month_buffer_size);
    static std::string to_current_month(struct tm* current_datetime_struct);

    // 仅天
    static void to_current_day(struct tm* current_datetime_struct, char* day_buffer, size_t day_buffer_size);
    static std::string to_current_day(struct tm* current_datetime_struct);

    // 仅小时
    static void to_current_hour(struct tm* current_datetime_struct, char* hour_buffer, size_t hour_buffer_size);
    static std::string to_current_hour(struct tm* current_datetime_struct);

    // 仅分钟
    static void to_current_minite(struct tm* current_datetime_struct, char* minite_buffer, size_t minite_buffer_size);
    static std::string to_current_minite(struct tm* current_datetime_struct);

    // 仅秒钟
    static void to_current_second(struct tm* current_datetime_struct, char* second_buffer, size_t second_buffer_size);
    static std::string to_current_second(struct tm* current_datetime_struct);
	
	// 将一个字符串转换成日期时间格式
	// str:符合“YYYY-MM-DD HH:MM:SS”格式的日期时间
	// datetime_struct:存储转换后的日期时间
	// 转换成功返回true，否则返回false
    static bool datetime_struct_from_string(const char* str, struct tm* datetime_struct);
    static bool datetime_struct_from_string(const char* str, time_t* datetime);

    // 返回“YYYY-MM-DD HH:MM:SS”格式的日期时间
    static std::string to_string(time_t datetime, const char* format = "%04d-%02d-%02d %02d:%02d:%02d");

    // 返回格式由参数format决定，默认为“YYYY-MM-DD HH:MM:SS”格式的日期时间
    static std::string to_datetime(time_t datetime, const char* format = "%04d-%02d-%02d %02d:%02d:%02d");

    // 返回格式由参数format决定，默认为“YYYY-MM-DD”格式的日期时间
    static std::string to_date(time_t datetime, const char* format = "%04d-%02d-%02d");

    // 返回格式由参数format决定，默认为“HH:MM:SS”格式的日期时间
    static std::string to_time(time_t datetime, const char* format = "%02d:%02d:%02d");

    // 得到当前的微秒值
    static int64_t get_current_microseconds();
};

// 返回今天
extern std::string today(const char* format = "%04d-%02d-%02d");
// 返回昨天
extern std::string yesterday(const char* format = "%04d-%02d-%02d");
// 返回明天
extern std::string tomorrow(const char* format = "%04d-%02d-%02d");

// 取得格式化的当前日期时间，
// 如果with_milliseconds为true，则返回格式为：YYYY-MM-DD hh:mm:ss/ms，其中ms最长为10位数字；
// 如果with_milliseconds为false，则返回格式为：YYYY-MM-DD hh:mm:ss。
//
// 如果with_milliseconds为true则datetime_buffer_size的大小不能小于sizeof("YYYY-MM-DD hh:mm:ss/0123456789")，
// 如果with_milliseconds为false则datetime_buffer_size的大小不能小于sizeof("YYYY-MM-DD hh:mm:ss")，
extern void get_formatted_current_datetime(char* datetime_buffer, size_t datetime_buffer_size, bool with_milliseconds = true);

// 如果with_milliseconds为false，则返回同CDatetimeUtils::get_current_datetime()
// 如果with_milliseconds为true，则返回为：YYYY-MM-DD hh:mm:ss/milliseconds
extern std::string get_formatted_current_datetime(bool with_milliseconds = true);

UTILS_NS_END

#endif
