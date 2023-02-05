#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	unsigned char data[16];
	int address;
	int count;
} hexa_t;

hexa_t memory[1000];
int cc=0;

void reset() {
	for(int i=0;i<1000;i++) {
		for(int j;j<16;j++)
			memory[i].data[j]=0;
		memory[i].count=0;
		memory[i].address=0;
	}
	cc=0;
}

void write_byte_at(int byte, unsigned int offset,unsigned int diff) {
	memory[offset].data[diff] = (unsigned char)byte;
	memory[offset].count++;
}
void write_text(int size, int startadd){
	int counter=0;
	for(int i=0;i<1000;i++) {
		if(counter<=size) {
			printf("%x | ",startadd+counter);
			for(int j=0;j<16;j++) {
				if (counter<=size) {
					printf("%02x ",memory[i].data[j]);
					counter++;
				} else {
					break;
				}
			}
			printf("\n");
		} else {
			break;
		}
	}
}

void main() {
	char name[121],*record,*tchar,pair[3],*end;
	int startadd,size,addr,temp,count,byte,codesize;
	size_t len;
	reset();
	FILE* input = fopen("objcode.txt","r");
	fscanf(input,"H^%6[0-9a-zA-Z ]^%x^%x^\n",name,&startadd,&codesize);
	printf("Name :- %s Starting Address :- %x Size :- %x\n",name,startadd,codesize);
	getline(&record,&len,input);
	pair[3]='\0';
	while(record[0]=='T') {
		tchar=strtok(record,"^");
		count=0;
		byte=0;
		while((tchar=strtok(NULL,"^"))!=NULL) {
			if (count==0) {
				addr=strtol(tchar,&end,16);
			} else if (count==1) {
				size=strtol(tchar,&end,16);
			} else if (strlen(tchar)>1){
				for(int i=0;i<strlen(tchar);i+=2) {
					pair[0]=tchar[i];
					pair[1]=tchar[i+1];
					temp=strtol(pair,&end,16);
					write_byte_at(temp,abs(addr-startadd+byte)/16,abs(addr-startadd+byte)%16);
					byte++;
				}
			}
			count++;
		}
		getline(&record,&len,input);
	}
	sscanf(record,"E^%x^",&addr);
	write_text(codesize,startadd);
	printf("Execution Address :- %x\n",addr);
}
