typedef struct
{
    int num1;
	char name1[40];
	int number1;
	int score1;
}Type;

menu1()
{
	printf("##########试题题型管理##########\n");
	printf("#           1.添加             #\n");
	printf("#           2.删除             #\n");
	printf("#           3.查看             #\n");
	printf("#           0.退出             #\n");
    printf("################################\n");
	printf("\n");
}

void main()
{
	FILE *fp;
	Type p,q;
    int i=0,t,a;
	char x;
	while(1)
	{
		menu1();
		printf("输入所需操作的编号:");
		scanf("%d",&t);
		getchar();
		switch(t)
		{
		case 1:
			{
				if((fp=fopen("type.txt","a"))==NULL)
				{printf("Cannot creat the file !");
				exit(0);}
				while(1)
				{
					printf("是否要输入?   (Y/N)  \n");
					scanf("%c",&x);
					if(x=='Y'||x=='y')
					{
						printf("依次输入:\n");
						printf("题型编号 名称 数量 分数  (以空格分开)  :\n");
						scanf("%d%s%d%d",&p.num1,p.name1,&p.number1,&p.score1);
						fprintf(fp,"%d\t%8s\t%4d\t%4d\n",p.num1,p.name1,p.number1,p.score1);
						getchar();
					}
					if(x=='n'||x=='N')
						break;
				}
				fclose(fp);
				printf("\n");
			}
			break;
		case 2:
			{
				printf("需要删除题型的编号:");
				scanf("%d",&a);
				if((fp=fopen("type.txt","r"))==NULL)
				{printf("Cannot open the file !");
				exit(0);}
				/*i=0;
				while((fscanf(fp,"%d\t%8s\t%4d\t%4d\n",&q.num1,q.name1,&q.number1,&q.score1))!=-1)
				i++;
                fclose(fp);
				i=0;
				fp=fopen("type.txt","w");
                while(q[i].num1!=a)
				
				fprintf(fp,"%d\t%8s\t%4d\t%4d\n",p.num1,p.name1,p.number1,p.score1);
				fclose(fp);*/
                
			}
		break;
		case 3:
			{
				if((fp=fopen("type.txt","r"))==NULL)
				{printf("Cannot creat the file !");
				exit(0);}
                printf("题型编号  名称\t数量\t分数\n");
				while(!feof(fp))
					putchar(fgetc(fp));
				fclose(fp);
				 
			}
				printf("\n");
			break;
		case 0:exit(0);
			
			
	}
	
}
}