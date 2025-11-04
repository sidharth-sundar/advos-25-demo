#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/irqnr.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/types.h>
#include <linux/atomic.h>

#define PRINT_PREF "[tutorial-module] "

static int __init my_mod_init(void)
{
  printk(PRINT_PREF "Entering module.\n");
  return 0;
}

static void __exit my_mod_exit(void)
{
  printk(PRINT_PREF "Exiting module\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CS-477 Course Staff");