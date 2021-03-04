//obj-y := 2017510014.o
//core-y        += kernel/ certs/ mm/ fs/ ipc/ security/ crypto/ block/ mysyscalls/
//335    common  2017510014        __x64_sys_2017510014
//asmlinkage long sys_201510014(char __user *, char __user *);


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc,char *argv[]){

	syscall(335, argv[1],argv[2] );
	return 0;

}

