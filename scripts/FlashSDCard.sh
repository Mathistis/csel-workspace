DEV=/dev/
IMG_FOLDER=/home/mathis/MSE/CSEL/csel-workspace/buildroot-images

if [[ $1 == sd* ]]
then
    if [ $1 == "sda" ]
    then
        echo "sda is not a good idea ...."
        exit 1
    fi
    echo "Found"
    DEV+=$1
    
    if [ -e $DEV ]
    then
        echo "Wrinting in : $DEV"
    else
        echo "Wrong file: $DEV"
        exit 1
    fi
else
    echo "Please enter a valide device file descriptor : $0 <sdb>"
    exit 1
fi
BOOT=${DEV}1
ROOTFS=${DEV}2
SUPP=${DEV}3

echo "Erasing $DEV"
sudo dd if=/dev/zero of=$DEV bs=4k count=120000 status=progress
if [ $? -ne 0 ]
then
    echo "Error Erasing $DEV"
    exit 1;
fi

echo "Copying image"
sudo dd if=$IMG_FOLDER/sdcard.img of=$DEV bs=512 status=progress
#sudo dd if=$IMG_FOLDER/sdcard.img of=$DEV bs=512 status=progress
if [ $? -ne 0 ]
then
    echo "Error Copying Image"
    exit 1;
fi

echo "Generating CoolExt4FS"
sudo parted -s -a optimal $DEV mkpart primary ext4 4329472s  5329472s
if [ $? -ne 0 ]
then
    echo "Error generating new partition"
    exit 1;
fi

sudo mkfs.ext4 $SUPP -L CoolExt4FS
sync


## Preparing boot partition
mkdir /tmp/mathis
sudo mount -t vfat $BOOT /tmp/mathis/
sudo cp $IMG_FOLDER/../boot-scripts/boot_cifs.cmd /tmp/mathis/
sudo cp $IMG_FOLDER/../uboot_env/uboot.env
sudo umount /tmp/mathis
##-----


# sudo mount $BOOT /media/lmi/
# sudo cp ~/workspace/nano/buildroot/output/images/Image /media/lmi
# sudo cp ~/workspace/nano/buildroot/output/images/nanopi-neo-plus2.dtb /media/lmi
# sudo cp ~/workspace/nano/buildroot/output/images/boot.scr /media/lmi
# sudo cp ~/MSE/SeS/uInitrd /media/lmi
# sync

# sudo umount $BOOT
# sudo e2label $BOOT BOOT
# sudo dd if=~/workspace/nano/buildroot/output/images/rootfs.ext4 of=$ROOTFS

# sudo umount $BOOT
# sudo umount $ROOTFS

# sudo e2fsck -f $ROOTFS
# sudo resize2fs $ROOTFS
# sudo e2label $ROOTFS rootfs


#setenv bootargs console=ttyS0,115200 earlyprintk root=/dev/mmcblk0p2 rootwait
#ext4load mmc 0 $kernel_addr_r Image
#ext4load mmc 0 $fdt_addr_r nanopi-neo-plus2.dtb
#booti $kernel_addr_r - $fdt_addr_r
