/* xsc256.c */
#include <xsc256.h>

const int64 RCs[] = {
  1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987
};

// int64 fibonacci(int8 rnd, int8 stop, int64 x, int64 y) {
//     if ((rnd == stop) || (rnd > 15))
//         return (x+y);
//     else if (!rnd || (rnd == 1))
//         return fibonacci(++rnd, stop, 1, 1);
//     else
//         return fibonacci(++rnd, stop, y, (x+y));
// }

// void fibonaccis() {
//     int64 num, x;

//     printf("const int64 RCs[] = {\n  ");
//     for (x = 0; x < 16; x++) {
//         num = fibonacci(0, x, 0, 0);
//         printf("%lld,", $8 num);
//     }
//     printf("\n};\n");

//     return;
// }

int192 xbox(int128 input) {
    int192 output;
    int256 *p;

    output.x = 0;
    p = (int256 *)&output;
    *p = $32 (0 | input);
    return output;
}

void showint192(int8 *ident, int192 input) {
    void *mem;
    int32 *p;
    int8 i;

    printf("%s = 0x", ident);
    mem = $v ($v &input+24);
    for (i = 0; i < 6; i++) {
        mem -= 4;
        p = (int32 *)mem;
        printf("%.08x", $i *p);
    }
    printf("\n");

    return;
}

void showint128(int8 *ident, int128 input) {
    void *mem;
    int32 *p;
    int8 i;

    printf("%s = 0x", ident);
    mem = $v ($v &input+16);
    for (i = 0; i < 4; i++) {
        mem -= 4;
        p = (int32 *)mem;
        printf("%.08x", $i *p);
    }
    printf("\n");

    return;
}

int192 sboxes(int192 input) {
    semiword word;
    int192 input_;
    int16 in, in_, out;
    int8 i, msbs;

    input_ = input;
    for (i = 0; i < 16; i++) {
        show(input_);

        in = (input_.x & 0xffff);
        msbs = ((in >> 12) & 0x0f);
        word.x = (in & 0x0f);
        word.y = ((in >> 4) & 0x0f);
        word.z = ((in >> 8) & 0x0f);
        in_ = (in & 0x0fff);
        out = ((msbs << 16) | in_);
        word = sbox(word);
        input_.x <<= 12;
        input_.x |= out;
    }

    return input_;

}

void showroundkey(int8 *id, roundkey *rk) {
    assert(rk);
    printf("%s = {\n", $c id);
    printf(" id=%d\n", rk->id);
    printf(" rc=%lld\n  ", $8 rk->rc);
    show(rk->subkey);
    printf("}\n");

    return;
}

semiword sbox(semiword input) {
    return input;
}

int128 cbox(int192 input) {
    int128 output;

    output = input.x;

    return output;
}

int128 f(int128 input) {
    int128 output;

    output = cbox(sboxes(xbox(input)));

    return output;
}

int main() {
    roundkey *p;

    p = gensubkeys($16 0x50505050);

    do {
        show(p);
    } while(p = p->next);
    return 0;
}

// int main() {
//     fibonaccis();
// }

roundkey *mkroundkey(int8 id, int128 subkey, int64 rc) {
    int16 size;
    roundkey *p;

    size = sizeof(struct s_roundkey);
    p = alloc(size);
    assert(p);
    zero($1 p, size);

    p->id = id;
    p->subkey = subkey;
    p->next = (roundkey *)0;

    return p;
}

int128 mksubkey(int8 id, int128 subkey, int64 rc) {
    return subkey;
}

roundkey *gensubkeys(int128 key) {
    roundkey *p, *last, *first;
    int8 x;

    x = 0;
    first = mkroundkey(x, key, RCs[x]);
    x++;

    for (last = first; x < 16; x++) {
        p = mkroundkey(x, last->subkey, RCs[x]);
        last->next = p;
        last = p;
    }
    last->next = first;

    return first;
}

void zero(int8* x, int16 size) {
    int16 n;
    int8 *p;

    for (p = x, n = size; n; n--, p++)
        *p = 0;

    return;
}
