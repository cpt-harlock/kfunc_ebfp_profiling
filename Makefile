obj-m += kfunc_ebpf_profiling.o

all:
	compiledb make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	clang -O2 -target bpf -g -c pass.ebpf.c -o pass.ebpf.o
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
run:
	sudo insmod kfunc_ebpf_profiling.ko
	sudo bpftool prog load pass.ebpf.o /sys/fs/bpf/pass
	sudo bpftool net att xdp name pass dev veth0
	sudo ip link set veth0 up
	sudo ip link set veth1 up
stop:
	sudo bpftool net detach xdp dev veth0
	-sudo rm /sys/fs/bpf/pass
	sleep 1
	sudo rmmod kfunc_ebpf_profiling
	sudo ip link set veth0 down
	sudo ip link set veth1 down
trace:
	sudo bpftool prog trace
