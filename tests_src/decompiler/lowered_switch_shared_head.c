__attribute__((noinline)) int shared_switch(int side, int first, int second, int nested)
{
    if (side) {
        switch (first) {
            case 3:
                goto shared;
            case 17:
                return 11;
            case 41:
                return 12;
            default:
                return 13;
        }
    }

    switch (second) {
        case 5:
            goto shared;
        case 23:
            return 21;
        case 47:
            return 22;
        default:
            return 23;
    }

shared:
    switch (nested) {
        case 7:
            return 31;
        case 29:
            return 32;
        case 53:
            return 33;
        default:
            return 34;
    }
}
