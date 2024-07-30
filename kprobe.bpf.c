#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>



__u64 bpf_kep_read_counter(int counter) __ksym;

SEC("kprobe/tcp_v4_connect")
int bpf_prog(struct pt_regs *ctx)
{
    __u64 ret;
    ret = bpf_kep_read_counter(0);
    bpf_printk("tcp_v4_connect: %d\n", ret);
    return 0;
}

char _license[] SEC("license") = "GPL";
