#include<stdio.h>
#include<unistd.h>
#include<string.h>

typedef void(*callshow_v)(double);//������չʾ�汾

void process(double rate){
    const char *rotat = "|/-\\";//��������ת��ʾ���������ƽ�
    static char bar[101] = {'\0'};//����������
    static size_t cnt = 0;//���ƽ���������ת
    int rlen = strlen(rotat);
    if(rate < 100.0){//�������ø�����ʱ��Ҫ���ǲ�ͬ�����ٶȵ����
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
		int tmp = 98;//�����õ��Ǹ��Ƚ���ͨ�ż�������5G��
        while(tmp>=0&&bar[tmp]!='='){
			bar[tmp]='=';
            --tmp;
        }
        printf("[%-100s][%5.1lf%%][%c]\n",bar,rate,rotat[0]); 
        memset(bar,'\0',sizeof(bar));
    }
}

void download(callshow_v cs){
    int aim = 1024*1024*10;//10MB ����������
    int v = 1024;//�����ٶ�Ϊ1KB
    int ret = 1024*1024*9;//���ؽ��ȼ�¼
    //int cnt = 500;
    while(ret<aim){//�ﵽĿ��ʱӦ��ֹͣ����Ϊ�������������ټ�¼
        ret += v;//��������
        usleep(1000*10);
        double rate = ret*100.0/aim;
        //while(cnt&&rate>=66.0){//ģ����·���ٵ����
        //    ret-=v;
        //    rate = ret*100.0/aim;
        //    --cnt;
        //}
        cs(rate);
    }
}

int main(){
    download(process);
    //download(process);//ģ��������
    //download(process);
    return 0;
}

