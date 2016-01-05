/******************************************
 * page 
 * modify
 * (1)2015/11/30 read code again, and rename to magic3.cpp
 ******************************************/
#include"stdio.h"
#include"math.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

int a[256][256];
int bak[256][256];
int aT[256][256];	//aT[i][j] = a[j]i];
int sum;
int g_nIgnoreTag = 0;
typedef struct tagPosition{
	int nX;
	int nY;
}Position;

int check(int n);
int check_simple(int n);
void ins(int n);
void print_a(int n);
void print_array(int arr[256][256], int n);
int transfer_1(int n);
int transfer_2(int n);
void bak_array(int n, int arr[256][256]);
void get_bak(int n, int nTag);
void array_transfer(int n);
void get_transfer(int n);

void test_set();
vector<vector<int> > SubSet( const vector<int>& SrcSet );
int find_sub(int a[], int b[], int n, int sub, vector<int> &vec);
int find_index(int nNum, int n, int &x, int &y);
//vector<int> compute_col(int arrA[], int arrB[], int n, int sub, bool tag);
vector<int> compute_col(int arrA[], int arrB[], int n, int sub, bool tag, vector<int> vecIgnore);
void swap_elem(int x1, int y1, int x2, int y2);
void swap_vec(int x1, int x2, vector<int> vec, int nTag);
bool exchange_elem(int n, int nNum, int dstX, int dstY, int nTag, vector<int> vecIgnore);
bool equal_transfor(int n, int srcX, int srcY, int srcNum, int dstX, int dstY, int nTag, vector<int> vecIgnore);
void get_ignore(int n, int n1, int n2, vector<int> &vecIgnore);
int sort_array(int src[256], int n);
int sort_diagnol(int nTag, int n, vector<int> &vecSort);
bool process_diagnol(int nTag, int n, vector<Position> vecPos);
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
	cout << "test sort" << endl;
	int nTest[256] = {1, 3, 2, 5};
	sort_array(nTest, 4);
	for(int i = 0; i < 4; i++)
		cout << nTest[i] << ", ";
	cout << endl;
	//test
	cout << "========================================================" << endl;
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
		//get aT
		for(int ia = 0; ia < n; ia++)
			for (int ja = 0; ja < n; ja++)
				aT[ia][ja] = a[ja][ia];
		cout << "aT: " << endl;
		print_array(aT, n);
			
		//test
		vector<int> testIgnore;
		get_ignore(n, 0, 5, testIgnore);
		for(int nTest = 0; nTest < testIgnore.size(); nTest++)
			cout << testIgnore[nTest] << ", ";
		cout << "end" << endl;
		
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
		if(n%2 == 0)
		{
			int xStart = n/2-1;
			int yStart = n/2-1;
			int c1 = a[xStart][yStart];
			int c2 = a[xStart][yStart+1];
			int c3 = a[xStart+1][yStart];
			int c4 = a[xStart+1][yStart+1];
			int x, y;
			cout << "c1, c2, c3, c4 = " << c1 << ", " << c2 << ", " << c3 << ", " << c4 << endl;
			
			vector<int> vecIgnore;
			vecIgnore.push_back(4);
			vecIgnore.push_back(5);
			//c1
			cout << "c1 start........................" << endl;
			exchange_elem(n, 19, xStart, yStart, 0, vecIgnore);
			//c2
			cout << "c2 start........................" << endl;
			exchange_elem(n, 88, xStart, yStart+1, 0, vecIgnore);
			//c3
			cout << "c3 start........................" << endl;
			exchange_elem(n, 11, xStart+1, yStart, 0, vecIgnore);

			//c4
			cout << "c4 start........................" << endl;
			exchange_elem(n, 28, xStart+1, yStart+1, 0, vecIgnore);
			cout << "After all check is: " <<  check(n) << endl;
			
			//
			int b[256][256] = {0};
			for(int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					b[i][j] = a[i][j];
			//vecIgnore.push_back();
			cout << "Additon start......... " << endl;
			g_nIgnoreTag = 1;
			vector<Position> vecPos;
			cout << "start process positive diagnol ... " << endl;
			//process_diagnol(0, n, vecPos);
			cout << "start process negative diagnol ... " << endl;
			//process_diagnol(1, n, vecPos);
			cout << "After all check is: " <<  check(n) << endl;

		} 
	}
	//
	//printf("Test Sub Set:\n");
	//test_set(); 
	return 0;
}

