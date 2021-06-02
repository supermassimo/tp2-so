include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: userModule sampleDataModule

userModule:
	cd UserModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd UserModule; make clean
	rm -rf *.bin

.PHONY: userModule all clean
