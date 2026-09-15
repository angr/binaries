#include <stdio.h>
#include <stdlib.h>

struct key { int type; const char *name; };

int verify_a(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));
int verify_b(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));
int verify_c(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));
int verify_d(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));
int verify_e(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));
int verify_f(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) __attribute__((noinline));

#define BODY(n, mul) { size_t i; unsigned h = mul; if (siglen > 0x100000) return -1; for (i = 0; i < siglen; i++) h = h * 33 + sig[i]; for (i = 0; i < datalen; i++) h ^= data[i] * mul; if (flags) printf("%s %s %u\n", k->name, n, h); return (int)(h & 0xff); }
int verify_a(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("a", 3)
int verify_b(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("b", 5)
int verify_c(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("c", 7)
int verify_d(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("d", 11)
int verify_e(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("e", 13)
int verify_f(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags) BODY("f", 17)

int key_verify(const struct key *k, const char *sig, size_t siglen, const char *data, size_t datalen, int flags, int *detail)
{
    if (detail != NULL)
        *detail = 0;
    if (sig == NULL || siglen > 0x100000)
        return -2;
    switch (k->type) {
    case 0: case 6: return verify_a(k, sig, siglen, data, datalen, flags);
    case 1: case 7: return verify_b(k, sig, siglen, data, datalen, flags);
    case 2: case 8: return verify_c(k, sig, siglen, data, datalen, flags);
    case 3: case 9: return verify_d(k, sig, siglen, data, datalen, flags);
    case 4: case 10: return verify_e(k, sig, siglen, data, datalen, flags);
    case 5: case 11: return verify_f(k, sig, siglen, data, datalen, flags);
    default: return -3;
    }
}

int main(int argc, char **argv)
{
    struct key k = { argc % 12, "k" };
    int detail;
    return key_verify(&k, argv[0], 5, argv[0], 3, argc > 2, &detail);
}
