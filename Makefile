
all:  bootloader kernel userland image

cppcheck:
	cppcheck --quiet --enable=all --force --inconclusive . 2> Tests/cppcheck/cppcheck-report.txt

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
