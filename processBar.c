#include<stdio.h>
#include<unistd.h>
#include<string.h>

typedef void(*callshow_v)(double);//进度条展示版本

void process(double rate){
    const char *rotat = "|/-\\";//进度条旋转表示程序正常推进
    static char bar[101] = {'\0'};//进度条本体
    static size_t cnt = 0;//控制进度条的旋转
    int rlen = strlen(rotat);
    if(rate < 100.0){//决定采用浮点数时，要考虑不同下载速度的情况
        if(rate<99.0){
            bar[(int)rate] = '=';
            bar[(int)rate+1] = '>';
            int tmp = (int)rate-1;
            while(tmp>=0&&bar[tmp]!='='){
                bar[tmp]='=';
                --tmp;
            }
        }
        else{
            bar[99]='=';
            bar[100]='\0';
        }
        printf("[%-100s][%5.1lf%%][%c]\r",bar,rate,rotat[cnt%rlen]);
        ++cnt;
        fflush(stdout);
    }
    else{
		rate = 100.0;
        bar[99]='=';
        bar[100]='\0';
		int tmp = 98;//假设用的是更先进的通信技术例如5G网
        while(tmp>=0&&bar[tmp]!='='){
			bar[tmp]='=';
            --tmp;
        }
        printf("[%-100s][%5.1lf%%][%c]\n",bar,rate,rotat[0]); 
        memset(bar,'\0',sizeof(bar));
    }
}

void download(callshow_v cs){
    int aim = 1024*1024*10;//10MB 待下载数据
    int v = 1024;//下载速度为1KB
    int ret = 1024*1024*9;//下载进度记录
    //int cnt = 500;
    while(ret<aim){//达到目标时应该停止，因为这里是先下载再记录
        ret += v;//正在下载
        usleep(1000*10);
        double rate = ret*100.0/aim;
        //while(cnt&&rate>=66.0){//模拟网路卡顿的情况
        //    ret-=v;
        //    rate = ret*100.0/aim;
        //    --cnt;
        //}
        cs(rate);
    }
}

int main(){
    download(process);
    //download(process);//模拟多次下载
    //download(process);
    return 0;
}

