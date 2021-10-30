
all:  bootloader kernel userland image
buddy: bootloader kernel_buddy userland image

cppcheck:
	cppcheck --quiet --enable=all --force --inconclusive . 2> Tests/cppcheck/cppcheck-report.txt

pvs-studio:
	make clean
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze --compiler gcc -j2 -o Tests/pvs-studio/PVS-Studio.log
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o Tests/pvs-studio/report.tasks Tests/pvs-studio/PVS-Studio.log
	mv strace_out Tests/pvs-studio/strace_out

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

kernel_buddy:
	cd Kernel; make buddy

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel kernel_buddy userland all clean
