extern "C" {
#include "mruby.h"
#include "mruby/proc.h"
#include "compile.h"
}
#include <stdio.h>

int main(int argc, const char* argv[])
{
	mrb_state* mrb = mrb_open();

  int n;
  n = mrb_compile_string(mrb, "\
def fib(n)\n\
  if n<2\n\
    n\n\
  else\n\
    fib(n-2)+fib(n-1)\n\
  end\n\
end\n\
p(fib(30), \"\\n\")\n\
");
    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_nil_value());

	mrb_close(mrb);

	return 0;
}
