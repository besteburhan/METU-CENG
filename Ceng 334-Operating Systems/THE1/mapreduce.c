#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int readInput(char *buffer, int fd, int size) {
    char c;
    int num = 0;
    long len;
    len = read(fd, &c, 1);
    while(len > 0) {
        buffer[num++] = c;
        if(c == '\n') {
            buffer[num++] = '\0';
            return num;
        }
        len = read(fd, &c, 1);
    }
    return -1;
}
int main(int argc, char const *argv[])
{

	int N;
	N = atoi(argv[1]);
	int pm_fd[N][2];
	int mr_fd[N][2];
	int rr_fd[N][2];
	char  path_map[100];
	char path_reduce[100];
	pid_t parent_id = getpid();
	char line[1000];
	int i;
	if(argc==3){// Mapper only .. 
		for(i=0;i<N;i++){
			pipe(pm_fd[i]);
		}
		strcpy(path_map,argv[2]);
		for(i=0;i<N;i++){
			if(fork()){//parent
				close(pm_fd[i][0]);
			}
			else{
					dup2(pm_fd[i][0], 0);
					close(pm_fd[i][0]);
					for(int j=0;j<N;j++){
						close(pm_fd[j][1]);
					}
					char key[5];
					sprintf(key,"%d",i);
					execl(path_map, path_map, key, (char*)0 );
				}	
			}
		if(getpid() == parent_id){

		i = 0;
		char line[512];
		while(readInput(line, 0, 512)>0){
			dup2(pm_fd[i%N][1], 1);
			printf("%s", line);
			i++;
			fflush(stdout);
		}

		for(int j = 0; j < N; ++j){
			close(pm_fd[j][1]);}
		close(1);
		for(int j = 0; j < N; ++j){
			int c;
			pid_t t = wait(&c);

		 }
		}
	}

	else if(argc==4){
		strcpy(path_map,argv[2]);
		strcpy(path_reduce,argv[3]);
		for(i=0;i<N;i++){
			pipe(pm_fd[i]);
			pipe(mr_fd[i]);
			if(i!=(N-1))
				pipe(rr_fd[i]);
		}
		for(i=0;i<N;i++){


			if(fork()){//parent
				if(fork()){//parent
						close(pm_fd[i][0]);
				}
				else{//reducer
					close(pm_fd[i][0]);
					dup2(mr_fd[i][0],0);
					if(i!=(N-1)){
						dup2(rr_fd[i][1],1);
					}
					if(i!=0){
						dup2(rr_fd[i-1][0],2);
					}
					for(int j=0;j<N;j++){
						close(mr_fd[j][0]);
						close(mr_fd[j][1]);
						close(pm_fd[j][1]);
						if(j != (N-1)){
							close(rr_fd[j][0]);
							close(rr_fd[j][1]);
						}
					}
					char key[5];
					sprintf(key,"%d",i);
					execl(path_reduce,path_reduce, key, (char*) 0);
				}
			}
			else{
				//mapper
				dup2(pm_fd[i][0],0);
				dup2(mr_fd[i][1],1);
				close(pm_fd[i][0]);
				for(int j=0;j<N;j++){
					close(pm_fd[j][1]);
					close(mr_fd[j][0]);
					close(mr_fd[j][1]);
					if(j != (N-1)){
						close(rr_fd[j][0]);
						close(rr_fd[j][1]);
					}

				}
				char key[5];
				sprintf(key,"%d",i);
				execl(path_map, path_map, key, (char*)0 );
				}
			}
		

		if(getpid() == parent_id){
			i = 0;
			while(readInput(line, 0, 512)>0){
				dup2(pm_fd[i%N][1], 1);
				printf("%s", line);
				i++;
				fflush(stdout);
			}

			for(int j = 0; j < N; ++j){
				close(pm_fd[j][1]);
				close(pm_fd[j][0]);
				close(mr_fd[j][0]);
				close(mr_fd[j][1]);
					if(j != (N-1)){
						close(rr_fd[j][0]);
						close(rr_fd[j][1]);
					}
			}
			close(1);
			for(int j = 0; j < 2*N; ++j){
			int c;
			pid_t t = wait(&c);

		 }
			}
	}
	
	return 0;
}
