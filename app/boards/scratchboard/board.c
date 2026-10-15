#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int board_our_board_init(void)
{
	printk("Board initialized\n");
	return 0;
}

SYS_INIT(board_our_board_init, APPLICATION, 0);