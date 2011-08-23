/*
**  RC4 Cipher in Assembly
**
**  Author:		Charlie Chen
**  Website:	http://www.CharmySoft.com/
*/

#include <stdio.h>

/**
 * Encrypt or decrypt data using RC4 stream cipher.
 * The input and output buffers may be the same buffer.
 * Since RC4 is a stream cypher, this function is used
 * for both encryption and decryption.
 *
 * @param inbuf This is the source data that needs to be encrypted.
 * @param outbuf This is the encrypted result data.
 * @param buflen The length of the source data and result data.
 * @param msg This is the message(password) used for encryption.
 * @param msglen The length of the message string.
 * @return If success, return the length of the result data buffer. Otherwise, return -1.
 */
int rc4_x86(const void *inbuf, void *outbuf, size_t buflen, const char *msg, size_t msglen)
{
	char s[256];
	char *s_ptr = s;
	char k[256];
	char *k_ptr = k;

	if(buflen <= 0)
		return -1;

	// Initialize RC4 state

	// Fill s with 0..255  
	__asm {
		mov eax, s_ptr
		mov ecx, 256
		fill_s:
			xor ebx, ebx
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


