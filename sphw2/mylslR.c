#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#define MAX 512
void subdir(char *wd, char** cwdlist);
int k=0;


//디렉토리 레벨 저장
int main()
{
	char **nlist = malloc(sizeof(char *) * MAX);     // DEPTH의 최대 512
	subdir(".", nlist);
	return 0;
}

void subdir(char *wd, char** cwdlist)
{

	struct dirent **dentry;
	struct stat sbuf;
	struct stat nsbuf;
	struct stat lbuf;
	struct tm *st_time;
	DIR *dirp;
	int i,j,n,z;
	int indent=0;
	char buf[MAX];
	int sum = 0;
	int ino;
	char **list = malloc(sizeof(char *) * MAX);
	
	// working directory change
	if(chdir(wd)<0)
	{
		printf("change dir error!\n");
		exit(1);
	}

	//디렉토리가 안열리면 error
	if((dirp=opendir("."))==NULL)
	{
		printf("open dir error!\n");
		exit(1);
	}
	
	cwdlist[k] = malloc(sizeof(char)*512);
	strcpy(cwdlist[k],wd);
	k++;
	for(z=0;z<k;z++)
	{
		if(cwdlist[z] == '\0')
		{
		}
		else if(z == k-1)
		{
			printf("%s",cwdlist[z]);
		}
		else
		{
			printf("%s/",cwdlist[z]);
		}
	}
	printf(":\n");

	n = scandir(".", &dentry, NULL, alphasort);
	for(j=0;j<n;j++)
	{
		if(dentry[j]->d_ino != 0)
		{
			if((!strcmp(dentry[j]->d_name,"."))||(!strcmp(dentry[j]->d_name,"..")))
			{
				continue;
			}
			lstat(dentry[j]->d_name,&nsbuf);
			sum += nsbuf.st_blocks;
		}
	}
	printf("합계 : %d\n",sum/2);
	for(i=0;i < n;i++)
	{
		if(dentry[i]->d_ino != 0)
		{
			if((!strcmp(dentry[i]->d_name,"."))||(!strcmp(dentry[i]->d_name,"..")))
			{
				continue;
			}
			
			lstat(dentry[i]->d_name,&sbuf);


			if(S_ISDIR(sbuf.st_mode)) //directory
			{
				printf("d"); // directoriy d
				printf("%s%s%s", (sbuf.st_mode & S_IRUSR)?"r":"-", (sbuf.st_mode & S_IWUSR)?"w":"-",(sbuf.st_mode & S_IXUSR)?"x":"-");
                                printf("%s%s%s", (sbuf.st_mode & S_IRGRP)?"r":"-", (sbuf.st_mode & S_IWGRP)?"w":"-",(sbuf.st_mode & S_IXGRP)?"x":"-");
                                printf("%s%s%s", (sbuf.st_mode & S_IROTH)?"r":"-", (sbuf.st_mode & S_IWOTH)?"w":"-",(sbuf.st_mode & S_IXOTH)?"x":"-");
				printf(" %ld ", (long) sbuf.st_nlink);
				printf("%s %s ", (getpwuid(sbuf.st_uid))->pw_name, (getgrgid(sbuf.st_gid))->gr_name);
				printf("%6ld ",(long) sbuf.st_size);
				st_time = localtime(&sbuf.st_mtime);
				strftime(buf,MAX, "%Y-%m-%d %H:%M",st_time);
				printf(buf);
				printf(" %s\n",dentry[i]->d_name);
				list[indent] = malloc(sizeof(char)*512);
				list[indent] = dentry[i]->d_name;
				indent++;
			}
			else if(S_ISREG(sbuf.st_mode))	//regular file
			{
				printf("-");	// regular file
				printf("%s%s%s", (sbuf.st_mode & S_IRUSR)?"r":"-", (sbuf.st_mode & S_IWUSR)?"w":"-",(sbuf.st_mode & S_IXUSR)?"x":"-");
   				printf("%s%s%s", (sbuf.st_mode & S_IRGRP)?"r":"-", (sbuf.st_mode & S_IWGRP)?"w":"-",(sbuf.st_mode & S_IXGRP)?"x":"-");
   				printf("%s%s%s", (sbuf.st_mode & S_IROTH)?"r":"-", (sbuf.st_mode & S_IWOTH)?"w":"-",(sbuf.st_mode & S_IXOTH)?"x":"-");
				printf(" %ld ", (long) sbuf.st_nlink);
				printf("%s %s ", (getpwuid(sbuf.st_uid))->pw_name, (getgrgid(sbuf.st_gid))->gr_name);
				printf("%6ld ",(long) sbuf.st_size);
				st_time = localtime(&sbuf.st_mtime);
				strftime(buf,MAX, "%Y-%m-%d %H:%M",st_time);
				printf(buf);
				printf(" %s\n",dentry[i]->d_name);
			}
			else if(S_ISLNK(sbuf.st_mode)) // symbolic link
			{
				printf("l");    // symbolic link	
				printf("%s%s%s", (sbuf.st_mode & S_IRUSR)?"r":"-", (sbuf.st_mode & S_IWUSR)?"w":"-",(sbuf.st_mode & S_IXUSR)?"x":"-");
				printf("%s%s%s", (sbuf.st_mode & S_IRGRP)?"r":"-", (sbuf.st_mode & S_IWGRP)?"w":"-",(sbuf.st_mode & S_IXGRP)?"x":"-");
				printf("%s%s%s", (sbuf.st_mode & S_IROTH)?"r":"-", (sbuf.st_mode & S_IWOTH)?"w":"-",(sbuf.st_mode & S_IXOTH)?"x":"-");
				printf(" %ld ", (long) sbuf.st_nlink);
				printf("%s %s ", (getpwuid(sbuf.st_uid))->pw_name, (getgrgid(sbuf.st_gid))->gr_name);
				printf("%6ld ",(long) sbuf.st_size);
				st_time = localtime(&sbuf.st_mtime);
				strftime(buf,MAX, "%Y-%m-%d %H:%M",st_time);
				printf(buf);
				printf(" %s",dentry[i]->d_name);
				stat(dentry[i]->d_name,&lbuf);
				ino = lbuf.st_ino;
				for(j=0;j<n;j++)
				{
					stat(dentry[j]->d_name,&lbuf);
					if(dentry[j]->d_ino == ino)
					{
						printf(" -> %s\n",dentry[j]->d_name);
						break;	
					}
				}
			}
			else
			{
				printf("\n\r");
			}
		}
	}
	for(i=0;i<indent;i++)
	{
		printf("\n");
		subdir(list[i],cwdlist);
	}	
	closedir(dirp);
	indent--;
	chdir("..");
	k--;
	cwdlist[k] = '\0';
}
