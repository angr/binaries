extern int host_value;
extern int flat_value;

int fixture_entry(void)
{
    return host_value + flat_value + 1;
}
