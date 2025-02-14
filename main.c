#include "pocketpy.h"
#include <stdio.h>
#include <stdlib.h>
#include "null0.h"

static py_Ref py_load_func = 0;
static py_Ref py_update_func = 0;

// C implementations that call Python
void load() {
    if (py_load_func) {
        py_push(py_load_func);
        py_pushnil();
        py_vectorcall(0, 0);
    }
}

void update(float deltaTime) {
    if (py_update_func) {
        py_push(py_update_func);
        py_pushnil();
        py_newfloat(py_getreg(0), deltaTime);
        py_vectorcall(1, 0);
    }
}

static bool add_numbers(int argc, py_Ref argv) {
    PY_CHECK_ARGC(2);                   // verify we got 2 arguments
    PY_CHECK_ARG_TYPE(0, tp_int);       // verify first arg is int
    PY_CHECK_ARG_TYPE(1, tp_int);       // verify second arg is int
    py_i64 a = py_toint(py_arg(0));     // get first argument
    py_i64 b = py_toint(py_arg(1));     // get second argument
    py_newint(py_retval(), a + b);      // return the sum
    return true;
}

static bool say_hello(int argc, py_Ref argv) {
    PY_CHECK_ARGC(1);
    PY_CHECK_ARG_TYPE(0, tp_str);

    const char* name = py_tostr(py_arg(0));
    printf("Hello, %s!\n", name);

    py_newnone(py_retval());
    return true;
}

// helper for Color arguments
static Color py_get_color(py_Ref color_ref) {
    // Assume color_ref is a tuple/list of 4 numbers (r,g,b,a)
    py_Ref r = py_list_getitem(color_ref, 0);
    py_Ref g = py_list_getitem(color_ref, 1);
    py_Ref b = py_list_getitem(color_ref, 2);
    py_Ref a = py_list_getitem(color_ref, 3);

    return (Color){
        .r = py_toint(r),
        .g = py_toint(g),
        .b = py_toint(b),
        .a = py_toint(a)
    };
}

// Example binding for clear()
static bool py_clear(int argc, py_Ref argv) {
    PY_CHECK_ARGC(2);
    u32 destination = py_toint(py_arg(0));
    Color color = py_get_color(py_arg(1));
    clear(destination, color);
    py_newnone(py_retval());
    return true;
}

// Example binding for draw_line()
static bool py_draw_line(int argc, py_Ref argv) {
    PY_CHECK_ARGC(7);
    u32 destination = py_toint(py_arg(0));
    i32 startPosX = py_toint(py_arg(1));
    i32 startPosY = py_toint(py_arg(2));
    i32 endPosX = py_toint(py_arg(3));
    i32 endPosY = py_toint(py_arg(4));
    u32 thickness = py_toint(py_arg(5));
    Color color = py_get_color(py_arg(6));

    draw_line(destination, startPosX, startPosY, endPosX, endPosY, thickness, color);
    py_newnone(py_retval());
    return true;
}

int main() {
    py_initialize();
    FILE* f = fopen("main.py", "r");
    if (!f) {
        printf("Could not open main.py\n");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    py_Ref r0 = py_getreg(0);

    py_newnativefunc(r0, add_numbers);
    py_setglobal(py_name("add_numbers"), r0);

    py_newnativefunc(r0, say_hello);
    py_setglobal(py_name("say_hello"), r0);

    py_newnativefunc(r0, py_clear);
    py_setglobal(py_name("clear"), r0);

    py_newnativefunc(r0, py_draw_line);
    py_setglobal(py_name("draw_line"), r0);

    bool ok = py_exec(buffer, "main.py", EXEC_MODE, NULL);
    free(buffer);

    if (!ok) {
        py_printexc();
        py_finalize();
        return 1;
    }

    py_load_func = py_getglobal(py_name("load"));
    py_update_func = py_getglobal(py_name("update"));

    // exmaple call
    load();
    update(0.016f);

    py_finalize();
    return 0;
}
