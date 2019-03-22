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

#ifndef _DAINTY_NAMED_STRING_2_H_
#define _DAINTY_NAMED_STRING_2_H_

#include "dainty_named_string_1.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  class t_string<TAG, N, OVERFLOW_GROW> {
    using t_impl_ = t_string_impl_<OVERFLOW_ASSERT>;
  public:
    using t_n      = named::t_n;
    using P_cstr   = named::P_cstr;
    using R_crange = t_prefix<t_crange>::R_;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;
    using t_char   = typename t_impl_::t_char;
    using R_block  = typename t_impl_::R_block;

    t_string();
    t_string(P_cstr);
    t_string(R_block);
    t_string(R_crange);
    t_string(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    t_string(R_string);

    template<t_overflow O1>
    t_string(t_string<TAG, 0, O1>&&);

    template<t_n_ N1>
    t_string(const t_char (&)[N1]);
    template<t_n_ N1, t_overflow O1>
    t_string(const t_string<TAG, N1, O1>&);

    r_string operator=(P_cstr);
    r_string operator=(R_block);
    r_string operator=(R_crange);
    r_string operator=(R_string);
    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1]);
    template<t_n_ N1, t_overflow O1>
    r_string operator=(const t_string<TAG, N1, O1>&);
    template<t_overflow O1>
    r_string operator=(t_string<TAG, 0, O1>&&);

    r_string assign(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string assign(const t_string<TAG1, N1, O1>&);

    r_string append(P_cstr);
    r_string append(R_block);
    r_string append(R_crange);
    r_string append(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<t_n_ N1>
    r_string append(const t_char (&)[N1]);
    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string append(const t_string<TAG1, N1, O1>&);

    r_string va_assign(P_cstr_ fmt, va_list vars);
    r_string va_append(P_cstr_ fmt, va_list vars);

    t_void clear();

    t_void display() const;
    t_void display_then_clear();

    t_bool is_match(P_cstr pattern) const;
    template<t_n_ N1>
    t_bool is_match(const t_char (&pattern)[N1]) const;
    template<class TAG1, t_n_ N1, t_overflow O1>
    t_bool is_match(const t_string<TAG1, N1, O1>& pattern) const;

    constexpr static
    t_n    get_capacity();
    t_n    get_count   (t_char) const;
    P_cstr get_cstr    () const;
    t_n    get_length  () const;
    t_bool is_empty    () const;
    t_char get_front   () const;
    t_char get_back    () const;

    t_crange mk_range()           const;
    t_crange mk_range(t_ix)       const;
    t_crange mk_range(t_ix, t_ix) const;

    template<class F> void  each(F);
    template<class F> void  each(F) const;
    template<class F> void ceach(F) const;

    t_void mod_(t_ix pos, t_char);

  private:
    template<class, t_n_, t_overflow> friend class t_string;
    t_char  sso_[N+1];
    p_cstr_ store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
