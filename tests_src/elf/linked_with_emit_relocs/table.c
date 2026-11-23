/* A freestanding image whose only path to two of its functions is a table of
   function pointers, the way u-boot's linker lists work. Built with a linker
   script that discards .dynstr, which leaves .hash and .dynsym with an sh_link
   that points at nothing -- the shape that used to make CLE throw the whole
   section header table away, .symtab included. */
static int counter;

int table_helper(int x) { return x * 3 + counter; }
int table_entry_one(void) { return table_helper(1); }
int table_entry_two(void) { return table_helper(2); }

int (*const entries[])(void) = {table_entry_one, table_entry_two};

void _start(void) { counter = entries[0]() + entries[1](); }
