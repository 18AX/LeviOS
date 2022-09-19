# Delete old iso
rm -f levi.iso

mkdir -p isofiles/boot

cp limine/limine-cd.bin isofiles/boot/
cp limine/limine-eltorito-efi.bin isofiles/boot/
cp limine/limine.sys isofiles/boot/
cp limine.cfg isofiles/boot/

# Copy the kernel
cp kernel/build/levi isofiles/

# Copy all files that will be in the initfs iso
cp init/init.elf initfs/init.elf

# Make the initfs iso
xorriso -as mkisofs initfs -o isofiles/initfs.iso

xorriso -as mkisofs -b boot/limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isofiles -o levi.iso

./limine/limine-install levi.iso