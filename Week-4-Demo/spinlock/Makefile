MOD_NAME=sync_spinlock
obj-m += $(MOD_NAME).o


KDIR="/lib/modules/$(shell uname -r)/build"

all:
	make -C $(KDIR) M=$(PWD) modules

# To avoid the password prompt each time you do make install, just
# install the host ssh key into the qemu VM by doing (on the host):
# ssh-keygen
# ssh-copy-id <ip address of the qemu vm>

clean:
	make -C $(KDIR) M=$(PWD) clean

