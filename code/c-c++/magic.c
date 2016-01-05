#include"stdio.h"
#include"math.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

int a[256][256];		// save a magic
int sum;
int check(int n);		// check a[][] is a magic
void ins(int n);
void print_a(int n);
int transfer_1(int n);
int transfer_2(int n);

void test_set();
vector<vector<int> > SubSet( const vector<int>& SrcSet );
int find_sub(int a[], int b[], int n, int sub, vector<int> &vec);
int find_index(int nNum, int n, int &x, int &y);
vector<int> compute_col(int arrA[], int arrB[], int n, int sub, bool tag);
//method
int transfer_1(int n)
{
	int i, j;
	int nTemp; int v = n-1; for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			//
			if( (i+j < v) || ((i+j == v) && i <= j)){
				nTemp = a[i][j];
				a[i][j] = a[v-i][v-j];
				a[v-i][v-j] = nTemp;	
			}
		}
	}
}

int main()
{
	int i,j,n,k,t,p,x;
	scanf("%d",&n);
	sum=(n*n+1)*n/2;
	if(n%2==1) ins(n);//奇数幻方
	if(n%4==2)//单偶数幻方
	{
		k=n/2;
		ins(k);
		for(i=0;i<k;i++)
			for(j=0;j<k;j++)
			{
				a[i][j+k]=a[i][j]+2*k*k; //(3)
				a[i+k][j]=a[i][j]+3*k*k; //(4)
				a[i+k][j+k]=a[i][j]+k*k; //(2)
			}
		t=(n-2)/4;
		for(i=0;i<k;i++)
		for(j=0;j<k;j++)
		{
			if((j<t)&&(i<t))
			{
				p=a[i][j];
				a[i][j]=a[i+k][j];
				a[i+k][j]=p;
			}
			if((j<t)&&(i>k-t-1))
			{
				p=a[i][j];
				a[i][j]=a[i+k][j];
				a[i+k][j]=p;
			}
			if((i>=t&&i<=k-t-1)&&(j>=t&&j<t*2))
			{
				p=a[i][j];
				a[i][j]=a[i+k][j];
				a[i+k][j]=p;
			}
			if(j>1&&j<=t)
			{
				p=a[i][j+k];
				a[i][j+k]=a[i+k][j+k];
				a[i+k][j+k]=p;
			}
		}
	}
	if(n%4==0)//双偶数幻方
	{
		x=1;
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				a[i][j]=x++;
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
			{
				if(i%4==0&&abs(i-j)%4==0)
					for(k=0;k<4;k++)
						a[i+k][j+k]=n*n-a[i+k][j+k]+1;
				else if(i%4==3&&(i+j)%4==3)
					for(k=0;k<4;k++)
						a[i-k][j+k]=n*n-a[i-k][j+k]+1;
			}
	}
	if(check(n)==1)
	{
		print_a(n);
		//for(i=0;i<n;i++)
		//{
		//	for(j=0;j<n;j++)
			//printf("%5d",a[i][j]);
		//	printf("\t%d",a[i][j]);
		//	printf("\n");
		//}
		//transfer_1(n);
		//printf("After Transfer:\n");
		//print_a(n);

		//transfer_2(n);
		//printf("After Transfer 2:\n");
		//print_a(n);
		
		vector<int> vec;
		//find_sub(a[1], a[2], n, 7, vec);
		if (vec.size() > 0 )
		{
			printf("After find_sub: \n");
			//print_a(n);
			for(int k = 0; k < vec.size(); k++)
				cout << vec[k] << ", ";
			cout << endl;
		}
		else	
			printf("Not Find result\n");
		//a[][],
		if(n%2 == 0)	// odd can make my goal
		{
			int xStart = n/2-1;
			int yStart = n/2-1;
			int c1 = a[xStart][yStart];
			int c2 = a[xStart][yStart+1];
			int c3 = a[xStart+1][yStart];
			int c4 = a[xStart+1][yStart+1];
			int x, y;
			cout << "c1, c2, c3, c4 = " << c1 << ", " << c2 << ", " << c3 << ", " << c4 << endl;
			find_index(19, n, x, y);
			cout << "19@ = " << x << ", " << y << endl;
			vec.clear();
			int arr1[256] = {0};
			int arr2[256] = {0};
			//x compute
			int nCount1 = 0;
			int nCount2 = 0;
			cout << "compute test start " << endl;
			vector<int> vecCompute = compute_col(a[x], a[xStart], n, 19-c1, 1);
			cout << "compute test end " << endl;
			for(int k = 0; k < n; k++){
				//arr1
				if (k != x && (k+x) != n-1)
				{
					arr1[nCount1] = a[x][k];
					nCount1++;
				}
				//arr2
				if (k != xStart && (k+xStart) != n-1)
				{
					arr2[nCount2] = a[xStart][k];
					nCount2++;
				}
			}	
			cout << "nCount1, nCount2: " << nCount1 << ", " << nCount2 << endl;
			find_sub(arr1, arr2, nCount1, c1-19, vec);
			
			if (vec.size() > 0 )
			{
				printf("After find_sub: \n");
				for(int k = 0; k < vec.size(); k++)
					cout << vec[k] << ", ";
				cout << endl;
			}
			else	
				printf("Not Find result\n");
		} 
	}
	//
	//printf("Test Sub Set:\n");
	//test_set(); 
	return 0;
}

