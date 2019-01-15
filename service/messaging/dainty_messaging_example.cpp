#include <unistd.h>
#include "dainty_messaging.h"

using namespace dainty::messaging;

int main() {
  t_err err;
  {
    messenger::t_messenger mgr1 = create_messenger(err, P_cstr{"messenger1"});
    messenger::t_messenger mgr2 = create_messenger(err, P_cstr{"messenger2"});
  }

  sleep(2);
  return 0;
}
