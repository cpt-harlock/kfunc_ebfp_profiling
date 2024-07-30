obj-m += kfunc_ebpf_profiling.o

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

SRCS = $(wildcard *.bpf.c)
OBJS = $(SRCS:.bpf.c=.bpf.o)

all: $(OBJS) module

clean:
	@echo "$(RED)Cleaning$(NC)"
	rm -f *.o *.ko *.mod.c *.order *.symvers *.mod.o *.bpf.o
module:
	@echo "$(GREEN)Building module$(NC)"
	compiledb make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

%.bpf.o: %.bpf.c
	@echo "$(GREEN)Compiling $<$(NC)"
	clang -O2 -target bpf -g -c $< -o $@

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

.PHONY: all clean 
