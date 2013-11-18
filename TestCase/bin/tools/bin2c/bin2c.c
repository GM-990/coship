 
 
 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/vfs.h>

#define MAX_FILE_NAME 512

#define BYTES_PER_LINE 16

#define MIN_BLOCK_SIZE (512*1024)
int main(int argc,char *argv[])
{
char binfilename[MAX_FILE_NAME];
char cfilename[MAX_FILE_NAME];
unsigned char buffer[MIN_BLOCK_SIZE];
FILE* fp_bin = NULL;
FILE* fp_c = NULL;
int numread = 0;
int i,j;
int nFileIndex;
int fileLength = 0;
int MMCP_ROMSizePerUnit = MIN_BLOCK_SIZE;
int MMCP_ROMUnits = 0;
 struct stat sFileState;
char temp[256] = "";
char temp2[256] = "";
unsigned char *ptr;

printf("bin2c 3.0 - Copyright (c) www.coship.com \n");

if (argc < 3)
{
printf("usage: bin2c <binary_filename> <c_filename>\n");
return -1;
}

/*for(nFileIndex=0;nFileIndex<argc;nFileIndex++)
	printf("argv[%d]=\"%s\"\n",nFileIndex,argv[nFileIndex]);
*/
memset(binfilename,0,MAX_FILE_NAME);
strcpy(binfilename, &argv[1][2]);
strcat(binfilename, "/");
strcat(binfilename, &argv[2][0]);
printf("bin2c 3.0 - binfilename=\"%s\"\n",binfilename);
if(stat(binfilename, &sFileState) == 0)
{
fileLength = sFileState.st_size;
MMCP_ROMUnits = fileLength/MIN_BLOCK_SIZE;
if(fileLength%MIN_BLOCK_SIZE)\
	MMCP_ROMUnits += 1;
printf ("file \"%s\" length %d bytes,ROMUnits=%d\n",binfilename,fileLength,MMCP_ROMUnits);
}
else
{
printf("error stating input file\n");
return -1;
}

if ((fp_bin= fopen(binfilename,"rb")) ==NULL)
{
	printf("error opening input file\n");
	return -1;
}
memset(cfilename,0,MAX_FILE_NAME);
strcpy(cfilename, &argv[1][2]);
strcat(cfilename,"/");
strcat(cfilename,"romfscfg.c");


if ((fp_c = fopen(cfilename,"wt")) !=NULL)
{


printf("genreating %s\n",cfilename);
fprintf(fp_c, "const int MMCP_ROMSizePerUnit = ");
fprintf(fp_c, "%d ;",MMCP_ROMSizePerUnit);
fprintf(fp_c, " \n\n");
fprintf(fp_c, "const int MMCP_ROMUnits = ");
fprintf(fp_c, "%d ;",MMCP_ROMUnits);
fprintf(fp_c, " \n\n");
for(nFileIndex=0;nFileIndex<MMCP_ROMUnits;nFileIndex++)
{
	fprintf(fp_c, "extern const char %s%03d[];\n",&argv[3][0],nFileIndex);
}
fprintf(fp_c, " \n\n");
fprintf(fp_c, "const char * MMCP_ROMArray[]={\n");
for(nFileIndex=0;nFileIndex<MMCP_ROMUnits-1;nFileIndex++)
	fprintf(fp_c, "		%s%03d,\n",&argv[3][0],nFileIndex);
fprintf(fp_c, "		%s%03d\n};",&argv[3][0],nFileIndex);
fprintf(fp_c, " \n\n");



fclose(fp_c);
for(nFileIndex=0;nFileIndex<MMCP_ROMUnits;nFileIndex++)
{
	memset(cfilename,0,MAX_FILE_NAME);
	sprintf(cfilename,"%s/%s%03d.c",&argv[1][2],&argv[3][0],nFileIndex);
	if ((fp_c = fopen(cfilename,"wt")) !=NULL)
	{
		printf("genreating %s\n",cfilename);
		fprintf(fp_c, " \n\n");
		numread = fread(buffer,sizeof(unsigned char),MIN_BLOCK_SIZE,fp_bin);
		fprintf(fp_c, "const char %s%03d[]={\n",&argv[3][0],nFileIndex);
		if(numread >0)
		{
			ptr = buffer;
			if(numread < MIN_BLOCK_SIZE)
			{
				memset(&buffer[numread],0,MIN_BLOCK_SIZE-numread);
			}
			for (i = 0; i < ((numread + BYTES_PER_LINE - 1) / BYTES_PER_LINE); i++)
			{

				for (j = 0; j < BYTES_PER_LINE; j++)
				{
					if (((i * BYTES_PER_LINE) + j) < (numread-1))
					{
						sprintf(temp2,"0x%02X, ",*ptr++);
						strcat(temp,temp2);
					}
					else if (((i * BYTES_PER_LINE) + j) < (numread))
					{
						sprintf(temp2,"0x%02X ",*ptr++);
						strcat(temp,temp2);
					}
				}
				fprintf(fp_c, "%s\n",temp);
				sprintf(temp,"");
			}
			fprintf(fp_c, "};\n");
		}
		fprintf(fp_c, " \n\n");
	}
	fclose(fp_c);

}
fclose(fp_bin);
}
else
{
printf("error opening output file\n");
return -1;
}
return 0;
}
