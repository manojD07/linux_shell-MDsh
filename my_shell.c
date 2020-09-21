#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // dirent, DIR, opendir()
#include <unistd.h> // chdir(),
#include <sys/stat.h>// mkdir()
#include <fcntl.h> 
#include <readline/readline.h>
#include <readline/history.h>


extern int errno;
void _initialize_shell()
{
	printf("\n\t\t************************************************");
	printf("\n\t\t*             ~MDsh: A UNIX Shell,             *");
	printf("\n\t\t*                                              *");
	printf("\n\t\t*       developed by:                          *");
	printf("\n\t\t*                     Manoj Deka (194101029)   *");
	printf("\n\t\t*               Dhananjay Shukla (194101018)   *");
	printf("\n\t\t*______________________________________________*\n");
	
}


void _print_user_cur_dir() // prints the user & current working dir
{
	char* user = (char*)malloc(sizeof(char));
	char* curr_dir = (char*)malloc(100*sizeof(char));
	user = getenv("USER");
	getcwd(curr_dir, 100*sizeof(char));
	
	printf("\n%s@%s$ ",user,curr_dir);
 }

char* _read_command()// to read the command
	{
	char* command = (char*) malloc (100*sizeof(char));
	command = readline("");
	add_history(command);
	return command;
}



char** _parse_string(char* command)// the command string is separated into words using strsep() and " " as delimiter
	{
	char** args_list = (char**)malloc (100*sizeof(char*));
	int i=0;
	while( (args_list[i++]=strsep(&command, " ")) );
	
	return args_list;
}

void _execute_ls(char** args_list)// using opendir() and readdir() directory content is read and printed
{
	int flag;
	if(args_list[1] == NULL)
		flag=0;
	else if ( !strcmp(args_list[1], "-a") && args_list[2] == NULL )//
		flag=1;
	else 
	{
		if( strcmp(args_list[1], "-a") )// checking if option is -a
		printf("\n-MDsh: %s: Command not found",args_list[1]);
		int i=2;
		while(args_list[i])
		{
			printf("\n-MDsh: %s: %s: No such file or directory",args_list[0],args_list[i]);
			i++;
		}
		return;
	}
	
	struct dirent *dirent_ptr;
	DIR* dir_ptr;
	char* curr_dir = (char*)malloc(100*sizeof(char));
	getcwd(curr_dir, 100*sizeof(char));// current directory is read
	dir_ptr = opendir(curr_dir);// curr dir is opened
	
	while( (dirent_ptr = readdir(dir_ptr)) )
	{
		
		if(!flag &&  dirent_ptr->d_name[0]=='.')
			continue;
		printf("%s    ",dirent_ptr->d_name);// contents in directory are printed
	
	}
		
	closedir(dir_ptr);
	
}

void _execute_cd(char** args_list)// cd is implemented using chdir()
{
	
	int status;
	if(args_list[1] == NULL)
		return ;
	else //if(args_list[2] == NULL)
		status = chdir(args_list[1]);
	
	if(status)
		printf("\n~MDsh: CD: %s: No such file or directory",args_list[1]);
}

void _execute_cat(char** args_list)//
{
	if(args_list[1] == NULL)
	{
		char* str = (char*)malloc(10*sizeof(char));
		while(1)
		{
			str = readline(">");
			printf(" %s\n",str);
		}
	}
	else if( !strcmp(args_list[1], "-n") )
	{
		FILE *fp;
		int i=2;
		while( !(args_list[i] == NULL) )
		{
			
			fp = fopen(args_list[i] , "r");
			if(fp == NULL)
			{
				printf("\n~MDsh: cat: %s: No such file or directory",args_list[i]);
			}
			else
			{
				int a=1;
				int j=2;
				printf("\n     1  ");
				while(a)
				{
					char c = fgetc(fp);
					if(c == EOF)
						a=0;
					else
					{
						printf("%c",c);
						if(c == '\n' )
						{
							if(j<10)
								printf(" ");
							if(j<100)
								printf(" ");
							printf("  %d  ",j++);
								
						}
						
					}
				}
				fclose(fp);
			}
			i++;
		}
	
	}
	
	
	
	else //if(args_list[2] == NULL)
	{
		int i=1;
		FILE *fp;
		while(args_list[i])
		{
			
			fp = fopen(args_list[i] , "r");
			if(fp == NULL)
			{
				printf("\n~MDsh: cat: %s: No such file or directory",args_list[i]);
			}
			else
			{
				int a=1;
				printf("\n");
				while(a)
				{
					char c = fgetc(fp);
					if(c == EOF)
						a=0;
					else
						printf("%c",c);
				}
				fclose(fp);
			}
			i++;
		}
	}
}

