mkdir -p isofiles/boot
mkdir -p isofiles/initc

rm -f levi.iso

cp limine/limine-cd.bin isofiles/boot/
cp limine/limine-eltorito-efi.bin isofiles/boot/
cp limine/limine.sys isofiles/boot/
cp limine.cfg isofiles/boot/
cp kernel/build/levi isofiles/boot/
cp init/init.elf isofiles/init/init.elf

xorriso -as mkisofs -b boot/limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isofiles -o levi.iso

./limine/limine-install levi.iso