#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define SIZEBR 2000

int main(int argc, char* argv[]){
	
	char* filename = argv[1];
    char* sh_data_name = argv[2];
	char* sh_size_data_name = argv[3];
    char* mutex_name = argv[4];

    //Create fd shared file
    int fd_sh_data_name = shm_open(sh_data_name, O_RDWR | O_CREAT, S_IRWXU);
    int fd_mutex_name = shm_open(mutex_name,O_RDWR | O_CREAT, S_IRWXU);
	int fd_filename = open(filename,O_RDWR | O_CREAT, 0777);
	int fd_sh_size_data_name = shm_open(sh_size_data_name,O_RDWR | O_CREAT, S_IRWXU);
	if((fd_sh_data_name == -1) ||(fd_filename == -1) || (fd_mutex_name == -1) ||(fd_sh_size_data_name == -1)){
		perror("Error of shm_open file!");
		return -6;
	}


	struct stat statbuf;
	if (fstat(fd_filename, &statbuf) < 0 ){
	    perror("fstat");
		return -9;
	}
	size_t len = statbuf.st_size;
	ftruncate(fd_filename,len);
	//printf("%lu /", len);
	ftruncate(fd_mutex_name,sizeof(pthread_mutex_t*));
	ftruncate(fd_sh_data_name,getpagesize());
	ftruncate(fd_sh_size_data_name,sizeof(int));

	
	
	//Map shared files in memory
    char* str = (char*) mmap(NULL,getpagesize(),PROT_READ | PROT_WRITE, MAP_SHARED, fd_filename, 0);
	//printf("%s /", str);
	float* result_mass = (float*) mmap(NULL,getpagesize(),PROT_READ | PROT_WRITE, MAP_SHARED, fd_sh_data_name, 0);
    pthread_mutex_t *Lock = (pthread_mutex_t*) mmap(NULL,sizeof(pthread_mutex_t*),PROT_READ | PROT_WRITE, MAP_SHARED,fd_mutex_name,0);
	int* sizes_ = (int*) mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd_sh_size_data_name, 0);
	
	float data[2];
	int k = 0, j = 0;
	size_t index = 0;
	*sizes_ = 0;

	char* substr = (char*) malloc(sizeof(char)*256);
	for(int i = 0; i < len; i = i+k+1){
		for(j = i, k = 0; j < len; ++j, ++k){
			if((str[j] == ' ') ||(str[j] == '\n')){
				substr[k] = '\0';
				break;
			}
			substr[k] = str[j];
		}
		if(i == 0){
			data[0] = atof(substr);
			continue;
		}
		if(i != 0){
			data[1] = atof(substr);
			if(data[1] == 0){
				printf("\nDivision 0\n");
				//printf("%d ", *sizes_);
				return -10;
			}
			result_mass[index++] = data[0] / data[1];
			*sizes_ += 1;
			data[0] = data[1];
			//printf("%f ", result_mass[index - 1]);
		}
	}
	free(substr);


	munmap(str,len);
    munmap(Lock,sizeof(pthread_mutex_t*));
    munmap(result_mass,getpagesize());
	munmap(sizes_, sizeof(int));

    close(fd_sh_data_name);
    close(fd_mutex_name);
	close(fd_sh_size_data_name);
    close(fd_filename);

	shm_unlink(sh_data_name);
    shm_unlink(mutex_name);
    shm_unlink(filename);
    shm_unlink(sh_size_data_name);
	return 0;
}
