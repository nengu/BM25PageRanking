#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
//#include "HashTableVoid.h"

using namespace std;

int TableSize = 200000;

struct HashTableVoidEntry {
  const char * _key;
  int docid;
  int tf;
  HashTableVoidEntry * _next;
};

class HashTableVoid {
 public:
  //enum { TableSize = 20000};
  HashTableVoidEntry **_buckets;
  int hash(const char * key); 
 public:
  HashTableVoid();
  int insertItem( const char * key, int docid);
  int findFi( const char * key, int docid);
};

HashTableVoid::HashTableVoid()
{
	_buckets=(HashTableVoidEntry **)malloc(TableSize*sizeof(HashTableVoidEntry *));
	for(int i=0;i<TableSize;i++){
		_buckets[i]=NULL;
	}
}

int HashTableVoid::insertItem(const char * key,int docid)
{
	int h=hash(key);
	HashTableVoidEntry *e=_buckets[h];
	while(e!=NULL&&docid!=e->docid){
		e=e->_next;
	}
	
	if(e!=NULL){
		//printf("Already have! Override TF!\n");
		if(e->docid==docid)
		{e->tf++;}
		return 1;
	}
	//printf("New Item Added\n");
	e=new HashTableVoidEntry;
	e->_key=strdup(key);
	e->docid=docid;
	e->tf=1;
	e->_next=_buckets[h];
	_buckets[h]=e;
  	return 0;
}


int HashTableVoid::findFi(const char * key,int docid)
{
	int h=hash(key);
	HashTableVoidEntry *e=_buckets[h];
	while(e!=NULL&&docid!=e->docid){
		e=e->_next;
	}
	
	if(e!=NULL){
		//printf("Already have! Override TF!\n");
		//if(e->docid==docid)
		//{e->tf++;}
		return e->tf;
	}
  	return 0;
}


int HashTableVoid::hash(const char * key)
{
	int sum=0;
	int len=strlen(key);
	for(int i=0; i<len; i++){
		sum +=(i+1)*key[i];
	}
	int h=sum%TableSize;
  	return h;
}

float avdl=0.0;
int docNum=0;
int dl[3300]={0};
float k1=1.2;
float b=0.75;
float k2=100;
int n[200000]={0};
float bmResult[3300][2]={0};
int wordTotal=0;
int u=5000;
  //since we know the k1k2value and each word in query has qfi value =1, we can 
  //simplify (k2+1)*qfi/(k2+qfi) to 1
  float getBM(int ni,int fi,float k){
  	return (log(1/((ni+0.5)/( (float)(docNum-ni) + 0.5)))	*	((2.2*fi)/(k+fi)));
  }
  
  float getQL(int fi,int cqi,int ith){
  	return (	log(	(fi+u*(float)(cqi/wordTotal))	/ 	(float)(dl[ith]+u)) );
  }



int qhash(const char * key)
{
	int sum=0;
	int len=strlen(key);
	for(int i=0; i<len; i++){
		sum +=(i+1)*key[i];
	}
	int h=sum%TableSize;
  	return h;
}


