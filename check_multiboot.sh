#!/usr/bin/env sh

if grub-file --is-x86-multiboot dist/bin/kernel.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi
