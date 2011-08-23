/*
**  RC4 Cipher in Assembly Test
**
**  Author:		Charlie Chen
**  Website:	http://www.CharmySoft.com/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rc4_x86(const void *inbuf, void *outbuf, size_t buflen, const char *msg, size_t msglen);
int rc4_c(const void *inbuf, void *outbuf, size_t buflen, const char *msg, size_t msglen);

int main(int argc, char* argv[])
{

	char password[] = "password";
	char data[] = "This is the data that needs to be encrypted";
	void *encryptedData = malloc(sizeof(data));
	void *decryptedData = malloc(sizeof(data));
	printf("Raw data:\n%s \n\n", data);
	rc4_c(data, encryptedData, sizeof(data), password, sizeof(password));
	printf("Encrypted data:\n%s \n\n", encryptedData);
	rc4_x86(encryptedData, decryptedData, sizeof(data), password, sizeof(password));
	printf("Decrypted data:\n%s \n\n", decryptedData);
	if(strcmp(data, (char*)decryptedData) == 0){
		printf("Raw data matches decrypted data! Success!\n");
	}else{
		printf("Raw data does not match decrypted data! Data damaged!\n");
	}
	getchar();
    return 0;
}


int rc4_c(const void *inbuf, void *outbuf, size_t buflen, const char *msg, size_t msglen)
{
	char s[256];
	char k[256];
	int i;
	unsigned char j;
	unsigned char temp;
	
	if(buflen <= 0)
		return -1;

	// Initialize RC4 state

	
	for(i = 0; i < sizeof(s); i++){
		// Fill s with 0..255  
		s[i] = i;
		// Generate k
		k[i] = msg[i % msglen];
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