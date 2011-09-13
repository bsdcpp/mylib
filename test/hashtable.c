#include <stdio.h>
#include <stdlib.h>
#include <search.h>

char *data[] = { "alpha", "bravo", "charlie", "delta",
    "echo", "foxtrot", "golf", "hotel", "india", "juliet",
    "kilo", "lima", "mike", "november", "whisky", "papa",
    "quebec", "romeo", "sierra", "tango", "uniform",
    "victor", "whisky", "whisky", "x-ray", "yankee", "zulu"
};

    int
main(void)
{
    ENTRY e, *ep, *ep2;
    int i;

    /* starting with small table, and letting it grow does not work */
    hcreate(24);
    for (i = 0; i < 24; i++) {
        e.key = data[i];
        /* data is just an integer, instead of a
           pointer to something */
        e.data = (void *) i;
        if ((ep2 = hsearch(e, FIND)) == NULL) {
            ep = hsearch(e, ENTER);
        } else {
            printf("%d行的%s 与%d行的出现重复\n", (int)(e.data), e.key, (int)(ep2->data));
        }

        /* there should be no failures */
        if (ep == NULL) {
            fprintf(stderr, "entry failed\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("=======================================================\n");
    for (i = 22; i < 26; i++) {
        /* print two entries from the table, and
           show that two are not in the table */
        e.key = data[i];
        ep = hsearch(e, FIND);
        printf("%9.9s -> %9.9s:%d\n", e.key,
                ep ? ep->key : "NULL", ep ? (int)(ep->data) : 0);
    }
    exit(EXIT_SUCCESS);
}