//nTag
vector<int> compute_col(int arrA[], int arrB[], int n, int sub, bool tag, vector<int> vecIgnore)
{
	int c[256] = {0};
	int d[265] = {0};
	int count = 0;
	vector<int> vecRelation;
	//cout << "compute_col-vecIgnore: " ;
	//for(int i = 0; i < vecIgnore.size(); i++)
	//	cout << vecIgnore[i] << ", ";	
	//cout << endl;	
	for(int i = 0; i < n; i++)
	{
		//vector<int>::iterator it = find(vecIgnore.begin(), vecIgnore.end(), i);
		bool bIsIgnore = false;
		for(int j = 0; j < vecIgnore.size(); j++)
		{
			if (vecIgnore[j] == i)
				bIsIgnore = true;
		}
		//if (tag && (i == 4 || i == 5))
		//if (tag && bIsIgnore)
		if ( bIsIgnore)
			continue;
		c[count] = arrA[i];
		d[count] = arrB[i];
		vecRelation.push_back(i);
		count++;
	}

	vector<int> vec;
	vector<int> vecRet;
	find_sub(c, d, count, sub, vec);
	//cout << "vec.size: " << vec.size() << ", vec = ";
	if (tag)
	{
		for(int i = 0; i < vec.size(); i++)
		{
			//cout << vec[i] << ", ";
			vecRet.push_back(vecRelation[vec[i]]);
			//if (vec[i] >= 4)
				//vec[i] += 2;
		}
	}
	//cout << endl;
	//cout << "coupute_col, size: " << vecRet.size() << ", result is: ";
	//for (int i = 0; i < vecRet.size(); i++)
	//	cout << vecRet[i] << ", ";
	//cout << endl;
	return vecRet;
}

void swap_elem(int x1, int y1, int x2, int y2)
{
	int nTemp = a[x1][y1];
	a[x1][y1] = a[x2][y2];
	a[x2][y2] = nTemp;
}

void swap_vec(int x1, int x2, vector<int> vec, int nTag)
{
	int (*p)[256] = a;
	if (nTag == 1)
		p = aT;
	//cout << "swap_vec " << x1 << ", " << x2 << ", " << vec.size() << endl;
	for(int i = 0; i < vec.size(); i++){
		int index = vec[i];
		int nTemp = p[x1][index];
		p[x1][index] = p[x2][index];
		p[x2][index] = nTemp;
	}
}
/*
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
}*/


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
		if((i==4)&&(j==4 || j==5) || (i==5 && (j==4 || j==5)))
			printf("\t\033[31m%d\033[0m",a[i][j]);
		else
			printf("\t%d",a[i][j]);
		printf("\n");
	}
}

void print_array(int arr[256][256], int n)
{
	int i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		if((i==4)&&(j==4 || j==5) || (i==5 && (j==4 || j==5)))
			printf("\t%d",arr[i][j]);
		else
			printf("\t%d",arr[i][j]);
		printf("\n");
	}
}

void array_transfer(int n)
{
	int i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			aT[i][j] = a[j][i];
	}
}

void get_transfer(int n)
{
	int i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			a[i][j] = aT[j][i];
	}
}



int check(int n)//检验是否是幻方
{
	int i,j,sum1=0,sum2;
	// row
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			sum1+=a[i][j];
		if(sum1!=sum) return 0;
		sum1=0;
	}
	// col
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			sum1+=a[i][j];
		if(sum1!=sum) return 0;
		sum1=0;
	}
	// dialgonal
	for(sum1=0,sum2=0,i=0,j=0;i<n;i++,j++)
	{
		sum1+=a[i][j];
		sum2+=a[i][n-j-1];
	}
	cout << "sum1, sum2, sum = " << sum1 << ", " << sum2 << ", " << sum << endl;
	if(sum1!=sum) return 0;
	if(sum2!=sum) return 0;
	else return 1;
}

