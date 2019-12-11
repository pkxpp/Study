

int fun(int a, int b);
int m = 10;
int main()
{
	int i = 4;
	int j = 5;
	m = fun(i, j);
	return 0;
}

int fun(int a, int b)
{
	int c = 0;
	c = a + b;
	return c;
}