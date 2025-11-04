#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/workqueue.h>
#include <linux/timekeeping.h>

#define PRINT_PREF "[tutorial-module] "
int irq = 9;
static atomic_t trigger_count = ATOMIC_INIT(0);
#define MODULE_STR "demo_irq"

struct work_struct work;

static void work_handler(struct work_struct *data) {
  printk(PRINT_PREF "running work handler, current time is %llu\n", ktime_get());

}

DECLARE_WORK(work, work_handler);

irqreturn_t ihr(int irq, void *dummy) {
  atomic_inc(&trigger_count);

  schedule_work(&work);

  return IRQ_HANDLED;
}

static int __init my_mod_init(void)
{
  int ret;
  printk(PRINT_PREF "Entering module.\n");

  LIST_HEAD(timer_vals_list);

  if (ret = request_irq(irq, ihr, IRQF_SHARED, MODULE_STR, &irq)) {
    printk(KERN_ERR PRINT_PREF "Can't allocate irq %d, return is %d\n", irq, ret);
    return -EBUSY;
  }


  return 0;
}

static void __exit my_mod_exit(void)
{
  free_irq(irq, &irq);
  printk(KERN_INFO PRINT_PREF "Exiting module, trigger count is %d\n", atomic_read(&trigger_count));
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CS-477 Course Staff");