int check_simple(int n)//检验是否是幻方
{
	int i,j,sum1=0,sum2;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++){
			sum1+=a[i][j];
			//cout << a[i][j] << ",";
		}
		//cout << "i, sum1: " << i << ", " << sum1 << endl;
		if(sum1!=sum) return 0;
		sum1=0;
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++){
			sum1+=a[j][i];
			//sum1+=a[i][j];
			//cout << a[j][i] << ", ";
		}
		//cout << "i, sum1: " << i << ", " << sum1 << endl;
		if(sum1!=sum) return 0;
		sum1=0;
	}
	for(sum1=0,sum2=0,i=0,j=0;i<n;i++,j++)
	{
		sum1+=a[i][j];
		sum2+=a[i][n-j-1];
	}
	//if(sum1!=sum) return 0;
	//if(sum2!=sum) return 0;
	//else return 1;
	return 1;
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
	//cout << "n, sub = " << n << ", " << sub << endl;
	//
	time_t t_start = time(NULL);
	for (int i = 0; i < n; i++)
	{
		//cout << a[i] << ", " << b[i] << endl;
	}
	for( int i=0; i<n; ++i )
	{
		MySet[i] = i;
	};
	vector<vector<int> > Sub = SubSet( MySet );
	
	//printf("Sub size: %d\n", Sub.size());
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
		//cout << "}" << ", nSumA = " << nSumA << ", nSumB = " << nSumB << ",sub = " << sub << ",ret: " << nSumA - nSumB << endl;
		// include(+, -)
		if (nSumA - nSumB == sub){
			// test
			cout << ", nSumA = " << nSumA << ", nSumB = " << nSumB << ",sub = " << sub << endl;
			// test end
			vec = Sub[i];
			break;
		}
	};
	time_t t_end = time(NULL);

	//cout << "find sun cost time: " << difftime(t_end, t_start) << "(s)" << endl;
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

/* calc subset of a set */
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

void bak_array(int n, int arr[256][256])
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			bak[i][j] = arr[i][j];	
}

void get_bak(int n, int nTag)
{
	int (*p)[256] = a;
	if (nTag == 1)
		p = aT;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			p[i][j] = bak[i][j];	
}
/*
 * n: array num;
 * nNum: the number which is i need;
 * dstX: nNum move to posX
 * dstY: nNum move to posY
 * nTag: not used again
 * vecIgnore: 
 */
bool exchange_elem(int n, int nNum, int dstX, int dstY, int nTag, vector<int> vecIgnore)
{
	nTag = 0;		// rol
	int x, y;
	int (*p)[256]= a;
	//p = aT;
	
	int dstNum = a[dstX][dstY];
	vector<int> vecCompute;
	find_index(nNum, n, x, y);
	if (x == dstX && y == dstY)
		return true;	
	cout << "swap:(" 
		<< x << ", " 
		<< y << ")-->(" 
		<< dstX << ", " << dstY << ")" << endl;
	swap_elem(dstX, dstY, x, y);
	cout << "after swap: " << endl;
	print_a(n);
	//rol
	bool bRet = false;
	bool bRet1 = equal_transfor(n, x, y, dstNum, dstX, dstY, 0, vecIgnore);
	if ( !bRet1){
		get_bak(n, 0);
		return bRet1;
	}
	bool bRet2 = equal_transfor(n, x, y, dstNum, dstX, dstY, 1, vecIgnore);
	if ( !bRet2)
		get_bak(n, 1);
	return (bRet1 && bRet2);
}

/* 
 * srcNum: is the number which will be replaced
 * nTag: 0-swap with rol;1-swap with col;
 */
