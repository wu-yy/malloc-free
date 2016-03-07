#include "firstlloc.h"
//測試文件
void* t[SIZE_TEST];

int main()
{
    init();
    t[0] = malloc(100);
    t[1] = malloc(200);
    t[2] = malloc(300);
    t[3] = malloc(400);
    t[4] = malloc(500);
    t[5] = malloc(600);
    test();
    printf("result:%d\n",free(t[4]));
    printf("result:%d\n",free(t[1]));
    printf("result:%d\n",free(t[0]));
    test();
    simple_malloc(700);
    test();
    return 0;
}
