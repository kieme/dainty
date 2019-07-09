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

  using base::t_void;
  using base::t_bool;
  using base::t_nsec;
  using base::r_nsec;
  using base::t_usec;
  using base::r_usec;
  using base::t_msec;
  using base::r_msec;
  using base::t_sec;
  using base::r_sec;
  using base::t_min;
  using base::r_min;
  using base::t_ticks;
  using base::r_ticks;
  using base::t_prefix;

  template<typename T> struct t_test_;
  template<> struct t_test_<t_nsec> { using t_dummy_ = base::t_void; };
  template<> struct t_test_<t_usec> { using t_dummy_ = base::t_void; };
  template<> struct t_test_<t_msec> { using t_dummy_ = base::t_void; };
  template<> struct t_test_<t_sec>  { using t_dummy_ = base::t_void; };
  template<> struct t_test_<t_min>  { using t_dummy_ = base::t_void; };

///////////////////////////////////////////////////////////////////////////////

  class t_time;
  using r_time = t_prefix<t_time>::r_;
  using R_time = t_prefix<t_time>::R_;

  class t_time {
  public:
    constexpr t_time() noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_time(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr r_time operator+=(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr r_time operator-=(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_bool test_overflow(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr t_bool test_underflow(T) noexcept;

    template<typename T, typename = typename t_test_<T>::t_dummy_>
    constexpr T to() const noexcept;

    constexpr r_time operator+=(R_time) noexcept;
    constexpr r_time operator-=(R_time) noexcept;

    constexpr operator t_bool() const noexcept;

  private:
    friend constexpr       ::timespec& to_(r_time) noexcept;
    friend constexpr const ::timespec& to_(R_time) noexcept;
    ::timespec spec_;
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr t_time operator+(t_time lh, R_time rh) noexcept {
    return (lh += rh);
  }

  constexpr t_time operator-(t_time lh, R_time rh) noexcept {
    return (lh -= rh);
  }

  constexpr t_bool operator<(R_time lh, R_time rh) noexcept {
    return  to_(lh).tv_sec < to_(rh).tv_sec ||
           (to_(lh).tv_sec  == to_(rh).tv_sec &&
            to_(lh).tv_nsec <  to_(rh).tv_nsec);
  }

  constexpr t_bool operator>(R_time lh, R_time rh) noexcept {
    return  to_(lh).tv_sec > to_(rh).tv_sec ||
           (to_(lh).tv_sec  == to_(rh).tv_sec &&
            to_(lh).tv_nsec >  to_(rh).tv_nsec);
  }

  constexpr t_bool operator<=(R_time lh, R_time rh) noexcept {
    return  to_(lh).tv_sec < to_(rh).tv_sec ||
           (to_(lh).tv_sec  == to_(rh).tv_sec &&
            to_(lh).tv_nsec <= to_(rh).tv_nsec);
  }

  constexpr t_bool operator>=(R_time lh, R_time rh) noexcept {
    return  to_(lh).tv_sec > to_(rh).tv_sec ||
           (to_(lh).tv_sec  == to_(rh).tv_sec &&
            to_(lh).tv_nsec >= to_(rh).tv_nsec);
  }

  constexpr t_bool operator==(R_time lh, R_time rh) noexcept {
    return  to_(lh).tv_sec  == to_(rh).tv_sec &&
            to_(lh).tv_nsec == to_(rh).tv_nsec;
  }

  constexpr t_bool operator!=(R_time lh, R_time rh) noexcept {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<> struct t_test_<t_time> { using t_dummy_ = base::t_void;  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr t_time add(T t) { return {t}; }

  template<typename T, typename... Ts>
  constexpr t_time add(T t, Ts... ts) { return (add(t) += add(ts...)); }

///////////////////////////////////////////////////////////////////////////////

  t_time monotonic_now()      noexcept;
  t_time monotonic_now(t_err) noexcept;
  t_time realtime_now ()      noexcept;
  t_time realtime_now (t_err) noexcept;

///////////////////////////////////////////////////////////////////////////////

  inline t_ticks get_ticks()
  {
    t_ticks::t_value tmp;
#if   (defined(__i386__))
    __asm__ __volatile__("rdtsc": "=A" (tmp));
#elif (defined(__x86_64__))
    unsigned int a, d;
    asm volatile("rdtsc" : "=a" (a), "=d" (d));
    tmp = (((base::t_uint64)d) << 32) | a;
#elif (defined(__powerpc__) || defined(__ppc__))
    unsigned int tbl, tbu0, tbu1;
    do {
      __asm__ __volatile__("mftbu %0" : "=r"(tbu0));
      __asm__ __volatile__("mftb %0"  : "=r"(tbl));
      __asm__ __volatile__("mftbu %0" : "=r"(tbu1));
     } while (tbu0 != tbu1);
     tmp = (((base::t_uint64)tbu0) << 32) | tbl;
#endif
    return t_ticks{tmp};
  }

  struct t_ticks_scope { // not overflow safe
    r_ticks ticks;

     t_ticks_scope(r_ticks t) : ticks(t)        { ticks = get_ticks(); }
    ~t_ticks_scope() { ticks = t_ticks{get(get_ticks()) - get(ticks)}; }
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr ::timespec& to_(r_time time) noexcept {
    return time.spec_;
  }

  constexpr const ::timespec& to_(R_time time) noexcept {
    return time.spec_;
  }

  constexpr ::timespec to_(t_nsec nsec) noexcept {
    return { ::time_t(get(nsec)/1000000000),
             base::t_long(get(nsec)%1000000000)}; // narrow - XXX
  }

  constexpr ::timespec to_(t_usec usec) noexcept {
    return { ::time_t(get(usec)/1000000),
             base::t_long(get(usec)%1000000)}; // narrow - XXX
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

  constexpr t_nsec& to_(r_nsec nsec, const ::timespec& spec) noexcept {
    nsec = t_nsec(spec.tv_nsec + (1000000000*spec.tv_sec));
    return nsec;
  }

  constexpr t_usec& to_(r_usec usec, const ::timespec& spec) noexcept {
    usec = t_usec(spec.tv_nsec/1000 + (1000000*spec.tv_sec));
    return usec;
  }

  constexpr t_msec& to_(r_msec msec, const ::timespec& spec) noexcept {
    msec = t_msec(spec.tv_nsec/1000000 + (1000*spec.tv_sec));
    return msec;
  }

  constexpr t_sec& to_(r_sec sec, const ::timespec& spec) noexcept {
    sec = t_sec(spec.tv_sec);
    return sec;
  }

  constexpr t_min& to_(r_min min, const ::timespec& spec) noexcept {
    min = t_min(spec.tv_sec/60);
    return min;
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool overflow_(R_time, const ::timespec&) noexcept {
    return true; // impl later - XXX
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool underflow_(R_time, const ::timespec&) noexcept {
    return true; // impl later - XXX
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_void add_(R_time time, ::timespec& spec) noexcept {
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

  constexpr t_void minus_(R_time time, ::timespec& spec) noexcept {
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

  constexpr t_time::t_time() noexcept : spec_{to_(t_nsec{0})} {
  }

  template<typename T, typename>
  constexpr t_time::t_time(T value) noexcept : spec_{to_(value)} {
  }

  template<typename T, typename>
  constexpr r_time t_time::operator+=(T value) noexcept {
    add_(t_time{value}, spec_);
    return *this;
  }

  template<typename T, typename>
  constexpr r_time t_time::operator-=(T value) noexcept {
    minus_(t_time{value}, spec_);
    return *this;
  }

  constexpr r_time t_time::operator+=(R_time value) noexcept {
    add_(value, spec_);
    return *this;
  }

  constexpr r_time t_time::operator-=(R_time value) noexcept {
    minus_(value, spec_);
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

  constexpr t_time::operator t_bool() const noexcept {
    return spec_.tv_sec || spec_.tv_nsec;
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_time operator"" _nsec(unsigned long long value) noexcept {
    return {t_nsec(value)};
  }

  constexpr t_time operator"" _usec(unsigned long long value) noexcept {
    return {t_usec(value)};
  }

  constexpr t_time operator"" _msec(unsigned long long value) noexcept {
    return {t_msec(value)};
  }

  constexpr t_time operator"" _sec (unsigned long long value) noexcept {
    return {t_sec(value)};
  }

  constexpr t_time operator"" _min (unsigned long long value) noexcept {
    return {t_min(value)};
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_timed_scope {
    r_time time;

    inline
    t_timed_scope(r_time t) noexcept : time(t) { time = monotonic_now(); }
    inline
    ~t_timed_scope() noexcept        { time = (monotonic_now() -= time); }
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

}
}
}

#endif
