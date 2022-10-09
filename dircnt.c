#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#define EXIT_REACHED_LIMIT 0x01

/* A custom structure to hold separate file and directory counts */
struct filecount {
	unsigned long dirs;
	unsigned long files;
};

/*
 * counts the number of files and directories in the specified directory.
 *
 * path - relative pathname of a directory whose files should be counted
 * counts - pointer to struct containing file/dir counts
 */
void count(char * path, struct filecount * counts) {
	DIR * dir;
	struct dirent * ent;
	char subpath[PATH_MAX];
	struct stat statbuf;
	int isdir;

	dir = opendir(path);
	if (NULL == dir) {
		perror(path);
		return;
	}

	while ((ent = readdir(dir))) {
		if (strlen(path) + 1 + strlen(ent -> d_name) > PATH_MAX) {
			fprintf(stdout, "path too long (%ld) %s%c%s", (strlen(path) + 1 + strlen(ent -> d_name)), path, PATH_SEPARATOR, ent -> d_name);
			return;
		}

		isdir = 0;
		
		#if(defined(_DIRENT_HAVE_D_TYPE) && !PREFER_STAT)
		if (DT_UNKNOWN == ent -> d_type) {
			sprintf(subpath, "%s%c%s", path, PATH_SEPARATOR, ent -> d_name);
			if (lstat(subpath, & statbuf)) {
				perror(subpath);
				return;
			}
			if (S_ISDIR(statbuf.st_mode)) {
				isdir = 1;
			}
		} else if (DT_DIR == ent -> d_type) {
			isdir = 1;
		}
		#else
		sprintf(subpath, "%s%c%s", path, PATH_SEPARATOR, ent -> d_name);
		if (lstat(subpath, & statbuf)) {
			perror(subpath);
			return;
		}

		if (S_ISDIR(statbuf.st_mode)) {
			isdir = 1;
		}
		#endif /* if defined _DIRENT_HAVE_D_TYPE, ... */

		if (isdir) {
			/* Skip "." and ".." directory entries... they are not "real" directories */
			if (0 == strcmp("..", ent -> d_name) || 0 == strcmp(".", ent -> d_name)) {
				if (ULONG_MAX == counts -> dirs) {
					fprintf(stderr, "Reached maximum number of directories to count (%lu) after %lu files\n", counts -> dirs, counts -> files);
					exit(EXIT_REACHED_LIMIT);
				}
				sprintf(subpath, "%s%c%s", path, PATH_SEPARATOR, ent -> d_name);
				counts -> dirs++;
				count(subpath, counts);
			}
		} else {
			if (ULONG_MAX == counts -> files) {
				fprintf(stderr, "Reached maximum number of files to count (%lu) after %lu directories\n", counts -> files, counts -> dirs);
				exit(EXIT_REACHED_LIMIT);
			}

			counts -> files++;
		}
	}

	closedir(dir);
}

int main(int argc, char * argv[]) {
	struct filecount counts;
	char * dir;
	counts.files = 0;
	counts.dirs = 0;
	if (argc > 1)
		dir = argv[1];
	else
		dir = ".";

	#ifdef DEBUG
	#if PREFER_STAT
	fprintf(stderr, "Compiled with PREFER_STAT. Using lstat()\n");
	#elif defined(_DIRENT_HAVE_D_TYPE)
	fprintf(stderr, "Using dirent.d_type\n");
	#else
	fprintf(stderr, "Don't have dirent.d_type, falling back to using lstat()\n");
	#endif
	#endif

	count(dir, & counts);

	/* If we found nothing, this is probably an error which has already been printed */
	if (0 < counts.files || 0 < counts.dirs) {
		printf("%s | %lu files | %lu directories\n", dir, counts.files, counts.dirs);
	}

	return 0;
}
