extern "C" {
#include <flo/html-parser.h>
}
#include <sys/mman.h>

#include "html-example.h"

#define CAP 1U << 27U

int main() {
    flo_html_Arena a;
    a.beg = (char *)mmap(NULL, CAP, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (a.beg == MAP_FAILED) {
        a.beg = NULL;
        a.end = NULL;
        a.cap = 0;
    }
    a.end = a.beg + (ptrdiff_t)(CAP);
    a.cap = CAP;

    void *jmp_buf[5];
    if (__builtin_setjmp(jmp_buf)) {
        flo_html_destroyArena(&a);
        FLO_HTML_PRINT_ERROR("OOM in arena!\n");
        return 1;
    }
    a.jmp_buf = jmp_buf;
    // Read and print the html-test file.
    readAndPrintHTML(FLO_HTML_S("src/html-test.html"), a);

    // Operating system automatically cleans up allocated memory.
    return 0;
}
