// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include "hello1.h"

MODULE_AUTHOR("Smirnov Bohdan <mrgumor2017@gmail.com>");
MODULE_DESCRIPTION("Hello1, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

struct my_list_item {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(my_list);



void print_hello(unsigned int count)
{
	struct my_list_item *item;
	int i;

	for (i = 0; i < count; i++) {
		item = kmalloc(sizeof(struct my_list_item), GFP_KERNEL);
		if (!item) {
			pr_err("Failed to allocate memory for list item\n");
			return;
		}

		item->start_time = ktime_get();
		INIT_LIST_HEAD(&item->list);
		list_add(&item->list, &my_list);

		pr_info("Hello, world!\n");

		item->end_time = ktime_get();
	}
}
EXPORT_SYMBOL(print_hello);

static void print_list(void)
{
	struct my_list_item *item;
	struct list_head *pos;

	list_for_each(pos, &my_list) {
		item = list_entry(pos, struct my_list_item, list);
		pr_info("Start time: %lld ns, End time: %lld ns, Duration: %lld ns\n",
			ktime_to_ns(item->start_time),
			ktime_to_ns(item->end_time),
			ktime_to_ns(item->end_time) - ktime_to_ns(item->start_time));
	}
}


static void clear_list(void)
{
	struct my_list_item *item;
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, &my_list) {
		item = list_entry(pos, struct my_list_item, list);
		list_del(pos);
		kfree(item);
	}
}

static int __init hello1_init(void)
{
	pr_info("Hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	print_list();
	clear_list();
	pr_info("Hello1 module unloaded\n");
}


module_init(hello1_init);
module_exit(hello1_exit);