vector<int> compute_col(int arrA[], int arrB[], int n, int sub, bool tag)
{
	int c[256] = {0};
	int d[265] = {0};
	int count = 0;
	for(int i = 0; i < n; i++)
	{
		if (tag && (i == 4 || i == 5))
			continue;
		c[count] = arrA[i];
		d[count] = arrB[i];
		count++;
	}

	vector<int> vec;
	find_sub(c, d, count, sub, vec);
	cout << "coupute_col, size: " << vec.size() << endl;
	return vec;
}

int cmp_arr(int n, int srcX, int srcY, int dstX, int dstY)
{
	
			vec.clear();
			int arr1[256] = {0};
			int arr2[256] = {0};
			//x compute
			int nCount1 = 0;
			int nCount2 = 0;
			for(int k = 0; k < n; k++){
				//arr1
				if (k != x && (k+x) != n-1)
				{
					arr1[nCount1] = a[x][k];
					nCount1++;
				}
				//arr2
				if (k != xStart && (k+xStart) != n-1)
				{
					arr2[nCount2] = a[xStart][k];
					nCount2++;
				}
			}	
			cout << "nCount1, nCount2: " << nCount1 << ", " << nCount2 << endl;
			find_sub(arr1, arr2, nCount1, c1-19, vec);
			
			if (vec.size() > 0 )
			{
				printf("After find_sub: \n");
				for(int k = 0; k < vec.size(); k++)
					cout << vec[k] << ", ";
				cout << endl;
			}
			else	
				printf("Not Find result\n");
}

// find nNum position, and return pos(x, y);
int find_index(int nNum, int n, int &x, int &y)
{
	for (int i = 0; i < n; i++)
		for(int j = 0; j < n;j ++)
			if (a[i][j] == nNum)
			{
				x = i;
				y = j;
			}
	return 0;
}

void print_a(int n)
{
	int i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		//printf("%5d",a[i][j]);
		printf("\t%d",a[i][j]);
		printf("\n");
	}
}

int check(int n)//检验是否是幻方
{
	int i,j,sum1=0,sum2;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			sum1+=a[i][j];
		if(sum1!=sum) return 0;
		sum1=0;
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			sum1+=a[i][j];
		if(sum1!=sum) return 0;
		sum1=0;
	}
	for(sum1=0,sum2=0,i=0,j=0;i<n;i++,j++)
	{
		sum1+=a[i][j];
		sum2+=a[i][n-j-1];
	}
	if(sum1!=sum) return 0;
	if(sum2!=sum) return 0;
	else return 1;
}

void ins(int n)//单偶数幻方的输入
{
	int x,y,m;
	x=0;
	y=n/2;
	for(m=1;m<=n*n;m++)
	{
		a[x][y]=m;
		if(m%n!=0)
		{
			x--;y++;
			if(x<0) x=x+n;
			if(y==n) y=n-y;
		}
		else
		{
			x++;
			if(x==n) x=x-n;
		}
	}

}