void _execute_mkdir(char** args_list)// using mkdir() system call & providing permission
{
	if(args_list[1] == NULL)
	{
		printf("\nusage: mkdir [-pv] [-m mode] directory ...");
	}
	else if ( args_list[1][0] == '-')
	{
		if( !(strcmp(args_list[1] , "-v") ) )
		{
			if(args_list[2] == NULL)
				printf("\nusage: mkdir [-pv] [-m mode] directory ...");
			else
			{
				/*int i=2;
				char* dirp;
				//while( (dirp = strsep(args_list[i], "/")) )
				{
					int status;
					//status = chdir(dir);
					//if(status)
				}*/
				
				int i=2;
				while(args_list[i])
				{
					mkdir(args_list[i++], 0755);
				}
				
				
			}
		}
		else if( !(strcmp(args_list[1] , "-m") ) )
		{
			if(args_list[2] == NULL)
			{
				printf("\nmkdir: option requires an argument -- m");
				printf("\nusage: mkdir [-pv] [-m mode] directory ...\n");
				
			}
			else if( (strlen(args_list[2])==4) && (strcmp("000",args_list[2])<=0 ) && (strcmp("777",args_list[2])>=0 ) )
			{
				if(args_list[3] == NULL)
				{
					printf("\nusage: mkdir [-pv] [-m mode] directory ...");
				}
				else
				{

					mode_t mode;
					mode = (args_list[2][0]-48)*512 + (args_list[2][1]-48)*64 + (args_list[2][2]-48)*8 + (args_list[2][3]-48);
					
					//printf("\n%d %d %d %d", args_list[2][0], args_list[2][1], args_list[2][2], args_list[2][3]);
					
					//printf("\n%s: dir created. mode: %o", args_list[3],mode);
					//mkdir(args_list[3], mode);
					
					int i=3;
					while(args_list[i])
					{
						mkdir(args_list[i], mode);
						i++;
					}
					
				}
				
			}
			
		}
		
	}
	else
	{
		int i=1;
		while(args_list[i])
		{
			mkdir(args_list[i++], 0755);
		}
	}
	
}


void _execute_cp(char** args_list)// 
{
	//if( !(strcmp(args_list[1], NULL) ) )
	if(args_list[1] == NULL)
	{
		printf("\nusage: cp [-R [-H | -L | -P]] [-fi | -n] [-apvXc] source_file target_file");
		return;
	}
	
	FILE* fd1;
	FILE* fd2;
	fd1 = fopen(args_list[1], "r");
	if(fd1 == NULL)
	{
		printf("\n%s: File not found", args_list[1]);
		//return;
	}
	else
	{
		fd2 = fopen(args_list[2], "r");
		if(fd2 == NULL)	
		{
			fd2 = fopen(args_list[2], "w");
		}
		char c; 
		fscanf(fd1,"%c",&c);
		while(c)
		{
		
		printf("%c",c);
		fprintf(fd2, "%c",c);
		fscanf(fd1,"%c",&c);
		}
	
		fclose(fd1);
		fclose(fd2);
		
	}
}

void _execute_sort(char** args_list)
{
	int opt = 0;
	if(args_list[1] == NULL)
	{
		return;
	}
	if(args_list[1][0] == '-')
	{
		if(args_list[1][1] == 'r')
			opt = 1;
			
		else
		{
			printf("\n~MDsh: %s: option not found",args_list[1]);
			return;
		}
	
		
	}
	{
	
	
		FILE* fd1 = fopen(args_list[1+opt], "r");
		int i=0;
		char** liness = (char**) malloc (100000*sizeof(char*));
		size_t size = 100;
		ssize_t sizee=1;
		while(sizee>0)
		{
			
			size = 100;//*sizeof(liness[i]);
			sizee = getline(&liness[i], &size, fd1);
			//printf("\n%d: %s\n",i+1,liness[i]);
			i++;
		}
		//printf("\n");
		char* str = (char*)malloc(100000*sizeof(char));
		for(int j=0; j<i-1; j++)
		{
			int min=j;
			for(int k=j+1; k<i; k++)
			{
				if( strcmp(liness[k],liness[min]) < 0 )
					min = k;
			}
			if(min != j)
			{
			strcpy(str,liness[min]);
			//printf("\n%d: %s",min,str);
			strcpy(liness[min], liness[j]);
			//printf(" %s",liness[min]);
			strcpy(liness[j],str);
			//printf("\n%d:%s",j+1,liness[j]);
			}
		
		}
		liness[i-1] = strcat(liness[i-1],"\n");
		if(opt)
		{
			
			for(int j=0; j<i; j++)//(int j=i-1;j>=0;j++)
			{
				printf("%s",liness[i-1-j]);
				if(!strlen(liness[i-1-j]))
					printf("\n");
			}
		}
		else
		{
			for(int j=0; j<i; j++)
			{
				printf("%s",liness[j]);
				if(!strlen(liness[j]))
					printf("\n");
			}
		}
		
	
	}
	
	
}

