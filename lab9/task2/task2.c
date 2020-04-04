#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#define TAG "week10"
MODULE_LICENSE("GPL");


int first=0;

int second=1;

int cnt = 1;

static struct hrtimer timer;
static ktime_t kt;

static s64 starttime_ns;

// Function to count next Fibonacci number and inrease the counter
void increment (void) {
    int x = second;
    second+=first;
    first = x;
    ++cnt;
}

// Main callback function. It prints the number
// and sets the timeout for the next call.
static enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
    printk(KERN_INFO "%d'th Fibonacci number is %d .\n", cnt, second);
    
    s64 now_ns = ktime_to_ns(ktime_get());
    increment();
    kt = ktime_set(0, second*100LL);
    hrtimer_forward(timer, timer->_softexpires, kt);

    return HRTIMER_RESTART;
}
 
// Module start function with timer initiation
int init_module(void)
{
    printk(KERN_INFO "Starting %s\n", TAG);
	kt = ktime_set(0,  100LL);
    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hrtimer_start(&timer, kt, HRTIMER_MODE_REL);
    timer.function = my_hrtimer_callback;
    return 0;
}

// Stopping the timer and exiting the module 
void cleanup_module(void)
{   
    printk(KERN_INFO "There has been a total of %d executions", cnt);
    printk(KERN_INFO "END %s\n", TAG);
    hrtimer_cancel( &timer );
}