int find_sub(int a[], int b[], int n, int sub, vector<int> &vec)
{
	int len;
	vector<int> MySet(n);
	cout << "n, sub = " << n << ", " << sub << endl;
	//
	time_t t_start = time(NULL);
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << ", " << b[i] << endl;
	}
	for( int i=0; i<n; ++i )
	{
		MySet[i] = i;
	};
	vector<vector<int> > Sub = SubSet( MySet );
	
	printf("Sub size: %d\n", Sub.size());
	for( int i=0; i<static_cast<int>(Sub.size()); ++i )
	{
		int nSumA = 0;
		int nSumB = 0;
		//cout << "{";
		for( int j=0; j<static_cast<int>(Sub[i].size()); ++j )
		{
			int index = Sub[i][j];
			nSumA += a[index];
			nSumB += b[index];
			
			//cout << index;
			//if( (j!=static_cast<int>(Sub[i].size()-1)) )
			//	cout << ", ";
		}
		//cout << "}" << ", nSumA = " << nSumA << ", nSumB = " << nSumB << endl;
		// include(+, -)
		if (nSumA - nSumB == sub){
			vec = Sub[i];
			break;
		}
	};
	time_t t_end = time(NULL);

	cout << "find sun cost time: " << difftime(t_end, t_start) << "(s)" << endl;
	return true;
}

int transfer_2(int n)
{
	int i, j;
	int nTemp;
	int v = n-1;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			//
			if(i <= j){
				nTemp = a[i][j];
				a[i][j] = a[j][i];
				a[j][i] = nTemp;	
			}
		}
	}
}

bool IsSubSetOnList( const vector<vector<int> > Src, const vector<int>& Sub )
{
	for( int i=0; i<static_cast<int>(Src.size()); ++i )
	{
		if( Src[i].size() != Sub.size() )
			continue;
		bool IsEqual = true;
		for( int j=0; j<static_cast<int>(Src[i].size()); ++j )
		{
			if( Src[i][j] != Sub[j] )
				IsEqual = false;
		};
		if( IsEqual == true )
			return true;
	};
	return false;
};

vector<vector<int> > SubSet( const vector<int>& SrcSet )
{
	if( SrcSet.size() == 1 ) // 只有一个元素则返回，结束递归
		return vector<vector<int> >( 1, vector<int>( 1, SrcSet[0] ) );
	// 用于返回的子集集
 	vector<vector<int> > ret;
 	int Count = static_cast<int>(SrcSet.size() );
 	// 对每个元素
 	for( int i=0; i<Count; ++i )
 	{
 		// 剔除该元素生成Count-1的子集
 		vector<int> Src( Count -1 );
 		for( int j=0; j<(Count-1); ++j )
 		{
 			if( j<i )
 				Src[j] = SrcSet[j];
 			else
 				Src[j] = SrcSet[j+1];
 		};
 		// 加入该子集
 		ret.push_back( Src );
 		// 调用递归函数求该子集的所有子集。
 		vector<vector<int> > Sub = SubSet( Src );
 		for( int j=0; j< static_cast<int>(Sub.size()); ++j )
 		{
 			// 如果子集已在返回列表中，不加入
 			if( IsSubSetOnList( ret, Sub[j] ) )
 				continue; // 
			//否则加入该子集
			ret.push_back(Sub[j]);
		}; 
	}
	// 自己也是一个子集
 	ret.push_back( SrcSet );
 	return ret;
 };

void test_set()
{
	vector<int> MySet(5);
	for( int i=0; i<5; ++i )
	{
		MySet[i] = i;
	};
	vector<vector<int> > Sub = SubSet( MySet );
	cout << Sub.size() << endl;
	for( int i=0; i<static_cast<int>(Sub.size()); ++i )
	{
		cout << "{ ";
		for( int j=0; j<static_cast<int>(Sub[i].size()); ++j )
		{
			cout << Sub[i][j];
			if( (j!=static_cast<int>(Sub[i].size()-1)) )
				cout << ", ";
		}
		cout << " }";
		cout << endl;
	};
	system( "pause" );
};
