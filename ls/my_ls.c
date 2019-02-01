#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int listDirs(int argc, char** argv, _Bool a_set, _Bool l_set, _Bool s_set) {
	DIR* head_dir;
	struct dirent* dp;
	int errno;
	struct stat stat_block;
	int bin_trac;

	for(int i = 0; i < argc; ++i) {
		if(argv[i] != NULL) {
			head_dir = opendir(argv[i]);
		}
//		head_dir = opendir(argv[i]);
		while(head_dir) {
			errno = 0;
			if((dp = readdir(head_dir)) != NULL) {
				if(l_set) {
					stat(dp->d_name, &stat_block);

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

					printf("\t%s\t", getpwuid(stat_block.st_uid)->pw_name);
					printf("%s\t", getgrgid(stat_block.st_gid)->gr_name);
					printf("%lld\t", (long long)stat_block.st_size);
					printf("%s\n", dp->d_name);
					
				} else {
					if(dp->d_name[0] != 46) { // If the first character is not a period, then show it 
						printf("%s \n", dp->d_name);
					} else if(a_set) { //If a is invoked, show the file
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

int main(int argc, char** argv) {

	_Bool a_set = 0; 
	_Bool l_set = 0;
	_Bool r_set = 0;

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
		//	strcpy(argv[i], "");
			argv[i] = NULL;
		}

	}

	_Bool directory_specified = 0;
	for(int i = 1; i < argc; ++i) {
		if(argv[i] != NULL) {
	//	if(!strcmp(argv[i], "")) {
			directory_specified = 1;
		}
	}
	
	if(!directory_specified) {
		strcpy(argv[0], ".");
	}
	if(l_set) {
		printf("permissions\tuser\tgroup\tsize\tfile\n");
	}
	return listDirs(argc, argv, a_set, l_set, r_set);
	//return 0;
}
