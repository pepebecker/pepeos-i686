CROSS_COMPILER = $(shell pwd)/cross-compiler/bin
export PATH := $(CROSS_COMPILER):$(PATH)

build: iso

kernel:
	mkdir -p dist/obj
	nasm -felf32 src/boot.asm -o dist/obj/boot.o
	$(CROSS_COMPILER)/i686-elf-gcc -m32 -c src/kernel.c -o dist/obj/kernel.o

	mkdir -p dist/bin
	$(CROSS_COMPILER)/i686-elf-gcc -T src/linker.ld -o dist/bin/kernel.bin -ffreestanding -O2 -nostdlib dist/obj/boot.o dist/obj/kernel.o -lgcc

iso: kernel
	sh check_multiboot.sh
	mkdir -p dist/isodir/boot/grub
	cp dist/bin/kernel.bin dist/isodir/boot/kernel.bin
	cp src/grub.cfg dist/isodir/boot/grub/grub.cfg
	grub-mkrescue -o dist/pepeos.iso dist/isodir

runk: kernel
	qemu-system-i386 -kernel dist/bin/kernel.bin

run: iso
	qemu-system-i386 -cdrom dist/pepeos.iso

clean:
	rm -rf dist
