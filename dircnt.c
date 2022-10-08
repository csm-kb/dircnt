#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int main (int argc, char *argv[])
{
	if (argc < 2) {
		fprintf (stderr, "[!] must specify a directory to count\n");
		return 1;
	}
	bool count_size = (argc > 2 && !strcmp(argv[2],"--size")); // todo: yes this is super basic flag checking
	DIR *dp;
	int i = 0;
	unsigned long long tsize = 0;
	struct dirent *ep;     
	dp = opendir (argv[1]);

	if (dp != NULL)	{
		while (ep = readdir (dp)) {
			if (!strcmp(ep->d_name,".") || !strcmp(ep->d_name,".."))
				continue;
			if (!count_size) {
				i++;
				continue;
			}
			char fpath[4096];
			struct stat st;

			strcpy(fpath,argv[1]);
			strcat(fpath,"/");
			strcat(fpath,ep->d_name);
			if (stat(fpath,&st) == 0)
				tsize += st.st_size;
			else
				perror("Failed to stat file");
			i++;
		}
		(void) closedir (dp);
	}
	else {
		fprintf(stderr, "[!] couldn't open the directory\n");
		return 1;
	}

	printf("[*] total: %d\t| %lli bytes\n", i, tsize);
	return 0;
}
