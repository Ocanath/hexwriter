#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

/*
Generic hex checksum calculation.
TODO: use this in the psyonic API
*/
uint8_t get_checksum(uint8_t * arr, int size)
{

	int8_t checksum = 0;
	for (int i = 0; i < size; i++)
		checksum += (int8_t)arr[i];
	return -checksum;
}


#define NUM_PAGES_TO_CORRUPT 12
#define FS_ADDR_LEN (NUM_PAGES_TO_CORRUPT * 4 * 1024)	//number of 4 byte words to write in our hex file
#define FS_ADDR_START 0x4000
void main()
{
	FILE * hexfile;
	hexfile = fopen("testhex.hex", "w");
	//hexfile = fopen("C:\Keil_v5\Project\ble_app_uart\testhex.hex", "w");
	if (hexfile != NULL)
	{
		fprintf(hexfile, ":020000040006F4\n");
		//printf(":02000004006F4\n");
		uint8_t bytecount = 0x10;
		uint8_t record_type = 0x00;
		uint8_t data_word = 0xDE;	//optional; make this an array of size bytecount and fill it with uniquie wordsicles. 
		for (int addr = FS_ADDR_START; addr < FS_ADDR_LEN + FS_ADDR_START; addr += bytecount)
		{
			const int bytecount_line = 21;
			uint8_t data_line_entry[bytecount_line];
			data_line_entry[0] = bytecount;
			data_line_entry[1] = (addr & 0xFF00) >> 8;
			data_line_entry[2] = (addr & 0x00FF);
			data_line_entry[3] = record_type;
			for (int i = 4; i < bytecount_line - 1; i++)
				data_line_entry[i] = data_word;


			//int8_t checksum = 0;
			//for (int i = 0; i < bytecount_line - 1; i++)
			//	checksum += (int8_t)data_line_entry[i];
			//data_line_entry[bytecount_line - 1] = -checksum;
			data_line_entry[bytecount_line - 1] = get_checksum(data_line_entry, bytecount_line - 1);

			fprintf(hexfile, ":");
			//printf(":");
			for (int i = 0; i < bytecount_line; i++)
			{
				fprintf(hexfile, "%02X", data_line_entry[i]);
				//printf("%02X", data_line_entry[i]);
			}
			fprintf(hexfile, "\n");
			//printf("\n");
		}
		fprintf(hexfile, ":00000001FF\n");
		//printf(":00000001FF\n");

		fclose(hexfile);
	}
	else
	{
		printf("SHIT\n");
		while (1);
	}
	//while (1);
}