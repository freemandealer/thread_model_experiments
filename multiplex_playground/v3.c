#include<assert.h>
#include<ev.h>
#include<stdio.h>
#include<unistd.h>
int fd[2];
const char msg[]={'m','e','s','s','a','g','e'};
void pipe_cb(EV_P_ ev_io *w, int revents){
    char buf[1024]={0};
    ssize_t bytes=read(fd[0],buf,sizeof(buf));
    printf("父进程读入%ld字节=%s\n",bytes,buf);
}
void timer_cb(EV_P_ ev_timer *w, int revents){
    ssize_t bytes=write(fd[1],msg,sizeof(msg));
    printf("子进程msg已写入:%ld字节\n",bytes);
}
int main(){
    assert(pipe(fd)==0);
    pid_t pid=fork();
    if(pid==0){//子进程
        struct ev_loop* loop=EV_DEFAULT;
        ev_timer timer_watcher;
        ev_timer_init(&timer_watcher,timer_cb,0,0);
        timer_watcher.repeat=1;
        ev_timer_again(loop,&timer_watcher);
        ev_run(loop,0);
    }else if(pid>0){//父进程
        struct ev_loop* loop=EV_DEFAULT;
        ev_io pipe_watcher;
        ev_io_init(&pipe_watcher,pipe_cb,fd[0],EV_READ);
        ev_io_start(loop,&pipe_watcher);
        ev_run(loop,0);
    }
    return 0;
}
