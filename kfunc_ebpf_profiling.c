#include <linux/types.h>
#include <asm/paravirt.h>
#include "linux/bpf.h"
#include "linux/btf.h"
#include <linux/module.h>
#include <linux/bpf.h>


__bpf_kfunc __u64 bpf_kep_read_counter(int counter) {
	__u64 ret; 
	rdpmcl(counter, ret);
	return ret;
}

// Sum of two u64 vectors of size 2 using _mm_add_epi64 
//__bpf_kfunc void bpf_kep_add_two_u64(__u64 *a, __u64 *b, __u64 *sum) {
//	_mm_add_epi64((__m64 *)a, (__m64 *)b, (__m64 *)sum);
//}

BTF_SET8_START(bpf_task_set);
BTF_ID_FLAGS(func, bpf_kep_read_counter);
BTF_SET8_END(bpf_task_set);

static const struct btf_kfunc_id_set bpf_task_kfunc_set = {
	.owner = THIS_MODULE,
	.set = &bpf_task_set,
};

int init_kep(void)
{
    register_btf_kfunc_id_set(BPF_PROG_TYPE_UNSPEC, &bpf_task_kfunc_set);
    //register_btf_kfunc_id_set(BPF_PROG_TYPE_XDP, &bpf_task_kfunc_set);
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
