#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/timekeeping.h>

#define PRINT_PREF "[SYNC_RWSPINLOCK]: "
#define POLL_INTERVAL 1000 /* poll for kthread_stop every second */
#define MAX_CONTENDING_THREADS 10
static int created_threads = 0;

			     
unsigned int ops_per_thread = 1000000; 
unsigned int num_readers = 5;

module_param(ops_per_thread, int, 0664);
module_param(num_readers, int, 0664);

unsigned int counter;	/* shared data: */

DEFINE_RWLOCK(counter_lock);
struct task_struct *threads[MAX_CONTENDING_THREADS];

static int writer_function(void *data)
{
  pid_t tid = current->pid;
  ktime_t start_time = ktime_get();

  int i = 0;
  while (i < ops_per_thread && !kthread_should_stop()) {
    
    write_lock(&counter_lock);
    counter++;
    write_unlock(&counter_lock);

    i++;
    //    msleep_interruptible(1);
  }

  ktime_t end_time = ktime_get();
  
  while(!kthread_should_stop()) {
    msleep_interruptible(POLL_INTERVAL);
  }

  printk(PRINT_PREF "(thread %d) Total time for writer thread to perform %d ops is %lld ns (start %lld)\n", tid, i,  end_time - start_time, start_time);
  
  return 0;
}

static int read_function(void *data)
{
  pid_t tid = current->pid;
  ktime_t start_time = ktime_get();

  unsigned int *arr = kmalloc(ops_per_thread * sizeof(unsigned int), GFP_KERNEL);
  if (arr == NULL) {
    goto err;
  }

  int i = 0;
  while (i < ops_per_thread && !kthread_should_stop()) {

    read_lock(&counter_lock);
    // printk(PRINT_PREF "counter: %d\n", counter);
    arr[i] = counter;
    read_unlock(&counter_lock);

    i++;
    //    msleep_interruptible(1);
  }

  ktime_t end_time = ktime_get();
  
  while(!kthread_should_stop()) {
    msleep_interruptible(POLL_INTERVAL);
  }

  /* if you want to check values read during read_function, parse the array here */

  printk(PRINT_PREF "(thread %d) Total time for reader thread to perform %d ops is %lld ns (start %lld)\n", tid, i, end_time - start_time, start_time);
  printk(PRINT_PREF "(thread %d) Last read value: %u\n", tid, arr[i-1]);


  kfree(arr);
  return 0;

 err:
  printk(PRINT_PREF "Reader failed to start, unable to allocate array\n");
  return -ENOMEM;
}

static int __init my_mod_init(void)
{
  printk(PRINT_PREF "Entering module.\n");
  counter = 0;

  if (num_readers > MAX_CONTENDING_THREADS || num_readers < 0) {
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

  printk(KERN_INFO "Exiting module. Final value of contended int is %u\n", counter);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
