TRGT  = fcmp
TGT_C = $(TRGT).c
TGT_O = $(TRGT).exe

# MinGW 15.1.0 64-bit
GCC   = D:\Portable\Programming\MinGW\15.1.0\bin\gcc
# LLVM 20.1.6 64-bit
CLANG = D:\Portable\Programming\LLVM\bin\clang

clean:
	rm -f $(TGT_O)

mingw: clean
	$(GCC) $(TGT_C) -o $(TGT_O) -std=gnu99

llvm: clean
	$(CLANG) $(TGT_C) -o $(TGT_O)