bool equal_transfor(int n, int srcX, int srcY, int srcNum, int dstX, int dstY, int nTag, vector<int> vecIgnore)
{
	//nTag = 0;		// rol
	int x, y;
	int (*p)[256]= a;
	
	int nDstNum = srcNum;		// before swap:because after transfer rol then col the num has changed!
	int nSrcNum = a[dstX][dstY];	// this is the number i need. because i have swap two number;
	cout << "nDstNum, nSrcNum, nTag: " << nDstNum << ", " << nSrcNum << ", " << nTag << endl;
	vector<int> vecCompute;
	//find_index(nNum, n, x, y);
	if (srcX == dstX && srcY == dstY)
		return true;	
	//swap_elem(dstX, dstY, x, y);
	//cout << "after swap: " << endl;
	//print_a(n);
	x = srcX;
	y = srcY;
	int nDstIndex = dstX;
	int nSrcIndex = x;
	if (nTag == 1){
		nDstIndex = dstY;
		nSrcIndex = y;
	}
	vector<int> vecElem;
			
	cout << "x, y, dstX, dstY = " << x << "," << y << "," << dstX << "," << dstY << endl;
	//decide is col or row
	for( int i=0; i<n; ++i )
	{
		if (nTag == 1){
			//cout << "i: " << i << "i !=y " << (i!=y) << "i != dstY " << (i != dstY) << endl;
			if(i != y && i != dstY){
				vecElem.push_back(i);
				//cout << "i=" << i << "y, dstY" << x << ", " << dstX << "vecElem.size: " << vecElem.size() << endl;
			}
		}
		else{
			//cout << "i: " << i << "i != " << (i!=x) << "i != dstX " << (i != dstX) << endl;
			if(i != x && i != dstX){
				vecElem.push_back(i);
				//cout << "i=" << i << "x, dstX" << x << ", " << dstX << "vecElem.size: " << vecElem.size() << endl;
			}
		}
	};
	if (nTag == 1){
		array_transfer(n);
		p = aT;
	}
	cout << "vecElem.size: " << vecElem.size() << endl;
	vector<vector<int> > SubElem = SubSet( vecElem);
	cout << "SubElem size:" << SubElem.size() << endl;
	cout << "nSrcIndex, nDstIndex: " << nSrcIndex << ", " << nDstIndex << endl;
	bak_array(n, p);
	print_array(p, n);
	bool bFind = false;
	//vector<int> vecIgnore;		
	//vecIgnore.push_back(4);
	//vecIgnore.push_back(5);
	// method: 1->swap all subset; 2->swap line by line
	//for( int i=0; i<static_cast<int>(SubElem.size()); ++i )
	for( int i=0; i<static_cast<int>(vecElem.size()); ++i )
	{
		//cout << "{ ";
		get_bak(n, nTag);
		int nSubSize = static_cast<int>(SubElem[i].size());
		int n1 = nSrcIndex;
		if (nTag == 1)
			n1 = y;
		int n2 = 0;
		//cout << "new sub set: " << nSubSize << ", {" ;
		//for(int ii = 0; ii < nSubSize;ii++)
		//	cout << SubElem[i][ii] << ", ";
		//cout << "}" << endl;
		vecCompute.clear();
		//for( int j=0; j<static_cast<int>(SubElem[i].size()); ++j )
		if (true)
		{	
			// method 1
			//int nIndex = SubElem[i][j];
			//n2 = nIndex;
			
			// method 2
			n2 = vecElem[i];
			//cout << "\tj=" << j << endl;
			vecCompute.clear();
			//vecIgnore.clear();
			if (g_nIgnoreTag == 1)
				get_ignore(n, n1, n2, vecIgnore);
			//cout << "n1, n2, check: " << n1 << ", " << n2 << ", " << check_simple(n) << endl;
			//if (j == 0 || n1 == nSrcIndex ){
			if ( n1 == nSrcIndex ){
				vecCompute = compute_col(p[n1], p[n2], n, nDstNum-nSrcNum, 1, vecIgnore);
			}
			//else if(j == nSubSize - 1)
			//	vecCompute = compute_col(a[nIndex], a[xStart], n, c2-91, 1);
			else
				vecCompute = compute_col(p[n1], p[n2], n, nDstNum-nSrcNum, 0, vecIgnore);
			
			if (vecCompute.size() > 0 ){
				swap_vec(n1, n2, vecCompute, nTag);
				n1 = n2;
				//break;
			}
			n2 = n1;
			//cout << SubElem[i][j];
			//if( (j!=static_cast<int>(SubElem[i].size()-1)) )
			//	cout << ", ";
		}
		//cout << " }";
		//cout << endl;
		vecCompute.clear();
		//cout << "exchange (" << n2 << ")<-->(" << nDstIndex << ")" << endl;
		if (g_nIgnoreTag == 1)
			get_ignore(n, n2, nDstIndex, vecIgnore);
		vecCompute = compute_col(p[n2], p[nDstIndex], n, nDstNum-nSrcNum, 1, vecIgnore);
		if (vecCompute.size() > 0 ){
			swap_vec(n2, nDstIndex, vecCompute, nTag);
			cout << "I Find it at last!!!" << endl;
			bFind = true;
			break;
		}
	};
	cout << "FindResult: " << bFind << ", nRrcNum: " << nSrcNum << ", size: " << vecCompute.size() << endl;
	if ( !bFind)
		get_bak(n, nTag);

	//print_a(n);
	//cout << "After check is: " <<  check_simple(n) << ", nTag = " << nTag << endl;
	//cout << "................................" << endl;
	if (nTag == 1)
		get_transfer(n);
	print_a(n);
	cout << "After check is: " <<  check_simple(n) << ", nTag = " << nTag << endl;
	return bFind;
}

