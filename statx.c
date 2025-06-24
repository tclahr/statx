// SPDX-License-Identifier: Apache-2.0

/*
 * based on https://github.com/torvalds/linux/blob/master/samples/vfs/test-statx.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <sys/stat.h>

#ifndef __NR_statx
#define __NR_statx -1
#endif

ssize_t statx(int dfd, const char *filename, unsigned flags,
              unsigned int mask, struct statx *buffer)
{
    return syscall(__NR_statx, dfd, filename, flags, mask, buffer);
}

int main(int argc, char **argv)
{
    int ret;
    char file_type = '?';
    char symlink[10240];
    int atflag = AT_SYMLINK_NOFOLLOW;
    unsigned int mask = STATX_ALL;
    struct statx stx;
    int exit_code = EXIT_SUCCESS;

    memset(&stx, 0xbf, sizeof(stx));

    for (int i = 1; i < argc; i++) {

        ret = statx(AT_FDCWD, argv[i], atflag, mask, &stx);
        if (ret < 0)
        {
            perror(argv[i]);
            exit_code = EXIT_FAILURE;
            continue;
        }

        // MD5
        printf("0");

        // name
	memset(&symlink, 0, sizeof(symlink));
        ret = readlink(argv[i], symlink, sizeof(symlink));
        if (ret > 0)
            printf("|%s -> %s", argv[i], symlink);
        else
            printf("|%s", argv[i]);

        // inode
        if (stx.stx_mask & STATX_INO)
            printf("|%llu", (unsigned long long)stx.stx_ino);
        else
            printf("|0");

        // type
        if (stx.stx_mask & STATX_TYPE)
        {
            switch (stx.stx_mode & S_IFMT)
            {
            case S_IFIFO:
                file_type = 'p';
                break;
            case S_IFCHR:
                file_type = 'c';
                break;
            case S_IFDIR:
                file_type = 'd';
                break;
            case S_IFBLK:
                file_type = 'b';
                break;
            case S_IFREG:
                file_type = '-';
                break;
            case S_IFLNK:
                file_type = 'l';
                break;
            case S_IFSOCK:
                file_type = 's';
                break;
            }
        }
        printf("|%c", file_type);

        // mode as string
        if (stx.stx_mask & STATX_MODE)
            printf("%c%c%c%c%c%c%c%c%c",
                stx.stx_mode & S_IRUSR ? 'r' : '-',
                stx.stx_mode & S_IWUSR ? 'w' : '-',
                stx.stx_mode & S_IXUSR ? 'x' : '-',
                stx.stx_mode & S_IRGRP ? 'r' : '-',
                stx.stx_mode & S_IWGRP ? 'w' : '-',
                stx.stx_mode & S_IXGRP ? 'x' : '-',
                stx.stx_mode & S_IROTH ? 'r' : '-',
                stx.stx_mode & S_IWOTH ? 'w' : '-',
                stx.stx_mode & S_IXOTH ? 'x' : '-');
        else
            printf("?????????");

        // uid
        if (stx.stx_mask & STATX_UID)
            printf("|%d", stx.stx_uid);
        else
            printf("|0");

        // gid
        if (stx.stx_mask & STATX_GID)
            printf("|%d", stx.stx_gid);
        else
            printf("|0");

        // size
        if (stx.stx_mask & STATX_SIZE)
            printf("|%llu", (unsigned long long)stx.stx_size);
        else
            printf("|0");

        // atime
        if (stx.stx_mask & STATX_ATIME)
            printf("|%llu", stx.stx_atime.tv_sec);
        else
            printf("|0");

        // mtime
        if (stx.stx_mask & STATX_MTIME)
            printf("|%llu", stx.stx_mtime.tv_sec);
        else
            printf("|0");

        // ctime
        if (stx.stx_mask & STATX_CTIME)
            printf("|%llu", stx.stx_ctime.tv_sec);
        else
            printf("|0");

        // btime
        if (stx.stx_mask & STATX_BTIME)
            printf("|%llu", stx.stx_btime.tv_sec);
        else
            printf("|0");

        printf("\n");

    }

    return exit_code;
}
