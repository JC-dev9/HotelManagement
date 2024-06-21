#include <stdio.h>
#include <stdbool.h>
#include "funcoes.c"
#include <signal.h>
#include "funcoes.h"

int main()
{
    system("chcp 65001");
    clear();

    signal(SIGINT, handle_sigint);
    menuLogin();
    
    return 0;
}