#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int listDirs(int argc, char** argv, int a_set, int l_set, int r_set) {
	//printf("\n~~~ listDirs entered ~~~\n\targc: %d\n\targv[0]: %s\n", argc, argv[0]);
	DIR* head_dir;
	struct dirent* dp;
	int errno;
	struct stat stat_block;
	int bin_trac;
	char* new_args[100];
	int new_argsc = 0;

	for(int i = 0; i < argc; ++i) {
		if(argv[i] != NULL) {
			head_dir = opendir(argv[i]);
			if(head_dir) {
				printf("\n\tListing %s\n", argv[i]);
			}
			if(l_set && head_dir) {
				printf("permissions\tuser\tgroup\tsize\tfile\n--------------------------------------------\n");
			}
		}
//		head_dir = opendir(argv[i]);
		while(head_dir) {
			errno = 0;
			if((dp = readdir(head_dir)) != NULL) {
				stat(dp->d_name, &stat_block);
				if(r_set && S_ISDIR(stat_block.st_mode) && dp->d_name[0] != '.') {
					//new_args = malloc(sizeof(new_args));
					new_args[new_argsc] = malloc(sizeof(argv[0]) + sizeof(dp->d_name));
					strcat(new_args[new_argsc], argv[i]);
					strcat(new_args[new_argsc], "/");
					strcat(new_args[new_argsc], dp->d_name); //This is building the relative path for the new file to recurse into
					++new_argsc;
				}

				if(l_set) {
					if(S_ISDIR(stat_block.st_mode)) {
						printf("d");
					} else {
						printf("-");
					}
					bin_trac = 1 << 8; //Found this strategy at https://jameshfisher.com/2017/02/24/what-is-mode_t.html
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
				if(new_argsc) {
					return listDirs(new_argsc, new_args, a_set, l_set, r_set);
				}
				if(errno == 0) {
					closedir(head_dir);
					return 1;
				}
				closedir(head_dir);
				return 2;
			}
		}
		
	}
	if(new_argsc) {
	return	listDirs(new_argsc, new_args, a_set, l_set, r_set);
	}
}

void setArgs(int argc, char** argv,  int* a_set, int* l_set, int* r_set) {

	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == 45) {
			//Parse the arguments passed
			int current = 1;
			while(argv[i][current]) {
				switch(argv[i][current]) {
					case 'a': 
						*a_set = 1;
						break;
					case 'l':
						*l_set = 1;
						break;
					case 'r': 
						*r_set = 1;
						break;
					default:
						printf("unknown arg: %c", argv[i][current]);
				}
				++current;
			}
			argv[i] = NULL;
		}

	}

	int directory_specified = 0;
	for(int i = 1; i < argc; ++i) {
		if(argv[i] != NULL) {
	//	if(!strcmp(argv[i], "")) {
			directory_specified = 1;
		}
	}
	
	if(!directory_specified) {
		strcpy(argv[0], ".");
	}
}

int main(int argc, char** argv) {

	int a_set = 0; 
	int l_set = 0;
	int r_set = 0;
	setArgs(argc, argv, &a_set, &l_set, &r_set);
	return listDirs(argc, argv, a_set, l_set, r_set);
	//return 0;
}
