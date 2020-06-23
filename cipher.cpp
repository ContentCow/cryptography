#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;
//reads through a text to find the number of outputs
int** getOccurances(char* fileName){
    int ** occurance=0;
    occurance= new int*[27];
    for(int i=0; i<27;i++){
        occurance[i]=new int[27];
        for(int j=0;j<27;j++){
            occurance[i][j]=1;
        }
    }
    fstream fs;
    fs.open(fileName,fstream::in);
    char current, prev;
    if(fs.good()){
        fs.get(prev);
    }
    while(fs.get(current)){
        if(current>='a'&&current<='z'){
            if(prev>='a'&&prev<='z'){
                occurance[prev-'a'][current-'a']++;
            }else{
                occurance[26][current-'a']++;
            }
        }else{
            if(prev>='a'&&prev<='z'){
                occurance[prev-'a'][26];
            }
        }
        prev=current;
    }
    fs.close();
    return( occurance);
}

int randint(int minimum, int maximum){
    unsigned long bins = maximum-minimum+1,
                rands=(unsigned long)RAND_MAX+1,
                bin_size=rands/bins,
                overflow=rands%bins;
    int r;
    do{
        r=rand();
    }while(rands-overflow<=(unsigned long)r);
    return(r/bin_size+minimum);
}

//shuffles an existing key
char* shuffle(char* key){
    char temp;
    int randNum;
    for(int i=0;i<26;i++){
            randNum=randint(0,25);
            temp=key[randNum];
            key[randNum]=key[i];
            key[i]=temp;
    }
}

char* randSwap(char* key){
    char temp;
    int randIndex1=randint(0,25);
    int randIndex2=randint(0,25);
    temp=key[randIndex1];
    key[randIndex1]=key[randIndex2];
    key[randIndex2]=temp;
}

//makes a new key
char* keyGenerator(){
    char *key=new char[26];
    for(int i=0;i<26;i++){
        key[i]='a'+i;
    }
    shuffle(key);
    return key;
}

//applies a key to file
void apply(char* key, char* inFile, char*outFile){
    fstream fin;
    fstream fout;
    fin.open(inFile,fstream::in);
    fout.open(outFile,fstream::out);
    char current;
    while(fin.get(current)){
        if(current>='a'&&current<='z'){
            fout << key[current-'a'];
        }
        else{
            fout<< current;
        }
    }
    fin.close();
    fout.close();
}

//adds up all probabilities based on the table and the input file
float sumProbabilities(float** table, char* fileName){
    float sum=0;
    fstream fin;
    fin.open(fileName,fstream::in);
    char current,prev;
    if(fin.good()){
        fin.get(prev);
    }
    while(fin.get(current)){
        if(current>='a'&&current<='z'){
            if(prev>='a'&&prev<='z'){
                sum+=table[prev-'a'][current-'a'];
            }else{
                sum+=table[26][current-'a'];
            }
        }else{
            if(prev>='a'&&prev<='z'){
                sum+=table[prev-'a'][26];
            }
        }
        prev=current;
    }
    fin.close();
    return sum;
}

//checks if the new sum is better than random
int bernouli(double p){
    double r = fmod(((double)rand()),((double)RAND_MAX));
    return(r<=abs(p));
}
/*
char** decipher(int** probabilities, char* fileName){
    fstream fs;
    fs.open(fileName,fstream::in);
    char** mapping=0;
    mapping = new char*[26];
    for(int i=0;i<26;i++){
        mapping[i]=new char[2];
        mapping[i][0]='a'+i;
        mapping[i][1]='a'+i;
    }
    char current,prev;
    if(fs.good()){
        fs.get(prev);
    }
    char mappedCurrent,mappedPrev;
    while(fs.get(current)){
        if(current>='a'&&current<='z'){
            if(prev>='a'&&prev<='z'){
                mappedPrev=mapping[prev-'a'][1];
                mappedCurrent=mapping[current-'a'][1];
                for(int i=0; i<26;i++){
                    if(probabilities[mappedPrev-'a'][mappedCurrent-'a']<probabilities[mappedPrev-'a'][i]){
                        char temp=mapping[current-'a'][1];
                        mapping[current-'a'][1]=mapping[i][1];
                        mapping[i][1]=temp;
                    }
                }
            }
        }
        prev=current;
    }
    fs.close();
    return (mapping);
}
*/

int main(int argc, char* argv[]){
    int** occurance=getOccurances(argv[1]);
    int numberOfOccurance=0;
    //sums the number of pairs
    for(int i=0;i<27;i++){
        for(int j=0;j<27;j++){
            //char a='a'+i;
            //char b='a'+j;
            //cout<<"["<< a << "]["<< b<< "]="<< occurance[i][j];
            numberOfOccurance+=occurance[i][j];
        }
    }
    //gets the log value of the occurances
    float** floatOccurance=new float*[27];
    for(int i=0;i<27;i++){
        floatOccurance[i]=new float[27];
        for(int j=0;j<27;j++){
            floatOccurance[i][j]=log(occurance[i][j])-log(numberOfOccurance);
        }
    }

    char* key1=keyGenerator();
    char* key2=keyGenerator();
    apply(key1,argv[2],argv[3]);
    float sum1=sumProbabilities(floatOccurance,argv[3]);
    float sum2;
    for(int i=0;i<500000;i++){
        shuffle(key2);
        apply(key2,argv[2],argv[4]);
        sum2=sumProbabilities(floatOccurance,argv[4]);
        if(sum1<sum2){
            if(bernouli(sum2)){
                apply(key2,argv[2],argv[3]);
                key1=key2;
                sum1=sum2;
                cout <<sum1<<endl;
            }
        }
    }
    cout<< "DONE :)";
    cin.get();
    return(0);
}
