#include <stdio.h>
#include <math.h>

struct sock{
	int fd;
};
struct select_pool {
	int socket_n;
	struct sock *fd;
};

void del(int a[], int n, int x)
{
	int i = 0;
	for(i = 0; i < n; i++){
		if (a[i] == x){
			a[i] = a[n-1];
			break;
		}
	}
}

void sp_del(struct select_pool *sp, int sock){
	int i;
	bool move = false;
	for (i = 0; i < sp->socket_n; i++)
	{
		if (move){
			sp->fd[i-1] = sp->fd[i];
		}else if (sp->fd[i].fd == sock){
			move = true;
		}

		if (move){
			sp->socket_n--;
		}
	}
}

int main(int argc, char** argv)
{
	const unsigned int Max = 10;
	int a[Max];
	int i = 0;
	srand((unsigned)time(NULL));
	for (i = 0; i < Max; i++)
		a[i] = rand(100);
	printf("before: \n");
	for (i = 0; i < Max; i++)
		printf("a[%d] = %d\n", i, a[i]);
	del(a, Max, a[5]);
	printf("after: \n");
	for(i = 0;i < Max; i++)
		printf("a[%d] = %d\n", i, a[i]);
	return 0;
}
