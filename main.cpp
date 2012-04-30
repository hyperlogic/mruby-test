#include <stdio.h>

extern "C" {
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <compile.h>
}

static mrb_value a_sym;
static mrb_value b_sym;

static void dump(mrb_value obj)
{
    switch (mrb_type(obj))
    {
    case MRB_TT_FALSE: printf("\tnil\n"); break;
    case MRB_TT_FREE: printf("\tfree\n"); break;
    case MRB_TT_TRUE: printf("\ttrue\n"); break;
    case MRB_TT_FIXNUM: printf("\tfixnum %d\n", mrb_fixnum(obj)); break;
    case MRB_TT_SYMBOL: printf("\tsymbol\n"); break;
    case MRB_TT_UNDEF: printf("\tundef\n"); break;
    case MRB_TT_FLOAT: printf("\tfloat\n"); break;
    case MRB_TT_OBJECT: printf("\tobject\n"); break;
    case MRB_TT_CLASS: printf("\tclass\n"); break;
    case MRB_TT_MODULE: printf("\tmodule\n"); break;
    case MRB_TT_ICLASS: printf("\ticlass\n"); break;
    case MRB_TT_SCLASS: printf("\tsclass\n"); break;
    case MRB_TT_PROC: printf("\tproc\n"); break;
    case MRB_TT_ARRAY: printf("\tarray\n"); break;
    case MRB_TT_HASH: printf("\thash\n"); break;
    case MRB_TT_STRING: printf("\tstring\n"); break;
    case MRB_TT_RANGE: printf("\trange\n"); break;
    case MRB_TT_REGEX: printf("\tregex\n"); break;
    case MRB_TT_STRUCT: printf("\tstruct\n"); break;
    case MRB_TT_EXCEPTION: printf("\texception\n"); break;
    case MRB_TT_MATCH: printf("\tmatch\n"); break;
    case MRB_TT_FILE: printf("\tfile\n"); break;
    case MRB_TT_ENV: printf("\tenv\n"); break;
    case MRB_TT_DATA: printf("\tdata\n"); break;
    case MRB_TT_THREAD: printf("\tthread\n"); break;
    case MRB_TT_THREADGRP: printf("\tthreadgrp\n"); break;
    case MRB_TT_MAXDEFINE: printf("\tmaxdefine\n"); break;
    default: printf("\t???\n"); break;
    }
}

static mrb_value bar(mrb_state* mrb, mrb_value obj)
{
    mrb_value hash;
    mrb_get_args(mrb, "o", &hash);
    mrb_value a_val = mrb_hash_get(mrb, hash, a_sym);
    dump(a_val);
    mrb_value b_val = mrb_hash_get(mrb, hash, b_sym);
    dump(b_val);
    return mrb_nil_value();
}

int main()
{
    mrb_state* mrb = mrb_open();
    a_sym = mrb_symbol_value(mrb_intern(mrb, "a"));
    b_sym = mrb_symbol_value(mrb_intern(mrb, "b"));

    RClass* foo_mod = mrb_define_module(mrb, "Foo");
    mrb_define_class_method(mrb, foo_mod, "bar", bar, ARGS_REQ(1));

    const char* code = "Foo::bar({ :a => 1, :b => 2 })\n\
                        puts '*********'\n\
                        Foo::bar({ :yyy => 10, :a => 1, :xxx => 10, :b => 2 })";

    struct mrb_parser_state *p;
    p = mrb_parse_string(mrb, (char*)code);
    int n = mrb_generate_code(mrb, p->tree);
    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_top_self(mrb));
    if (mrb->exc) {
        mrb_p(mrb, mrb_obj_value(mrb->exc));
    }
    return 0;
}
