#include <stdio.h>
#include <string.h>
#include <time.h>

#define filename "generator_out.h"

int main(void){
FILE * file = fopen(filename, "w+");
if(!file){
	printf("Error!");
	return 0;
}
	printf("\
Input up to 8 ones or zeroes, then press enter.\n\
To delete previous line enter \"d\".\n\
To finish just enter \"f\"\n\
|====1====|\n\
>>--------|\n\
1>");
	char credits[] = {"//File generated by shark46 header generator\n\n"};
	fwrite(&credits[0], sizeof(char), strlen(&credits[0]), file);
	char header_start[] =  {"const unsigned char animation[][8]={\n"};
	fwrite(&header_start[0], sizeof(char), strlen(&header_start[0]), file);
	char separator[] = {"\n},{\n\t"};

	char input;
	unsigned char inputbuf = 0;
	char byte_count = 0;
	char flag_bc = 0;
	char buf[4];
	char bit_count = 0;
	unsigned int frame_count = 1;
	char buf2[256];
	char enough = 0;
	long tmp = 0;
	char filebuf[500000];
	fpos_t pos;

	while (!enough){		
		input = getchar();
		bit_count++;
		if(input=='\n'){	// If we reach end of input string
			bit_count = 0;
			sprintf(&buf[0], "%d", inputbuf);
			inputbuf = 0;
			if (++byte_count <= 8){	
				flag_bc = 1;
				if(byte_count == 1){	// First byte in frame
					fwrite("{ ", sizeof(char), 2, file);	// Open brackets
					fwrite(&buf[0], sizeof(char), strlen(&buf[0]), file); // Write byte
					fwrite(",", sizeof(char), 1, file); // comma between digits
					flag_bc = 0;
				}else{
					fwrite(&buf[0], sizeof(char), strlen(&buf[0]), file);
					fwrite(",", sizeof(char), 1, file); // comma between digits
					flag_bc = 0;
				}
			}
			if(byte_count>=8) {			// If we just wrote 8th byte, then
				fseek(file, -1, SEEK_CUR); // Delete the comma
				sprintf(&buf2[0], " },\t\t\t// %d\n", frame_count);
				fwrite(&buf2[0], sizeof(char), strlen(&buf2[0]), file); //close the brackets
				byte_count = 0;
				flag_bc = 0;
				if(frame_count++ < 9){
					printf("|====%d====|\n", frame_count);
				}else{
					printf("|===%d====|\n", frame_count);
				}
				
			}
			printf(">>--------|\n%d>", byte_count+1);
		}else{						//If something exept enter is coming
			if(input=='0'||input=='1'){
				if(bit_count > 8){ //If too many digits is coming...//
					printf("8 digits maximum!\a\n%d>--------|\n>>", 1+byte_count);
					bit_count = 0;									//
					do{												//
						input = getchar();							//
					}while(input != '\n');							//
					input=0;										//
					inputbuf=0;										//
					buf[0]=0;										//
					if(flag_bc)	byte_count--;						//
					flag_bc = 0;									//
				}else{//============================================//
					inputbuf<<=1; //Everything is OK. Storing the data
					inputbuf|=(input-'0');
				}
			}else{//=======If input is not 0 or 1===================//
				switch (input)
				{
				case 'f':
					enough++;
					break;
				case 'd':		//Remove previous byte
						if(byte_count){ // If it is not first byte
							fseek(file, -1, SEEK_CUR);
							do{
								fseek(file, -1, SEEK_CUR);
								fread(&tmp, sizeof(char), 1, file);
								fseek(file, -1, SEEK_CUR);
							}while(!((tmp == ',') || (tmp == ' ')));
							fseek(file, 1, SEEK_CUR);
							byte_count--;				
							bit_count = 0;	
							if(!byte_count){
								do{
									fseek(file, -1, SEEK_CUR);
									fread(&tmp, sizeof(char), 1, file);
									fseek(file, -1, SEEK_CUR);
								}while(tmp != '\n');
								fseek(file, 1, SEEK_CUR);	
								fgetpos(file, &pos);
								fseek(file, 0, SEEK_SET);
								tmp = pos.__pos;
								fread(filebuf, sizeof(char), tmp, file);
								fclose(file);
								file = fopen(filename, "w+");
								fwrite(&filebuf[0], sizeof(char), tmp, file);
							}							
						}else{		//If it is first byte
							if(frame_count-1){	//And it is not first frame
								do{
									fseek(file, -1, SEEK_CUR);
									fread(&tmp, sizeof(char), 1, file);
									fseek(file, -1, SEEK_CUR);
								}while(tmp != '}');
								do{
									fseek(file, -1, SEEK_CUR);
									fread(&tmp, sizeof(char), 1, file);
									fseek(file, -1, SEEK_CUR);
								}while(tmp != ',');
								fseek(file, 1, SEEK_CUR);
								byte_count = 7;
								bit_count = 0;
								frame_count--;
								if((frame_count + 1) < 9){
									printf("|====%d====|\n", frame_count);
								}else{
									printf("|===%d====|\n", frame_count);
								}
							}else{	//first byte first frame
								printf("Nothing to delete...\n");//
							}
						}
						do{
							tmp = getchar();
						}while(tmp != '\n');
						printf(">>--------|\n%d>", byte_count + 1);
					break;
				
				default:
					printf("Ony 1 or 0 are allowed!\a\n%d>--------|\n>>", 1+byte_count);
					do{													//
						input = getchar();								//
					}while(input != '\n');								//
					input=0;											//
					inputbuf=0;											//
					buf[0]=0;											//
					if(flag_bc) byte_count--;							//
					flag_bc = 0;										//
					bit_count = 0;										//
						break;
				}				
			}//=====================================================//
		}
	}
	if(frame_count - 1){
		for(;;){	//============find "}," and delete ","==========//
			fseek(file, -2, SEEK_CUR);								//
			fread(&buf[0], sizeof(char), 2, file);					//
			if((buf[0] == '}') && (buf[1] == ',')){					//
				fseek(file, -1, SEEK_CUR);							//
				buf[0] = ' ';										//
				fwrite(&buf[0], sizeof(char), 1, file);				//
				fseek(file, 0, SEEK_END);							//
				break;												//
			}														//
			fseek(file, -1, SEEK_CUR);								//
		}//=========================================================//
	}
	if(byte_count){	//If it is not first byte in frame
		for(;;){	//============find "\n{" and delete ============//
			fseek(file, -2, SEEK_CUR);								//
			fread(&buf[0], sizeof(char), 2, file);					//
			if((buf[0] == '\n') && (buf[1] == '{')){				//
				fseek(file, -2, SEEK_CUR);							//
				break;												//
			}														//
			fseek(file, -1, SEEK_CUR);								//
		}//=========================================================//
	}else{
		fseek(file, -1, SEEK_CUR);
	}
	
	fwrite("\n};", sizeof(char), 3, file);
	sprintf(&buf2[0], "\n\nconst unsigned int frame_count = %d;", --frame_count);
	fwrite(&buf2[0], sizeof(char), strlen(&buf2[0]), file);
	fclose(file);

	printf("Done.\n");
	return 0;
}
