/*
**  RC4 Cipher in Assembly
**
**  Author:		Charlie Chen
**  Website:	http://www.CharmySoft.com/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rc4_x86(const void *inbuf, void *outbuf, size_t buflen, const char *msg, size_t msglen)
{
	char s[256];
	char *s_ptr = s;
	char k[256];
	char *k_ptr = k;

	if(buflen <= 0)
		return 0;

	// Initialize RC4 state

	// Fill s with 0..255  
	__asm {
		mov eax, s_ptr
		mov ecx, 256
		xor ebx, ebx
		fill_s:
			mov bl, 256
			sub bl, cl
			mov [eax + ebx], bl
		loop fill_s
	}

	// Generate k
	__asm {
		mov edx, msg
		mov edi, msglen ;//edi = size of msg
		mov esi, k_ptr ;//esi= k;
		mov ecx, 256
		xor ebx, ebx ;//ebx=j
loop_j:
		cmp ebx, edi
			jl continue_loop
			xor ebx, ebx ;// clear ebx, move to the start of msg, repeat until k is full
continue_loop:
		mov ah, [edx + ebx]
		mov [esi], ah
		inc esi
		inc ebx
		loop loop_j
	}
	
	// Generate s
	__asm {
		mov edi, s_ptr
		xor ebx, ebx
		sub esi, 256
		xor eax, eax
		mov ecx, 256
loop_s:
		mov dl, [esi+eax]
		add bl, dl
		mov dl, [edi+eax]
		add bl, dl
		mov dl, [edi+eax]
		mov dh, [edi+ebx]
		mov [edi+eax], dh
		mov [edi+ebx], dl
		inc eax
		loop loop_s
	}
	
	// allocate memory to ret
	//outbuf = malloc(buflen);

	// Encrypt / decrypt
	/*
	esi = inbuf
	edx = outbuf
	ecx = buflen
	edi = s
	eax = z
	ebx = z2
	ebx = t
	dl = Y
	*/
	__asm{
			mov esi, inbuf ;// esi = inbuf
			mov edi, s_ptr ;// edi = s
			mov edx, outbuf ;// edx = outbuf
		
			;// clear registers
			xor eax,eax
			xor ebx,ebx
			
			mov ecx, buflen ;//ecx = buflen
cd:
			push ecx
			movzx ecx,al
			inc cl
			push edx
			mov dh,[edi+ecx]
			add bl,dh
			mov dl,[edi+ebx]
			mov [edi+ecx],dl
			mov [edi+ebx],dh
			add dl,dh
			movzx edx,dl
			mov dl,[edi+edx]
			mov cl,[esi+eax]
			xor cl,dl
			pop edx
			mov [edx+eax],cl
			inc eax
			pop ecx
			loop cd
	}

	return buflen;
}

int main(int argc, char* argv[])
{

	char password[] = "this is the password";
	char data[] = "This is the data to be encrypted";
	void *encryptedData = malloc(sizeof(data));
	void *decryptedData = malloc(sizeof(data));
	printf("Raw data:\n%s \n\n", data);
	rc4_x86(data, encryptedData, sizeof(data), password, sizeof(password));
	printf("Encrypted data:\n%s \n\n", encryptedData);
	rc4_x86(encryptedData, decryptedData, sizeof(data), password, sizeof(password));
	printf("Decrypted data:\n%s \n\n", decryptedData);
	if(strcmp(data, (char*)decryptedData) == 0)
		printf("Raw data matches decrypted data! Success!\n", decryptedData);
	getchar();
    return 0;
}


