#include <unistd.h>
#include "dainty_tracing.h"
#include "dainty_mt_detached_thread.h"

using namespace dainty::tracing;
using namespace dainty::mt;
using namespace dainty::os;


struct worker : public detached_thread::t_thread::t_logic
{
  worker(dainty::tracing::t_err err, int i)
    : tracer_(make_tracer(err, t_tracer_name{FMT, "tracer_%d", i})) {
  }

  virtual t_void update(t_err, r_pthread_attr) noexcept {
  }

  virtual t_void prepare(t_err err) noexcept {
  }

  virtual t_void run() noexcept {
    while (1) {
      DAINTY_TR_DEBUG(tracer_, "tracing......");
      usleep(5);
    }
  }

  t_tracer tracer_;
};

int main() {
  dainty::tracing::t_err err;
  {
    //detached_thread::t_thread thread1(err, P_cstr{"worker1"}, new worker(err, 1), true);
    //detached_thread::t_thread thread2(err, P_cstr{"worker2"}, new worker(err, 2), true);
    //detached_thread::t_thread thread3(err, P_cstr{"worker3"}, new worker(err, 3), true);
    //detached_thread::t_thread thread4(err, P_cstr{"worker4"}, new worker(err, 4), true);
    //detached_thread::t_thread thread5(err, P_cstr{"worker5"}, new worker(err, 5), true);

    t_observer_name observer_name("logger");
    t_tracer_name   tracer_name  ("frits");

    create_observer(err, observer_name);
    t_tracer tracer = make_tracer(err, tracer_name);

    bind_tracer(err, observer_name, tracer_name);
    t_point point = tracer.make_point("dick");

    t_observer_name observer_name1("logger1");
    create_observer(err, observer_name1);
    bind_tracer(err, observer_name1, tracer_name);

    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_ALERT(point,  "alert");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_ALERT(point,  "alert");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    DAINTY_TR_DEBUG(tracer, "hallo");
    DAINTY_TR_DEBUG(point,  "hallo");
    sleep(1);

    DAINTY_TR_ERROR(point,  "error");

    DAINTY_TR_ALERT(point,  "alert");

    unbind_tracers(err, observer_name1, t_wildcard_name{tracer_name.c_str()});
    //for (int i = 0; i < 10000; ++i)
    //  DAINTY_TR_DEBUG(point,  "yes");

    sleep(1);
  }

  /*
  sleep(2);
  destroy();
  {
    start(err);
  }
  */
  sleep(1);
};
