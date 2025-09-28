#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>


#define PRINT_PREF "[SYNC_SPINLOCK]: "
#define POLL_INTERVAL 1000 /* poll for kthread_stop every second */
#define MAX_CONTENDING_THREADS 2
static int created_threads = 0;

			     
unsigned int ops_per_thread = 1000; 
unsigned int num_readers = 1;

module_param(ops_per_thread, int, 0664);
module_param(num_readers, int, 0664);

unsigned int counter;	/* shared data: */

DEFINE_SPINLOCK(counter_lock);
struct task_struct *threads[10];

static int writer_function(void *data)
{
  int i = 0;
  while (i < ops_per_thread && !kthread_should_stop()) {
    
    spin_lock(&counter_lock);
    counter++;
    spin_unlock(&counter_lock);

    i++;
    //msleep(500);
  }
  while(!kthread_should_stop()) {
    msleep_interruptible(POLL_INTERVAL);
  }
  //  do_exit(0);
  return 0;
}

static int read_function(void *data)
{
  int i = 0;
  while (i < ops_per_thread && !kthread_should_stop()) {

    spin_lock(&counter_lock);
    printk(PRINT_PREF "counter: %d\n", counter);
    spin_unlock(&counter_lock);

    i++;
    //msleep(500);
  }
  while(!kthread_should_stop()) {
    msleep_interruptible(POLL_INTERVAL);
  }
  //  do_exit(0);
  return 0;
}

static int __init my_mod_init(void)
{
  printk(PRINT_PREF "Entering module.\n");
  counter = 0;

  if (num_readers > MAX_CONTENDING_THREADS) {
    goto err;
  }
  
  for (int i = 0; i < num_readers; i++) {
    threads[i] = kthread_run(read_function, NULL, "read-thread");
    if (IS_ERR(threads[i])){
      goto terminate_threads_err;
    }
    created_threads++;
  }

  for (int i = num_readers; i < MAX_CONTENDING_THREADS; i++) {
    threads[i] = kthread_run(writer_function, NULL, "writer-thread");
    if (IS_ERR(threads[i])){
      goto terminate_threads_err;
    }
    created_threads++;
  }


  
  //threads[0] = kthread_run(read_function, NULL, "read-thread");
  //threads[1] = kthread_run(writer_function, NULL, "write-thread");

  return 0;

 terminate_threads_err:
  for (int i = 0; i < created_threads; i++) {
    kthread_stop(threads[i]);
  }
  return PTR_ERR(threads[created_threads]);

 err:
  return -ENOENT;
}

static void __exit my_mod_exit(void)
{
  
  for (int i = 0; i < MAX_CONTENDING_THREADS; i++) {
    kthread_stop(threads[i]);
  }

  printk(KERN_INFO "Exiting module.\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
