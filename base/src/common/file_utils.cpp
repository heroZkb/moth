#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "file_utils.h"

UTILS_NS_BEGIN

size_t CFileUtils::file_copy(int src_fd, int dst_fd) throw (sys::CSysCallException)
{
    char buf[IO_BUFFER_MAX] = {0};
    size_t file_size = 0;

    for (;;)
    {    
        ssize_t ret = read(src_fd, buf, sizeof(buf) - 1);
        if (0 < ret)
        {      
            for (;;)
            {            
                if (write(dst_fd, buf, ret) != ret)
                {
                    if (EINTR == errno)
					{
						continue;
                    }
                    THROW_SYSCALL_EXCEPTION(NULL, errno, "write");
                }

                break;
            }

            file_size += (size_t)ret;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            if (EINTR == errno)
			{
				continue;
            }
            THROW_SYSCALL_EXCEPTION(NULL, errno, "read");
        }
    }

    return file_size;
}

size_t CFileUtils::file_copy(int src_fd, const char* dst_filename) throw (sys::CSysCallException)
{    
    int dst_fd = open(dst_filename, O_WRONLY|O_CREAT|O_EXCL);
    if (-1 == src_fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "open");
    }

    CloseHelper<int> ch(dst_fd);
    return file_copy(src_fd, dst_fd);
}

size_t CFileUtils::file_copy(const char* src_filename, int dst_fd) throw (sys::CSysCallException)
{
    int src_fd = open(src_filename, O_RDONLY);
    if (-1 == src_fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "open");
    }

    CloseHelper<int> ch(src_fd);
    return file_copy(src_fd, dst_fd);
}

size_t CFileUtils::file_copy(const char* src_filename, const char* dst_filename) throw (sys::CSysCallException)
{ 
    int src_fd = open(src_filename, O_RDONLY);
    if (-1 == src_fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "open");
    }

    CloseHelper<int> src_ch(src_fd);
    int dst_fd = open(dst_filename, O_WRONLY|O_CREAT|O_EXCL);
    if (-1 == dst_fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "open");
    }

    CloseHelper<int> dst_ch(dst_fd);
    return file_copy(src_fd, dst_fd);
}

off_t CFileUtils::get_file_size(int fd) throw (sys::CSysCallException)
{
    struct stat buf;
    if (-1 == fstat(fd, &buf))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "fstat");
    }

    if (!S_ISREG(buf.st_mode))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "fstat");
    }

    return buf.st_size;
}

off_t CFileUtils::get_file_size(const char* filepath) throw (sys::CSysCallException)
{
    int fd = open(filepath, O_RDONLY);
    if (-1 == fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "fstat");
    }

    CloseHelper<int> ch(fd);
    return get_file_size(fd);
}

uint32_t CFileUtils::crc32_file(int fd) throw (sys::CSysCallException)
{
    uint32_t crc = 0;
    int page_size = CUtils::get_page_size();
    char* buffer = new char[page_size];
    CPointGuard<char> dh(buffer, true);

    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "lseek");
    }
    for (;;)
    {
        int r = read(fd, buffer, page_size);
        if (0 == r)
        {
            break;
        }
        if (-1 == r)
        {
            THROW_SYSCALL_EXCEPTION(NULL, errno, "read");
        }

        crc = crc32(crc, (unsigned char*)buffer, r);
        if (r < page_size)
        {
            break;
        }
    }
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "lseek");
    }
    
    return crc;
}

uint32_t CFileUtils::crc32_file(const char* filepath) throw (sys::CSysCallException)
{
    int fd = open(filepath, O_RDONLY);
    if (-1 == fd)
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "open");
    }

    CloseHelper<int> ch(fd);
    return crc32_file(fd);
}

uint32_t CFileUtils::get_file_mode(int fd) throw (sys::CSysCallException)
{
    struct stat st;
    if (-1 == fstat(fd, &st))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "fstat");
    }

    return st.st_mode;
}

void CFileUtils::remove(const char* filepath) throw (sys::CSysCallException)
{
    if (-1 == unlink(filepath))
    {
        if (errno != ENOENT)
        {
            THROW_SYSCALL_EXCEPTION(NULL, errno, "unlink");
        }
    }
}

void CFileUtils::rename(const char* from_filepath, const char* to_filepath) throw (sys::CSysCallException)
{
    if (-1 == ::rename(from_filepath, to_filepath))
    {
        THROW_SYSCALL_EXCEPTION(NULL, errno, "rename");
    }
}

UTILS_NS_END
