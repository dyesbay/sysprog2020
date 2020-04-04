#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#define TAG "week10"

static struct timer_list mytimer;
struct timespec tv;

//Simple function to print the current time
void currtime(void) {
   getnstimeofday(&tv);
   long long hours = (tv.tv_sec/3600 + 6)%24;
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   pr_info("Current time is: : %lld:%lld:%lld \n", hours, minutes, seconds);
}

// Callback function
// Simply prints current time and set a timer to run it in 1 second recursively
void exampleWithTimer(struct timer_list *t) {
   currtime();
   mytimer.expires = jiffies + msecs_to_jiffies(1000);
   timer_setup(&mytimer, exampleWithTimer, 0);

   add_timer(&mytimer);
}

// Module starts here
// Printing initial info and calling the callback function
int init_module(void) {
   printk(KERN_INFO "Starting %s\n", TAG);
   printk( "Starting timer to fire every 1000ms (%ld)\n", jiffies );

   exampleWithTimer(&mytimer);
   return 0;
}

// This fires at module removal
void cleanup_module(void) {
   printk(KERN_INFO "Cleanup %s\n", TAG);
   del_timer(&mytimer);
}
