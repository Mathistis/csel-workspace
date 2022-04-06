cd /workspace/src/03_kernel/exercice03/
rmmod mymodule.ko
rm /dev/mymodule*
insmod ./mymodule.ko instances=2
mknod /dev/mymodule c 511 0
mknod /dev/mymodule1 c 511 1
mknod /dev/mymodule2 c 511 2
echo "test0" > /dev/mymodule
echo "test1" > /dev/mymodule1
echo "test2" > /dev/mymodule2
cat /dev/mymodule
cat /dev/mymodule1
cat /dev/mymodule2