**RC4 Cipher in Assembly**
========================
Details of this project can be found on the [RC4 Cipher project page][2] under:  
[*http://www.CharmySoft.com/app/rc4-cipher.htm*][2]

Introduction
------------------------
[RC4 Cipher in Assembly][2] is the RC4 stream cipher implemented in Assembly language for faster speed.  


Instruction
------------------------
This project uses inline assemly in C to optimize the RC4 sstream cipher, so the assembly code is in Intel style syntax that is supported by Microsoft Assembler (MASM). Thus, you should be using Microsoft Visual C++ to compile the code.  
The [rc4_cipher.c](rc4_cipher.c) file contains the x86 assemly version of the RC cipher.  
The [rc4_test.c](rc4_test.c) file contains the C version of the RC cipher and a benchmark test example to demonstrate the speed improvement.  
The [rc4_cipher.e](rc4_cipher.e) file is for E language (or EPL) users.  


Screenshots
------------------------
![Benchmark](/screenshots/benchmark.png?raw=true "Benchmark")


Licensing
------------------------
Please see the file named [LICENSE](LICENSE).


Author
------------------------
* Charlie Chen  
	founder of [CharmySoft][1]


Contact
------------------------
* CharmySoft: [*http://www.CharmySoft.com/*][1]  
* About: [*http://www.CharmySoft.com/about.htm*][3]  

[1]: http://www.CharmySoft.com/ "CharmySoft"
[2]: http://www.CharmySoft.com/app/rc4-cipher.htm "RC4 Cipher in Assembly"
[3]: http://www.CharmySoft.com/about.htm "About CharmySoft"