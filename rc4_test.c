/*
**  RC4 Cipher in Assembly Test
**
**  Author:		Charlie Chen
**  Website:	http://www.CharmySoft.com/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int rc4_x86(const void *inbuf, void *outbuf, size_t buflen, const char *key, size_t keylen);
int rc4_c(const void *inbuf, void *outbuf, size_t buflen, const char *key, size_t keylen);

char* string_repeat( int n, const char * s );

int main(int argc, char* argv[])
{
	char password[] = "password";
	char data[] = "This is the data that needs to be encrypted";
	size_t datalen = sizeof(data);
	char *data_long;
	void *encryptedData = malloc(datalen);
	void *decryptedData = malloc(datalen);

	// Benchmark speed
	const int TRIALS = 10000;
	int i;
	time_t start;
	time_t t_c;
	time_t t_x86;

	// Short data benchmark test
	printf("For short data (size %d)\n", datalen);

	start = clock();
	for (i = 0; i < TRIALS; i++)
		rc4_c(data, encryptedData, datalen, password, sizeof(password));
	t_c = clock() - start;
	printf("C code time consumed: %d ms.\n", t_c);

	start = clock();
	for (i = 0; i < TRIALS; i++)
		rc4_x86(encryptedData, decryptedData, datalen, password, sizeof(password));
	t_x86 = clock() - start;
	printf("Assembly code time consumed: %d ms.\n", t_x86);

	printf("The Assembly version is %.2fx faster than the c version.\n\n", (double)t_c / t_x86);

	if(memcmp(data, (char*)decryptedData, datalen) != 0)
		printf("Raw data does not match decrypted data! Data damaged!\n\n");

	
	// Long data benchmark test
	data_long = string_repeat(1000, "This is the data that needs to be encrypted");
	datalen *= 1000;
	encryptedData = malloc(datalen);
	decryptedData = malloc(datalen);
	
	printf("For long data (size %d):\n", datalen);

	start = clock();
	for (i = 0; i < TRIALS; i++)
		rc4_c(data_long, encryptedData, datalen, password, sizeof(password));
	t_c = clock() - start;
	printf("C code time consumed: %d ms.\n", t_c);

	start = clock();
	for (i = 0; i < TRIALS; i++)
		rc4_x86(encryptedData, decryptedData, datalen, password, sizeof(password));
	t_x86 = clock() - start;
	printf("Assembly code time consumed: %d ms.\n", t_x86);

	printf("The Assembly version is %.2fx faster than the c version.\n\n", (double)t_c / t_x86);

	if(memcmp(data_long, (char*)decryptedData, datalen) != 0)
		printf("Raw data does not match decrypted data! Data damaged!\n\n");
	free(data_long);

	getchar();
    return 0;
}


int rc4_c(const void *inbuf, void *outbuf, size_t buflen, const char *key, size_t keylen)
{
	char s[256];
	char k[256];
	unsigned int i;
	unsigned char j;
	unsigned char temp;
	
	if(buflen <= 0)
		return -1;

	// Initialize RC4 state

	for(i = 0; i < sizeof(s); i++){
		// Fill s with 0..255  
		s[i] = i;
		// Generate k
		k[i] = key[i % keylen];
	}
	
	j = 0;
	for(i = 0; i < sizeof(s); i++){
		j = j + s[i] + k[i];
		temp = s[i]; // swap
		s[i] = s[j];
		s[j] = temp;
	}
	
	j = 0;
	for(i = 0; i < buflen; i++){
		unsigned char c;
		unsigned char t;
		c = i + 1;
		j += s[c];
		temp = s[c]; // swap
		s[c] = s[j];
		s[j] = temp;
		t = s[c] + s[j];
		*((char*)outbuf + i) = *((char*)inbuf + i) ^ s[t];
	}
	

	return buflen;

}

char * string_repeat( int n, const char * s ) {
  size_t slen = strlen(s);
  char * dest = (char *)malloc(n*slen+1);
 
  int i; char * p;
  for ( i=0, p = dest; i < n; ++i, p += slen ) {
    memcpy(p, s, slen);
  }
  *p = '\0';
  return dest;
}