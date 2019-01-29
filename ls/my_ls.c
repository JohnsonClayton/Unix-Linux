#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char** argv) {

	DIR* head_dir;
	struct dirent* dp;
	int errno;
	_Bool a_set = 0; 
	_Bool l_set = 0;
	char** folders_list;
	int folders_counter = 0;
	struct stat stat_block;
	struct passwd* pwd;
	struct group* grp;
	char* temp;
	struct tm* timeinfo;
	//char* buffer;
	int bin_trac;

	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == 45) {
			//Parse the arguments passed
			int current = 1;
			while(argv[i][current]) {
				switch(argv[i][current]) {
					case 'a': 
						a_set = 1;
						break;
					case 'l':
						l_set = 1;
						break;
					default:
						printf("unknown arg: %c", argv[i][current]);
				}
				current++;
			}
			argv[i] = NULL;
		}

	}

	_Bool directory_specified = 0;
	for(int i = 1; i < argc; ++i) {
		if(argv[i] != NULL) {
			directory_specified = 1;
		}
	}
	
	if(!directory_specified) {
		strcpy(argv[0], ".");
	}
	if(l_set) {
		printf("permissions\tuser\tgroup\tsize\tfile\n");
	}
	//printf("argv[0] = %s\n", argv[0]);
	for(int i = 0; i < argc; ++i) {
		if(argv[i] != NULL) {
		head_dir = opendir(argv[i]);
		while(head_dir) {
			errno = 0;
			if((dp = readdir(head_dir)) != NULL) {
				if(l_set) {
					//memset(temp, 0, sizeof(temp));
					//strcpy(temp, argv[i]);
					//strcat(temp, dp->d_name); //This probably breaks the program b/c it doesn't know the absolute path
					//printf("temp: %s\n", temp);
					stat(dp->d_name, &stat_block);
					//printf("d_name : %s -> ", dp->d_name);
					//stat(temp, &stat_block);

					//time(&stat_block.st_mtime);
					//timeinfo = localtime(&stat_block.st_mtime);
					//strftime(buffer, 80, "%h %e %R", timeinfo);

					bin_trac = 1 << 8;
					for(int j = 0; j < 9; ++j) {
						if(bin_trac & stat_block.st_mode) {
							switch(j % 3) {
								case 0:
									printf("r");
									break;
								case 1:
									printf("w");
									break;
								case 2:
									printf("x");
									break;
								default:
									printf("?");
							}
						} else {
							printf("-");
						}

						bin_trac = bin_trac >> 1;
					}

					//printf("%04o\t", stat_block.st_mode);
					printf("\t%s\t", getpwuid(stat_block.st_uid)->pw_name);
					printf("%s\t", getgrgid(stat_block.st_gid)->gr_name);
					printf("%lld\t", (long long)stat_block.st_size);
					//printf("%s\t", buffer);
					//puts(buffer);
					printf("%s\n", dp->d_name);
					
					//printf("%s\t%s\t%lld\t%s\t%s\n", getpwuid(stat_block.st_uid)->pw_name, getgrgid(stat_block.st_gid)->gr_name, (long long)stat_block.st_size, ctime(&stat_block.st_mtime), dp->d_name);
					//if(dp->d_name[0] != 46) {
					//	printf("l is called%s \n", dp->d_name);
					//} else if(a_set) {
					//	printf("l is called%s \n",dp->d_name);
					//} 
				} else {
					if(dp->d_name[0] != 46) {
						printf("%s \n", dp->d_name);
					} else if(a_set) {
						printf("%s \n", dp->d_name);
					}
				}
			}
			else {
				if(errno == 0) {
					closedir(head_dir);
					return 1;
				}
				closedir(head_dir);
				return 2;
			}
		}
		}
	}
	return 0;
}
