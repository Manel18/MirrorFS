#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <string.h>
#include <fuse.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#define LOG(s) printf((s))
#define LOGA(s, a) printf((s),(a))
#define ELOG(e) printf("ERROR::%d\n", e)

/*
 * INFO: This filesystem simply calls the respective system call of each
 * 		 operation.
 *		 Check man pages for more info on the system calls 
 */

static int mir_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
						 off_t offset,  struct fuse_file_info* fi,
						 enum fuse_readdir_flags flags)
{

	DIR* dp;
	struct dirent* dirp;

	LOGA("[DEBUG] mir_readdir...%s...", path);	
	if((dp = opendir(path)) == NULL)
	{
		/*LOGA("ERROR::%d",errno);*/
		ELOG(errno);
		return -errno;
	}

	while((dirp = readdir(dp)))
	{
		LOGA("\n\t:> %s", dirp->d_name);
		filler(buf, dirp->d_name, NULL, 0, 0);
	}

	LOG("\nDONE\n");
	return 0;
}

static int mir_mkdir(const char* path, mode_t mode)
{
	int res;
	LOGA("[DEBUG] mir_mkdir...%s...", path);
	res = mkdir(path, mode);
	if(res == -1)
	{
		ELOG(errno);
		return -errno;
	}

	LOG("DONE\n");
	return 0;
}

static int mir_rmdir(const char* path)
{
	int res;
	LOGA("[DEBUG] mir_rmdir...%s...", path);
	res = rmdir(path);

	if(res == -1)
	{
		ELOG(errno);
		return -errno;
	}

	LOG("DONE\n");
	return 0;
}

static int mir_getattr(const char* path, struct stat* stbuf,
				struct fuse_file_info *fi)
{
	(void) fi;
	int res;

	LOGA("[DEBUG] mir_getattr...%s...",path);
	
	res = lstat(path, stbuf);
	if(res == -1)
	{
		ELOG(errno);
		return -errno;
	}
	LOG("DONE\n");
	return res;
}

static int mir_open(const char* path, struct fuse_file_info* fi)
{
	int fd;

	LOGA("[DEBUG] mir_open...%s...", path);
	fd = open(path, fi->flags);
	if(fd == -1)
	{
		ELOG(errno);
		return -errno;
	}
	fi->fh = fd;
	LOG("DONE\n");
	return 0;
}

static int mir_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int res;
	(void) path;

	res = pread(fi->fh, buffer, size, offset);
	if(res == -1)
		return -errno;
	
	return res;
}

static int mir_write(const char* path, const char* buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int res;
	
	res = pwrite(fi->fh, buffer, size, offset);
	if(res == -1)
		res = -errno;
	
	return res;
}

static int mir_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{
	int fd;

	LOGA("[DEBUG] mir_create...%s...", path);
	fd = open(path, fi->flags, mode);
	if(fd == -1)
	{
		ELOG(errno);
		return -errno;
	}
	fi->fh = fd;
	LOG("DONE\n");
	return 0;
}

static int mir_unlink(const char* path)
{
	int res;
	
	LOGA("[DEBUG] mir_unlink...%s...", path);

	res = unlink(path);
	if(res == -1)
	{
		ELOG(errno);
		return -errno;
	}

	LOG("DONE\n");
	return 0;
}

static struct fuse_operations oper = {
	.readdir = mir_readdir,
	.getattr = mir_getattr,
	.mkdir = mir_mkdir,
	.rmdir = mir_rmdir,
	.unlink = mir_unlink,
	.open = mir_open,
	.read = mir_read,
	.write = mir_write,
	.create = mir_create,
};

int main(int argc, char* argv[])
{
	umask(0);
	return fuse_main(argc, argv, &oper, NULL);
}