void get_ignore(int n, int n1, int n2, vector<int> &vecIgnore)
{
	int nTemp[10] = {0};
	int nCount = 0;
	vecIgnore.clear();
	nTemp[nCount++] = n1;
	nTemp[nCount++] = n-1-n1;
	nTemp[nCount++] = n2;
	nTemp[nCount++] = n-1-n2;
	if(n1 == 4 || n1 == 5 || n2 == 4 || n2 == 5)
	{
		nTemp[nCount++] = 4;
		nTemp[nCount++] = 5;
	}
	for (int i = 0; i < nCount; i++)
	{
		bool bIsExist = false;
		for (int j = 0; j < vecIgnore.size(); j++)
		{
			if (vecIgnore[j] == nTemp[i])	
			{
				bIsExist = true;
				break;
			}
		}
		if (!bIsExist)
			vecIgnore.push_back(nTemp[i]);
	}
	//cout << "get_ignore(n1, n2) " << n1 << ", " << n2 << ":";
	//for (int j = 0; j < vecIgnore.size(); j++)
	//	cout << vecIgnore[j] << ", ";
	//cout << endl;
}

int sort_array(int src[256], int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = i+1; j < n; j++)
		{
			if(src[i] > src[j])
			{
				int nTemp = src[i];
				src[i] = src[j];
				src[j] = nTemp;
			}
		}
	}
}

int sum_array(int nTag, int n)
{
	int nSum = 0;
	for(int i = 0; i < n; i++)
	{
		int j = i;
		if (nTag == 1)
			j = n-i-1;
		nSum += a[i][j];
	}
	return nSum;
}

int sort_diagnol(int nTag, int n, vector<int> &vecSort){
	int i = 0;
	int j = 0;
	//
	//if (nTag == 0)
	//	j = i;
	//else if (nTag == 1)
	//	j = n-i-1;	
	int nSrc[256] = {0};
	for(int k = 0; k < n; k++)
	{
		if (nTag == 0)
			j = k;
		else if (nTag == 1)
			j = n-k-1;
		nSrc[k] = a[k][j];
	}
	sort_array(nSrc, n);
	
	for(int i = 0; i < n; i++)
	{
		for(int k = 0; k < n; k++)
		{
			if(nTag == 0 )
				j = k;
			else if (nTag == 1)
				j = n-k-1;
			if(a[k][j] == nSrc[i])
				vecSort.push_back(k);	//k is a[k][] index
		}
	}
	cout << "After sort is: " << endl;
	for(i = 0; i < n; i++)
		cout << vecSort[i] << ", ";
	cout << endl;
}

