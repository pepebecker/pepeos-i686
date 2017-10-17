build: iso

kernel:
	mkdir -p dist/obj
	nasm -felf32 src/boot.asm -o dist/obj/boot.o
	i686-elf-gcc -m32 -c src/kernel.c -o dist/obj/kernel.o

	mkdir -p dist/bin
	i686-elf-gcc -T src/linker.ld -o dist/bin/kernel.bin -ffreestanding -O2 -nostdlib dist/obj/boot.o dist/obj/kernel.o -lgcc
	sh check_multiboot.sh

iso: kernel
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
