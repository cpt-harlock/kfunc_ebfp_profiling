#include "asm/paravirt.h"
#include "linux/btf.h"
#include <linux/module.h>
#include <linux/bpf.h>


__bpf_kfunc __u64 bpf_kep_read_counter(int counter) {
	__u64 ret; 
	rdpmcl(counter, ret);
	return ret;
}

int init_kep(void)
{
    printk(KERN_INFO "Hello world\n");
    return 0;
}

void cleanup_kep(void)
{
    printk(KERN_INFO "Goodbye world\n");
}

module_init(init_kep);
module_exit(cleanup_kep);

MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("harlock");
MODULE_DESCRIPTION("Hello world module");
