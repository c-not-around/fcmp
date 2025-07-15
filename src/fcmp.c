#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <locale.h>


#define CHUNK_SIZE			(4*1024UL)
#define DIFFS_MAX			10
#define DIFF_MAX_LEN		60

#define READ_CHUNK(f,c)		fread((void*)(c), 1, CHUNK_SIZE, f)


typedef uint64_t chunk_t;


size_t fsize(FILE *file)
{
	_fseeki64(file, 0, SEEK_END);
	size_t size = _ftelli64(file);
	_fseeki64(file, 0, SEEK_SET);
	return size;
}

char *prefix(float size)
{
	int p = 0;
	while (size >= 1024.0f)
	{
		size /= 1024.0f;
		p++;
	}
	
	static char text[10];
	char *ptxt = text + sprintf(text, "%.2f", size);
	if (p)
	{
		*ptxt++ = " kMGTP"[p];
	}
	*ptxt++ = 'b';
	*ptxt++ = '\0';
	
	return text;
}

int print_be(char *buf, uint8_t *x, int len, int width)
{
	for (int i = 0; i < (width-len); i++)
	{
		buf += sprintf(buf, "  ");
	}
	
	for (int i = 0; i < len; i++)
	{
		buf += sprintf(buf, "%02X", x[i]);
	}
	
	return (width * 2);
}

void print_file_stat(char *fname, size_t size)
{
	printf("\t\"%s\" %llibytes", fname, size);
	
	if (size > 1023)
	{
		printf("(%s)", prefix((float)size));
	}
	
	printf("\r\n");
}

int main(int argc, char **args)
{
	setlocale(LC_ALL, "");
	
	setvbuf(stdout, NULL, _IONBF, 0);
	
	if (argc > 2)
	{
		char *f1name = args[1];
		char *f2name = args[2];
		
		FILE *f1 = NULL;
		FILE *f2 = NULL;
		
		if ((f1 = fopen(f1name, "rb")) && (f2 = fopen(f2name, "rb")))
		{
			size_t size1 = fsize(f1);
			size_t size2 = fsize(f2);
			size_t size  = min(size1, size2);
			
			printf("compare files:\r\n");
			print_file_stat(f1name, size1);
			print_file_stat(f2name, size2);
			
			clock_t t0 = clock();
			
			chunk_t chunk1[CHUNK_SIZE/sizeof(chunk_t)];
			chunk_t chunk2[CHUNK_SIZE/sizeof(chunk_t)];
			memset(chunk1, 0x00, CHUNK_SIZE);
			memset(chunk2, 0x00, CHUNK_SIZE);
			
			size_t   n1 = 0;
			size_t   n2 = 0;
			uint64_t p  = 0;
			int      d  = 0;
			char     diffs[DIFFS_MAX][DIFF_MAX_LEN];
			int      no = 0;
			
			while ((n1 = READ_CHUNK(f1, chunk1)) && (n2 = READ_CHUNK(f2, chunk2)))
			{
				int m = min(n1, n2);
				int n = (m + (sizeof(chunk_t)-1)) / sizeof(chunk_t);
				
				for (int i = 0; i < n; i++)
				{
					if (chunk1[i] != chunk2[i])
					{
						int pos = i * sizeof(chunk_t);
						int len = min(sizeof(chunk_t), m - pos);
						
						char *pdiff = diffs[d];
						pdiff += sprintf(pdiff, "[%016llX]: ", p+pos);
						pdiff += print_be(pdiff, (uint8_t*)(&chunk1[i]), len, sizeof(chunk_t));
						pdiff += sprintf(pdiff, " != ");
						pdiff += print_be(pdiff, (uint8_t*)(&chunk2[i]), len, len);
						
						if (++d >= DIFFS_MAX)
						{
							break;
						}
					}
				}
				
				p += m;
				
				n = (50 * p) / size - no;
				if (n)
				{
					no += n;
					
					while (n--)
					{
						printf("#");
					}
				}
				
				if (d >= DIFFS_MAX)
				{
					break;
				}
			}
			
			no = 50 - no;
			while (no--)
			{
				printf("#");
			}
			
			float dt = (float)(clock() - t0) / CLOCKS_PER_SEC + 1e-6;
			printf("\r\n%llubytes %.3fs (%s/s).\r\n", p, dt, prefix((float)p/dt));
			
			if (d == 0)
			{
				printf("matched!\r\n");
			}
			else
			{
				printf("different(first %i):\r\n", DIFFS_MAX);
				
				for (int i = 0; i < d; i++)
				{
					printf("%s\r\n", diffs[i]);
				}
			}
		}
		else
		{
			printf("files \"%s\" or \"%s\" open error.\r\n", f1name, f2name);
		}
		
		if (f1 != NULL)
		{
			fclose(f1);
		}
		if (f2 != NULL)
		{
			fclose(f2);
		}
	}
	else
	{
		printf("incorrect parameters.\r\nUsage: fcmp.exe <file1> <file2>\r\n");
	}
	
	return 0;
}