//
//nTag: 0-Positive and 1-negative diagnol
bool process_diagnol(int nTag, int n, vector<Position> vecPos)
{
	vector<int> vecSort;
	sort_diagnol(nTag, n, vecSort);
	vector<int>::iterator it = vecSort.begin();
	vector<int> vecIgnore;
	bool bFind = false;
	int nSum = sum_array(nTag, n);
	if(nSum < 605 && nSum >405)
		bFind = true;
	int b[256][256] = {0};
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			b[i][j] = a[i][j];
	
	//bak_array(n, a);
	for(;it != vecSort.end() && !bFind; it++)
	{
		int nIndex = *it;
		// target position is vecPos
		if (nIndex == 4 || nIndex == 5)
			continue;
		int nMax = 100;
		int nTarget = a[nIndex][nIndex];
		int nIndexY = nIndex;
		if (nTag == 1){
			nTarget = a[nIndex][n-nIndex-1];
			nIndexY = n-nIndex-1;
		}
		int nExchange = nMax;
		bool bSubFind = false;
		//bak temp
		int bakTemp[256][256] = {0};
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				bakTemp[i][j] = a[i][j];
		//bak temp end
		for(; nExchange > nTarget; nExchange--)
		{
			// nExchange not in target position
			int x, y;
			find_index(nExchange, n, x, y);
			if( (x==4 || x == 5) && (y==4 ||  y == 5))
				continue;
			bool bRet = exchange_elem(n, nExchange, nIndex, nIndexY, 0, vecIgnore);
			if (bRet)
			{
				bSubFind = true;
				break;
			}
			else{
				cout << nExchange << " not find, and next" << endl;
				//for(int i = 0; i < n; i++)
				//	for(int j = 0; j < n; j++)
				//		a[i][j] = b[i][j];
				for(int i = 0; i < n; i++)
					for(int j = 0; j < n; j++)
						a[i][j] = bakTemp[i][j];
			}
		}
			
		int nSum = sum_array(nTag, n);
		bool bRet = false;
		cout << "Binggo! bSubFind: " << bSubFind << ", nSum: " << nSum << endl;
		print_a(n);
		cout << "After all check is: " <<  check(n) << endl;
		if (bSubFind){
			int nThreshHold = 20;
			if (nSum > 505-nThreshHold && nSum < 505+nThreshHold){
				//bFind = true;
				//////////////////////////////////////
				cout << "Additon start......... " << "nSum = " << nSum << endl;
				g_nIgnoreTag = 1;
				int nNeed = 0;
				int bb[256][256] = {0};
				for(int i = 0; i < n; i++)
					for (int j = 0; j < n; j++)
						bb[i][j] = a[i][j];
				for(int k = 0; k < n; k++){
					if (k == 4 || k == 5 )
						continue;
					//exchange_elem(n, a[0][n-1]-2, 0, n-1, 0, vecIgnore);
					for(int i = 0; i < n; i++)
						for (int j = 0; j < n; j++)
						a[i][j] = bb[i][j];
					int nIndexY = k;
					if(nTag == 1)
						nIndexY = n-k-1;
					int nNeed = a[k][nIndexY] + 505 - nSum;
					cout << "nNeed: " << nNeed << endl;
					//if (nSum < 505)
					//	nNeed = a[k][nIndexY] + nNeed;
					//else
					//	nNeed = nNeed + a[k][nIndexY];
					if (nNeed <=0 || nNeed > 100)
						continue;
					//not in p/n diagnol
					int nX, nY;
					find_index(nNeed, n, nX, nY);
					cout << "nX, nY: " << nX << ", " << nY << endl;
					if ( (nX == 4 || nX == 5) && (nY ==4 && nY == 5))
						continue;

					if (nX == nY || nY == n-nX-1)
						continue;
					cout << "Addition: " << "nNeed = " << nNeed << ", " << k << ", a[][] = " << endl;
					print_a(n);
					bRet = false;
					bRet = exchange_elem(n, nNeed, k, nIndexY, 0, vecIgnore);
					cout << "After all check is: " <<  check(n) << endl;
					if (bRet)	
					{
						cout << "The End and result is " << bRet << endl;
						bFind = true;
						break;
					}	
				}
				cout << "check all is: " <<  check(n) << endl;
				//////////////////////////////////////
				//break;
			}
		}
		else 	// not find bak a
		{
			for(int i = 0; i < n; i++)
				for(int j = 0; j < n; j++)
					a[i][j] = b[i][j];
		}
		if(bRet){
			bFind = true;
			break;
		}
		//else if(!bSubFind)	// not find bak a
		//{
			//get_bak(n, 0);	
		//	for(int i = 0; i < n; i++)
		//		for(int j = 0; j < n; j++)
		//			a[i][j] = b[i][j];
		//}
		cout << "End a circle, index: " << nIndex << ", bSubFind: " << bSubFind << endl;
	
	}
	bool bEnd = false;
	if (bFind)
	{
		print_a(n);
		//cout << "the last check is: " <<  check_simple(n) << endl;
		//bak_array(n, a);
		//bool bRet = false;
		//for(it = vecSort.begin(); it != vecSort.end(); it++)
		//{
		//	get_bak(n, 0);
		//	int nIndex = *it;
		//	int nIndexY = nIndex;
		//	if (nTag == 1)
		//		nIndexY = n-nIndex-1;
		//	bRet = exchange_elem(n, a[nIndex][nIndexY], nIndex, nIndexY, 0, vecIgnore);
		//	if(bRet){
		//		bEnd = true;
		//		break;
		//	}
		//}	
	}
	if(bEnd)
	{
		cout << "Find it " << endl;
	}
	else
		cout << "Not Find the end!" << endl;
	///////////////////////////////////////////////////////////////
}
// g++ magic3.cpp -o magic
//./magic | tee log


