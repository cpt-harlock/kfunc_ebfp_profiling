#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

__u64 bpf_kep_read_counter(int counter) __ksym;

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, int);
    __type(value, int);
    __uint(max_entries, 32);
} btf_map SEC(".maps");

SEC("xdp")
int pass(struct xdp_md *ctx)
{
	__u64 counter_start = bpf_kep_read_counter(0);
	int* value;
	for (int i = 0; i < 10000; i++) {
		int j = i % 32;
		//value = bpf_map_lookup_elem(&btf_map, &j);
		value = &j;
		if (value) {
			bpf_printk("hello");
		}
	}
	__u64 counter_end = bpf_kep_read_counter(0);
	bpf_printk("bpf_kep_read_counter: %lu\n", counter_end - counter_start);
	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
