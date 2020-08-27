#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static int do_getattr(const char *path, struct stat *st) {
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if (strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    }
    else if (strcmp(path, "/lenta.ru") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 1;
    }
    else {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }

    return 0;
}


static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if (strcmp(path, "/") == 0) {
        filler(buffer, "lenta.ru", NULL, 0);
    }
    else if (strcmp(path, "/lenta.ru") == 0) {
        FILE *fp = popen("./trace.sh", "r");
        const int max_buff_size = 256;

        char buff[max_buff_size];

        fgets(buff, max_buff_size, fp);
        memset(buff, 0, sizeof(buff));

        while(fgets(buff, max_buff_size, fp) != NULL) {
            buff[strlen(buff)-1] = '\0';

            filler(buffer, buff, NULL, 0);
            memset(buff, 0, sizeof(buff));
        }

        pclose(fp);
    }


    return 0;
}


static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
};


int main(int argc, char *argv[]) {

    return fuse_main(argc, argv, &operations, NULL);
}
