#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

enum {
	NPREF	= 2,
	NHASH	= 4093,	/* size of state hash table array */
	MAXGEN	= 10000,	/* maximum words generated */
};

enum {MULTIPLIER = 31};

typedef struct State State;
typedef struct Suffix Suffix;

struct State{	/* prefix + suffix list */
	char *pref[NPREF];	/* prefix words */
	Suffix	*suf;		/* list of suffixex */
	State	*next;		/* next in hash table */
};

struct Suffix{	/* list of suffixes */
	char	*word;	/* suffix */
	Suffix	*next;	/* next in list of suffixes */
};

State *statetab[NHASH];	/* hash table of states */

/* function declartion */
void add(char *prefix[NPREF], char *suffix);
void addsuffix(State *sp, char *suffix);

/* hash: */
unsigned int hash( char *s[NPREF])
{
	unsigned int h;
	unsigned char *p;
	int i;
	
	h = 0;
	for (i = 0; i < NPREF; i++)
		for (p = (unsigned char *) s[i]; *p != '\0'; p++)
			h = MULTIPLIER *h + *p;
	return h % NHASH;
}

/* lookup: */
/* returns pointer */
/* creation */
State* lookup(char *prefix[NPREF], int create)
{
	int i, h;
	State *sp;

	h = hash(prefix);
	for (sp = statetab[h]; sp != NULL; sp = sp->next){
		for (i = 0; i < NPREF; i++)
			if ( strcmp(prefix[i], sp->pref[i]) != 0)
				break;
		if (i == NPREF)		/* found it */
			return sp;
	}
	if (create){
		sp = (State *) malloc(sizeof(State));
		for (i = 0; i < NPREF; i++)
			sp->pref[i] = prefix[i];
		sp->suf = NULL;
		sp->next = statetab[h];		//insert into the first
		statetab[h] = sp;
	}
	return sp;
}

/* build: read input, */
void build(char *prefix[NPREF], FILE *f)
{
	char buf[100], fmt[10];
	
	/* create a format string; */
	sprintf(fmt, "%%%ds", sizeof(buf)-1);
	while (fscanf(f, fmt, buf) != EOF)
		add(prefix, (char*)strdup(buf));
}

/* add: add word to */
void add(char *prefix[NPREF], char *suffix)
{
	State *sp;
	//printf("[test]suffix: %s\n", suffix);
	
	sp = lookup(prefix, 1);		/* create if not found */
	addsuffix(sp, suffix);
	/* move the words down the prefix */
	memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
	prefix[NPREF-1] = suffix;
}

/* addsuffix: */
void addsuffix(State *sp, char *suffix)
{
	Suffix *suf;
	suf = (Suffix *) malloc(sizeof(Suffix));
	suf->word = suffix;
	suf->next = sp->suf;
	sp->suf = suf;
}

char NONWORD[] = "\n";	/* cannot appear as real word */

/* generate: produce ouput, */
void generate(int nwords)
{
	State *sp;
	Suffix *suf;
	char *prefix[NPREF], *w;
	int i, nmatch;
	
	for (i = 0; i < NPREF; i++)	/* reset initial prefix */
		prefix[i] = NONWORD;
	
	for (i = 0; i < nwords; i++){
		sp = lookup(prefix, 0);
		nmatch = 0;
		for (suf = sp->suf; suf != NULL; suf = suf->next){
			int nRand = rand();
			printf("nRand: %d\n", nRand);
			if (nRand % ++nmatch == 0)	/* prob = 1/nmatch */
				w = suf->word;
		}
		if (strcmp(w, NONWORD) == 0 )
			break;
		printf("%s\n", w);
		memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
		prefix[NPREF-1] = w;
	}

}

void print_prefix(char *pref)
{
	if (strcmp(pref, NONWORD) == 0 )
		printf("NONWORD");
	else
		printf("%s", pref);
}

//print State *statetab[NHASH];	/* hash table of states */
void print_tab()
{
	int i, h, n;
	State *sp;
	Suffix *suf;
	for (n = 0; n < NHASH; n++)
	{
		sp = statetab[n];
		if (sp == NULL)
			continue;
		printf("hash-%d:\n", n);
		for (; sp != NULL; sp = sp->next){
			printf("\tprefix:");
			for(i = 0; i < NPREF; i++)
				printf("%s, ", sp->pref[i]);
			printf("\n");
			printf("\tsuf: ");
			for(suf = sp->suf; suf != NULL; suf = suf->next)
				printf("%s, ", suf->word);
			printf("\n\n");
		}
	}
}

/* markov main: markov-chain random text generation */
int main(void)
{
	//srand(time(NULL));
	int i, nwords = MAXGEN;
	char *prefix[NPREF];		/* current input prefix */
	//string sentence = "Show your flowchars and conceal your tables and I will be mystified. Show your tables and your flowcharts will be obvious. (end)";
	//isstringstream in(sentence);
	for (i = 0; i < NPREF; i++)	/* set up initial prefix */
		prefix[i] = NONWORD;
	//build(prefix, in);
	build(prefix, stdin);
	add(prefix, NONWORD);
	//print_tab();
	generate(nwords);
	return 0;
}
