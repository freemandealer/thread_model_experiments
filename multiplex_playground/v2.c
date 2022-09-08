#include<assert.h>
#include<signal.h>
#include<stdio.h>
#include<sys/select.h>
#include<sys/wait.h>
#include<unistd.h>
int fd[2];
const char msg[]={'m','e','s','s','a','g','e'};
void sigHandler(int){
    alarm(1);
    ssize_t bytes=write(fd[1],msg,sizeof(msg));
    printf("子进程msg已写入:%ld字节\n",bytes);
}
int main(){
    assert(pipe(fd)==0);
    pid_t pid=fork();
    if(pid==0){//子进程
        signal(SIGALRM,sigHandler);
        alarm(1);
        while(true){pause();}
    }else if(pid>0){//父进程
        while(true){
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd[0],&fds);
            int maxfdp=(fd[0]>fd[1])?fd[0]+1:fd[1]+1;
            timeval timeout={3,0};

            switch(select(maxfdp,&fds,NULL,NULL,&timeout)){
                case -1:printf("select失败\n");return 1;break;
                case 0:printf("再次轮询\n");break;
                default:
                       printf("父进程收到管道可读通知:");
                       if(FD_ISSET(fd[0],&fds)){
                           char buf[1024]={0};
                           ssize_t bytes=read(fd[0],buf,sizeof(buf));
                           printf("读入%ld字节=%s\n",bytes,buf);
                       }
                       break;
            }
        }//end while
        int status;
        wait(&status);
    }
    return 0;
}
