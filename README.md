# Kernel based on x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

The following project is a basic OS based on x64BareBones by RowDaBoat. It implements features such as a memory manager, a scheduler and pipes. It also has an alternative memory manager, which is a buddy system.

## Environment setup:

1- Install the following packages before building the Toolchain and Kernel:

```bash
nasm qemu gcc make
```

2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

```bash
  user@linux:$ cd Toolchain
  user@linux:$ make all
```

3- Build the Kernel

From the x64BareBones project directory run:

```bash
  user@linux:$ make all
```

Another option is to run make buddy, to run the project using the buddy system:

```bash
  user@linux:$ make buddy
```

4- Run the kernel

From the x64BareBones project directory run:

```bash
  user@linux:$ ./run.sh
```