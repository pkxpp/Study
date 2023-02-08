#include   <iostream> 
#include   <iomanip> 

using   std::cout; 
using   std::cin; 

/* 
                name: 
                argumens: 
                return:number   of   string   changed 
  */ 
int           getutfchar(unsigned   int   n,   char*   s){ 
/* 
U-00000000   -   U-0000007F:     0xxxxxxx 
U-00000080   -   U-000007FF:     110xxxxx   10xxxxxx 
U-00000800   -   U-0000FFFF:     1110xxxx   10xxxxxx   10xxxxxx 
U-00010000   -   U-001FFFFF:     11110xxx   10xxxxxx   10xxxxxx   10xxxxxx 
U-00200000   -   U-03FFFFFF:     111110xx   10xxxxxx   10xxxxxx   10xxxxxx   10xxxxxx 
U-04000000   -   U-7FFFFFFF:     1111110x   10xxxxxx   10xxxxxx   10xxxxxx   10xxxxxx   10xxxxxx 
*/ 
                static   const   int   __character_offset[]   =   {7,11,16,21,26,31}; 
                static   const   int   __type_prefix[]   =   {0x00,0xc0,0xe0,0xf0,0xfa,0xfc}; 
                int   type; 
                for(type   =   0;   n   >>   __character_offset[type]   &&   type   <   6;   type++); 
                if(type   ==   6)return   0; 
                for(int   i   =   type;   i   >   0   ;   --i){ 
                                int   r   =   n   -   ((n   >>   6)   <<   6); 
                                n   >>=   6; 
                                s[i]   =   0x80   |   r; 
                } 
                s[0]   =   __type_prefix[type]   |   n; 
                return   type   +   1; 
} 

int   main(){ 
                char   utfchar[6]= "ERROR"; 
                unsigned   int     utfnum; 
                do{ 
                                cout   <<   "U+ "; 
                                cin     >>   std::hex   >>   utfnum; 
                                utfchar[getutfchar(utfnum,utfchar)]   =   0; 
								cout   <<   std::hex << utfchar   <<   std::endl; 
                }while(utfnum); 

				getchar();/* stop here */
}