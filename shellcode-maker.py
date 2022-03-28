#!/usr/bin/python3
import sys
import os
import tempfile

def main():
	'''
	Extract .text segment from asm file and display it as a C shellcode
	'''
	if (len(sys.argv) < 2 or len(sys.argv[1]) < 2):
		print('Usage: shellcode.py <file.s>')
		return 1

	tmpdir = tempfile.mkdtemp()
	os.system("cp " + sys.argv[1] + " " + tmpdir)
	c_workplace = os.getcwd()
	os.chdir(tmpdir)

	os.system("\
	nasm -f elf64 " + sys.argv[1] + "\
	&& ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 " + sys.argv[1][:-1] + "o -o" + sys.argv[1][:-1] + "out" + " -lc\
	&& objcopy --dump-section .text=" + sys.argv[1][:-1] + "obj " + sys.argv[1][:-1] + "o")

	a = open(sys.argv[1][:-1] + "obj",'rb').read().hex();
	for i in range(0, len(a)):
		if i % 2 == 0:
			print("\\x", end='')
		print(a[i], end='')
	print()

	os.chdir(c_workplace)
	os.system("rm -r " + tmpdir);

	return 0

if __name__ == "__main__":
	exit(main())