int main()
{
  int flag;
  int theDocID=0;
  char	* str;
  char line[160];
  HashTableVoid hs;
  FILE *file=fopen("tccorpus.txt","r");	
  while(fgets(line,sizeof(line),file)!=NULL) 
  {
	str=strtok(line," ");
	if(strcmp(str,"#")==0)
		{
			str=strtok(NULL," \n");
			theDocID=atoi(str);
			//printf(">>>>>>>>>>>>>>>%d\n",theDocID);
			docNum++;
			continue;
		}
	else{ 
		    while (str!= NULL)
  			{
    				//printf ("%s\n",str);
    				if(str){
    				flag=hs.insertItem(str,theDocID);
    				n[qhash(str)]++;
    				dl[theDocID]++;
    				wordTotal++;
    				/*if(flag==0)
						{
							n[qhash(str)]++;
						}*/
					}
					str=strtok(NULL," \n");	
  			}
	}
  }
  int sumdl=0;
  for(int i=1;i<=docNum;i++)
  {sumdl+=dl[docNum];}
  avdl=sumdl/docNum;
  
  float k=0.0;
  float u=2000;
  int fi=0;
  	//Query 1
  	//portabl oper system
	 for(int i=1;i<=docNum;i++)
  	{   //k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=	getQL(hs.findFi("portabl",i),n[qhash("portabl")],i);
  		bmResult[i][0]+=getQL(hs.findFi("oper",i)	,n[qhash("oper")] 	,i);
  		bmResult[i][0]+=getQL(hs.findFi("system",i)	,n[qhash("system")]	,i);
  	}
	

	//Query 2 
	//code optim for space effici
	/*
	for(int i=1;i<=docNum;i++)
  	{   k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=getBM(n[qhash("code")] , hs.findFi("code",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("optim")] , hs.findFi("optim",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("for")] , hs.findFi("for",i) , k  );
		bmResult[i][0]+=getBM(n[qhash("space")] , hs.findFi("space",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("effici")] , hs.findFi("effici",i) , k  );
  	}
	*/
	
	//Query 3 
	//parallel algorithm 
	/*for(int i=1;i<=docNum;i++)
  	{   k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=getBM(n[qhash("parallel")] , hs.findFi("parallel",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("algorithm")] , hs.findFi("algorithm",i) , k  );
  	}
	*/
	
	//Query 4 
	//distribut comput structur and algorith
	/*for(int i=1;i<=docNum;i++)
  	{   k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=getBM(n[qhash("distribut")] , hs.findFi("distribut",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("comput")] , hs.findFi("comput",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("structur")] , hs.findFi("structur",i) , k  );
		bmResult[i][0]+=getBM(n[qhash("and")] , hs.findFi("and",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("algorith")] , hs.findFi("algorith",i) , k  );
  	}*/
	
	//Query 5 
	//appli stochast process 
	/*for(int i=1;i<=docNum;i++)
  	{   k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=getBM(n[qhash("appli")] , hs.findFi("appli",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("stochast")] , hs.findFi("stochast",i) , k  );
		bmResult[i][0]+=getBM(n[qhash("process")] , hs.findFi("process",i) , k  );
  	}*/
	
	//Query 6 
	//perform evalu and model of comput system
	/*for(int i=1;i<=docNum;i++)
  	{   k=1.2*(0.25+0.75*(dl[i]/avdl));
  		bmResult[i][1]=i;
  		bmResult[i][0]=getBM(n[qhash("perform")] , hs.findFi("perform",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("evalu")] , hs.findFi("evalu",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("and")] , hs.findFi("and",i) , k  );
		bmResult[i][0]+=getBM(n[qhash("model")] , hs.findFi("model",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("of")] , hs.findFi("of",i) , k  );
		bmResult[i][0]+=getBM(n[qhash("comput")] , hs.findFi("comput",i) , k  );
  		bmResult[i][0]+=getBM(n[qhash("system")] , hs.findFi("system",i) , k  );
  	}*/
	//Query 7 
	//parallel processor in inform retriev
	  /*for(int i=1;i<=docNum;i++)
		{   k=1.2*(0.25+0.75*(dl[i]/avdl));
			bmResult[i][1]=i;
			bmResult[i][0]=getBM(n[qhash("parallel")] , hs.findFi("parallel",i) , k  );
			bmResult[i][0]+=getBM(n[qhash("processor")] , hs.findFi("processor",i) , k  );
			bmResult[i][0]+=getBM(n[qhash("in")] , hs.findFi("in",i) , k  );
			bmResult[i][0]+=getBM(n[qhash("inform")] , hs.findFi("inform",i) , k  );
			bmResult[i][0]+=getBM(n[qhash("retriev")] , hs.findFi("retriev",i) , k  );
		}*/
  ///////////////sort
  	for(int i=2;i<=3300;i++){
  		for(int j=1;j<=3300-i;j++){
  			if(bmResult[j][0]<bmResult[j+1][0])
  			{
  				float temp=bmResult[j+1][0];
  				float temp2=bmResult[j+1][1];
  				bmResult[j+1][0]=bmResult[j][0];
  				bmResult[j+1][1]=bmResult[j][1];
  				bmResult[j][0]=temp;
				bmResult[j][1]=temp2;
			  }
  		}
	}
  	/////////////////////////////
  	for(int i=1;i<=100;i++)
  	{
  		printf("query_id %d Q0 doc_id %d rank %d BM25_score %f system_name\n",1,(int)bmResult[i][1],i,bmResult[i][0]);
  	}
  
 fclose(file);
 return 0;
}
