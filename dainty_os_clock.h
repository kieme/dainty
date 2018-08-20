/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

#ifndef _DAINTY_OS_CLOCK_H_
#define _DAINTY_OS_CLOCK_H_

#include "dainty_os_call.h"

namespace dainty
{
namespace os
{
namespace clock
{
///////////////////////////////////////////////////////////////////////////////

  using named::t_void;
  using named::t_nsec;
  using named::t_usec;
  using named::t_msec;
  using named::t_sec;
  using named::t_min;
  using named::t_ticks;

  template<typename T> struct t_test_;
  template<> struct t_test_<t_nsec> { using t_dummy_ = named::t_void; };
  template<> struct t_test_<t_usec> { using t_dummy_ = named::t_void; };
  template<> struct t_test_<t_msec> { using t_dummy_ = named::t_void; };
  template<> struct t_test_<t_sec>  { using t_dummy_ = named::t_void; };
  template<> struct t_test_<t_min>  { using t_dummy_ = named::t_void; };

///////////////////////////////////////////////////////////////////////////////

  class t_time {
  public:
    constexpr t_time() noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_time(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_time& operator+=(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_time& operator-=(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_bool test_overflow(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_bool test_underflow(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr T to() const noexcept;

  private:
    friend constexpr       ::timespec& to_(t_time&) noexcept;
    friend constexpr const ::timespec& to_(const t_time&) noexcept;
    ::timespec spec_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<> struct t_test_<t_time> { using t_dummy_ = named::t_void;  };

///////////////////////////////////////////////////////////////////////////////

  constexpr t_time operator"" _nsec(unsigned long long value) {
    return {t_nsec(value)};
  }

  constexpr t_time operator"" _usec(unsigned long long value) {
    return {t_usec(value)};
  }

  constexpr t_time operator"" _msec(unsigned long long value) {
    return {t_msec(value)};
  }

  constexpr t_time operator"" _sec (unsigned long long value) {
    return {t_sec(value)};
  }

  constexpr t_time operator"" _min (unsigned long long value) {
    return {t_min(value)};
  }

  template<typename T>
  constexpr t_time add(T t) { return {t}; }

  template<typename T, typename... Ts>
  constexpr t_time add(T t, Ts... ts) { return (add(t) += add(ts...)); }

///////////////////////////////////////////////////////////////////////////////

  t_time monotonic_now();
  t_time monotonic_now(t_err);
  t_time realtime_now ();
  t_time realtime_now (t_err);

///////////////////////////////////////////////////////////////////////////////

  struct t_timed_scope {
    t_time& time;

    inline
    t_timed_scope(t_time& t) noexcept : time(t) { time = monotonic_now(); }
    inline
    ~t_timed_scope() noexcept         { time = (monotonic_now() -= time); }
  };

  class t_timer {
  public:
    enum t_start_tag_ { START };

    inline
    static t_time get_now() noexcept {
      return monotonic_now();
    }

    inline
    t_timer() noexcept : last_ {} {
    }

    inline
    t_timer(t_start_tag_) noexcept : last_(monotonic_now()) {
    }

    inline
    t_time get_last() const noexcept {
      return last_;
    }

    inline
    t_void restart() noexcept {
      last_ = get_now();
    }

    inline
    t_time elapsed() const noexcept {
      return (get_now() -= get_last());
    }

    inline
    t_time elapsed_restart() noexcept {
      t_time now{get_now()};
      t_time elapse{now};
      elapse -= last_;
      last_ = now;
      return elapse;
    };

  private:
    t_time last_;
  };

///////////////////////////////////////////////////////////////////////////////

  inline t_ticks get_ticks()
  {
    t_ticks::t_value tmp;
#if   (defined(__i386__))
    __asm__ __volatile__("rdtsc": "=A" (tmp));
#elif (defined(__x86_64__))
    unsigned int a, d;
    asm volatile("rdtsc" : "=a" (a), "=d" (d));
    tmp = (((named::t_uint64)d) << 32) | a;
#elif (defined(__powerpc__) || defined(__ppc__))
    unsigned int tbl, tbu0, tbu1;
    do {
      __asm__ __volatile__("mftbu %0" : "=r"(tbu0));
      __asm__ __volatile__("mftb %0"  : "=r"(tbl));
      __asm__ __volatile__("mftbu %0" : "=r"(tbu1));
     } while (tbu0 != tbu1);
     tmp = (((named::t_uint64)tbu0) << 32) | tbl;
#endif
    return t_ticks{tmp};
  }

  struct t_ticks_scope { // not overflow safe
    t_ticks& ticks;
    t_ticks_scope(t_ticks& t) : ticks(t)    { ticks = get_ticks(); }
    ~t_ticks_scope() { set(ticks) = get(get_ticks()) - get(ticks); }
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr ::timespec& to_(t_time& time) noexcept {
    return time.spec_;
  }

  constexpr const ::timespec& to_(const t_time& time) noexcept {
    return time.spec_;
  }

  constexpr ::timespec to_(t_nsec nsec) noexcept {
    return { ::time_t(get(nsec)/1000000000),
             named::t_long(get(nsec)%1000000000)}; // narrow - XXX
  }

  constexpr ::timespec to_(t_usec usec) noexcept {
    return { ::time_t(get(usec)/1000000),
             named::t_long(get(usec)%1000000)}; // narrow - XXX
  }

  constexpr ::timespec to_(t_msec msec) noexcept {
    return { get(msec)/1000, get(msec)%1000}; // narrow
  }

  constexpr ::timespec to_(t_sec sec) noexcept {
    return {get(sec), 0}; // narrow
  }

  constexpr ::timespec to_(t_min min) noexcept {
    return { get(min)*60, 0}; // narrow
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_nsec& to_(t_nsec& nsec, const ::timespec& spec) noexcept {
    set(nsec) = spec.tv_nsec + (1000000000*spec.tv_sec);
    return nsec;
  }

  constexpr t_usec& to_(t_usec& usec, const ::timespec& spec) noexcept {
    set(usec) = spec.tv_nsec/1000 + (1000000*spec.tv_sec);
    return usec;
  }

  constexpr t_msec& to_(t_msec& msec, const ::timespec& spec) noexcept {
    set(msec) = spec.tv_nsec/1000000 + (1000*spec.tv_sec);
    return msec;
  }

  constexpr t_sec& to_(t_sec& sec, const ::timespec& spec) noexcept {
    set(sec) = spec.tv_sec;
    return sec;
  }

  constexpr t_min& to_(t_min& min, const ::timespec& spec) noexcept {
    set(min) = spec.tv_sec/60;
    return min;
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool overflow_(const t_time& time,
                             const ::timespec& spec) noexcept {
    return true; // impl later - XXX
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool underflow_(const t_time& time,
                              const ::timespec& spec) noexcept {
    return true; // impl later - XXX
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_void add_(const t_time& time, ::timespec& spec) noexcept {
#ifdef DAINTY_OS_CLOCK_OVERFLOW_ASSERT
    if (overflow_(time, spec)) // impl later - XXX
      throw 1;
#endif
    spec.tv_sec  += to_(time).tv_sec;
    spec.tv_nsec += to_(time).tv_nsec;
    spec.tv_sec  += spec.tv_nsec/1000000000;
    spec.tv_nsec  = spec.tv_nsec%1000000000;
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_void minus_(const t_time& time, ::timespec& spec) noexcept {
#ifdef DAINTY_OS_CLOCK_OVERFLOW_ASSERT
    if (underflow_(time, spec)) // impl later - XXX
      throw 1;
#endif
    spec.tv_sec -= to_(time).tv_sec;
    if (to_(time).tv_nsec > spec.tv_nsec) {
      spec.tv_sec  -= 1;
      spec.tv_nsec += (1000000000 - to_(time).tv_nsec);
    } else
      spec.tv_nsec -= to_(time).tv_nsec;
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_time::t_time() noexcept : spec_{to_(0_nsec)} {
  }

  template<typename T, typename>
  constexpr t_time::t_time(T value) noexcept : spec_{to_(value)} {
  }

  template<typename T, typename>
  constexpr t_time& t_time::operator+=(T value) noexcept {
    add_(t_time{value}, spec_);
    return *this;
  }

  template<typename T, typename>
  constexpr t_time& t_time::operator-=(T value) noexcept {
    minus_(t_time{value}, spec_);
    return *this;
  }

  template<typename T, typename>
  constexpr T t_time::to() const noexcept {
    T value{0};
    return to_(value, spec_);
  }

  template<typename T, typename>
  constexpr t_bool t_time::test_overflow(T value) noexcept {
    return overflow_(t_time{value}, spec_);
  }

  template<typename T, typename>
  constexpr t_bool t_time::test_underflow(T value) noexcept {
    return underflow_(t_time{value}, spec_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
