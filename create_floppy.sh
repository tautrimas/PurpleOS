#!/bin/bash

# By David Lindsay <address@hidden>
# I wouldn't have discovered the --device-map/dev/null and "device
#(hd0) /dev/loopX" tricks had I not found http://sig9.com/bochs-grub,
#so I must thank "Vivek" for that vital knowledge!

# Some required info:

# Image name (include a path if you want)
imgname=floppy.img

# Loopback device
loopdevice=/dev/loop1

# Filesystem
fs=ext2
# OR
# fs=msdos

# Image mountpoint (NEEDS TO EXIST):
mntpath=/mnt/floppy2

# Location of stage1 and stage2
stage1=stage1
stage2=stage2

# ---

# 1. Create a floppy disk image:
! dd if=/dev/zero of=$imgname bs=1024 count=1440
# OR
# ! qemu-img create $imgname 1440

# 2. Setup a loopback device
losetup $loopdevice $imgname

# 3. Create a filesystem:
mkfs -V -t $fs $loopdevice

# 4. Mount it somewhere:
mount -o loop $loopdevice $mntpath

# 5. Copy stage1 and stage2 to it:
cp $stage1 $mntpath
cp $stage2 $mntpath

# 7. Unmount the image:
umount $mntpath

# 8. Use the GRUB shell to work its magic on stage1 and stage2 so they boot
echo -e "device (fd0) $loopdevice\nroot (fd0)\ninstall /$(basename \
$stage1) d (fd0) /$(basename $stage2)\nquit" | grub \
--device-map=/dev/null --batch

# 9. Remount the image
mount -o loop $loopdevice $mntpath

# 10. Remove stage1 and stage2, which don't appear to be needed
# anymore and will only consume unneccessary space on the disk - I've
# personally had success removing both files under both FAT and ext2.
# rm $mntpath/$(basename $stage1)
# rm $mntpath/$(basename $stage2)

# 11. Copy any extra files you want to the disk at this point.
# Note: I have this step here because until step 10 you had around
# 100K of data on the image that would have been deleted anyway.

mkdir -p $mntpath/boot/grub
cp menu.lst $mntpath/boot/grub/grub.conf
cp menu.lst $mntpath/boot/grub/menu.lst
cp kernel.bin $mntpath
cp README $mntpath

# 12. Unmount the image. Again. :P
umount $mntpath

# 13. Finally delete the loopback device.
losetup $loopdevice -d