void _grep_p(char** args_list, int n)
{
	char* pattern = args_list[1+n];
	//printf("\nPattern: %s\n",pattern);
	int p=2;
	while( args_list[p+n] != NULL )
	{
		//printf("\nFile: %s\n",args_list[p+n]);
		printf("\n");
		FILE* fd = fopen(args_list[p+n], "r");
		if(fd == NULL)
		{
			printf("\n~MDsh: %s: File not found\n",args_list[p+n]);
			return;
		}
		int i=1;
		size_t size = 100;
		ssize_t size_of_a_line=1;
		char* line;

		while(size_of_a_line>0)
		{
			size_of_a_line = getline(&line, &size, fd);
			int k=0;
			int flag=0;
			while( line[k] != '\0' )
			{
				if(line[k] == pattern[0])
				{
					flag = 1;
					for(int l=1; l< strlen(pattern); l++)
					{
						if(line[k+l] != pattern[l])
						{
							flag=0;
						}
					}
					if(flag)
						break;
				}
				k++;
			}
			if(flag)
			{
				printf("%s: %d: %s",args_list[p+n], i, line);
			}
			i++;
		}
		fclose(fd);
		p++;
	}	
}

void _execute_grep(char** args_list)
{
	if(args_list[1] == NULL)
	{
		printf("usage: grep [option][pattern] [files]\n");
		printf("");
		printf("");
		return;
	}
	if(args_list[1][0] == '-')
	{
		if(args_list[1][1] == 'n')
		{
			if( !args_list[2] || !args_list[3])
				printf("usage: grep [option][pattern] [files]\n");
			_grep_p(args_list, 1);
			return;
		}
		else
		{
			printf("\nMDsh: %s: option not found\n",args_list[1]);
			return;
		}
	}
	if(args_list[2] == NULL)
	{
		printf("usage: grep [option][pattern] [files]\n");
		return;
	}
	_grep_p(args_list,0);
}


void _execute_command(char** args_list)
{
	if( !strcmp(args_list[0], "ls") )
		_execute_ls(args_list);
	else if( !strcmp(args_list[0], "cd") )
		_execute_cd(args_list);
	else if( !strcmp(args_list[0], "cat") )
		_execute_cat(args_list);
	else if( !strcmp(args_list[0], "mkdir") )
		_execute_mkdir(args_list);
	else if( !strcmp(args_list[0], "cp") )
		_execute_cp(args_list);
	else if( !strcmp(args_list[0], "sort") )
		_execute_sort(args_list);
	else if( !strcmp(args_list[0], "grep") )
		_execute_grep(args_list);
	
		
		
	else if( !strcmp(args_list[0], "exit") )
	{
		system("clear");
		exit(0);
	}
	else
	{
		printf("\n-MDsh: %s: Command not found",args_list[0]);
		return;
	}
		
	
	
}


int main()
{
	system("clear");
	_initialize_shell();
	

	//printf("\033[H\033[J"); // \033 - ASCII escape character 33 in octal, 27 in dec.
							// \033[H moves the cursor to the top left corner of the screen
							// \033[J clears the part of the screen from the cursor to the end of the screen
	//char command[max_command_size];
	char* command = (char*) malloc (100*sizeof(char));
	//int i = 5;
	while(1)
	{
		char** args_list = (char**)malloc (100*sizeof(char*));
		
		_print_user_cur_dir();
		command = _read_command();
		if(strlen(command) == 0)
			continue;
		//printf("\nSize: %zu, Command: %s1\n",sizeof(command),command);
		printf(" ");
		args_list = _parse_string(command);
		//printf("\nC1: %s\nC2: %s\nC3: %s",args_list[0],args_list[1],args_list[2]);
		//printf("\nC1: %sC2: %s2\n",args_list[0],args_list[1]);
		_execute_command(args_list);
		
		//free(args_list);
	}
	
	return 0;
}










































