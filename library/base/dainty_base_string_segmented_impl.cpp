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

#include <utility>
#include <cstring>
#include "dainty_base_string_segmented.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace segmented
{
///////////////////////////////////////////////////////////////////////////////

  P_char find_(P_char buf, P_char end, t_crange range) noexcept {
    t_n_ range_n = get(range.n);
    P_char ptr = buf;
    while (ptr < end) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      t_n_ len = hdr.hdr.len;
      t_n_ min = len < range_n ? len : range_n;
      if (len == range_n && std::strncmp(ptr + HDR_MAX_, range.ptr, min) == 0)
        return ptr;
      ptr += HDR_MAX_ + len;
    }
    return end;
  }

  P_char find_next_(P_char buf, P_char end, t_crange range) noexcept {
    t_n_ range_n = get(range.n);
    P_char ptr = buf;
    if (ptr < end) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      ptr += HDR_MAX_ + hdr.hdr.len;
      while (ptr < end) {
        t_seg_hdr_ hdr{ptr[0], ptr[1]};
        t_n_ len = hdr.hdr.len;
        t_n_ min = len < range_n ? len : range_n; // can improve
        if (len == range_n &&
            std::strncmp(ptr + HDR_MAX_, range.ptr, min) == 0)
          return ptr;
        ptr += HDR_MAX_ + len;
      }
    }
    return end;
  }

  inline
  p_char push_back_(p_char buf, t_user usr) noexcept {
    t_seg_hdr_ hdr{0, usr};
    *buf++ = hdr.raw[0];
    *buf++ = hdr.raw[1];
    return buf;
  }

  inline
  p_char push_back_(p_char buf, t_crange range, t_user usr) noexcept {
    t_n_ range_n = get(range.n);
    t_seg_hdr_ hdr{range_n, usr};
    *buf++ = hdr.raw[0];
    *buf++ = hdr.raw[1];
    std::memcpy(buf, range.ptr, range_n);
    return buf + range_n;
  }

  inline
  p_char change_(p_char buf, p_char next, t_user usr) noexcept {
    t_seg_hdr_ hdr1{buf[0], buf[1]}, hdr2{0, usr};
    buf[0] = hdr2.raw[0];
    buf[1] = hdr2.raw[1];
    if (hdr1.hdr.len) {
      t_n_ size = HDR_MAX_ + hdr1.hdr.len;
      if (next != buf + size)
        std::memmove(buf, buf + size, (next - buf) - size);
      next -= size;
    }
    return next;
  }

  inline
  p_char insert_(p_char buf, p_char next, t_user user) noexcept {
    t_seg_hdr_ hdr{0, user};
    std::memmove(buf + HDR_MAX_, buf, next - buf);
    *buf++ = hdr.raw[0];
    *buf++ = hdr.raw[1];
    return next + HDR_MAX_;
  }

  inline
  p_char insert_(p_char buf, p_char next, t_crange range,
                 t_user user) noexcept {
    T_n_ len = get(range.n);
    t_seg_hdr_ hdr{len, user};
    std::memmove(buf + HDR_MAX_ + hdr.hdr.len, buf, next - buf);
    *buf++ = hdr.raw[0];
    *buf++ = hdr.raw[1];
    std::memcpy(buf, range.ptr, len);
    return next + HDR_MAX_ + hdr.hdr.len;
  }

  inline
  p_char change_(p_char buf, p_char end, t_crange range, t_user usr) noexcept {
    t_seg_hdr_ hdr1{buf[0], buf[1]};
    t_n_ new_len = get(range.n), old_len = hdr1.hdr.len;
    t_seg_hdr_ hdr2{new_len, usr};
    *buf++ = hdr2.raw[0];
    *buf++ = hdr2.raw[1];
    p_char next = buf + old_len;
    if (new_len < old_len) {
      std::memcpy(buf, range.ptr, new_len);
      if (next != end)
        std::memmove(buf + new_len, next, (end - next));
      end -= old_len - new_len;
    } else if (new_len > old_len) {
      t_n_ diff = new_len - old_len;
      if (next != end)
        std::memmove(next + diff, next, (end - next));
      std::memcpy(buf, range.ptr, new_len);
      end += diff;
    } else
      std::memcpy(buf, range.ptr, new_len);
    return end;
  }

  inline
  p_char remove_(p_char buf, p_char next, t_n n) noexcept {
    t_n_   max = get(n), cnt = 0;
    p_char ptr = buf;
    for (; cnt < max && ptr < next; ++cnt) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
    if (cnt == max) {
      if (ptr < next)
        std::memmove(buf, ptr, next - ptr);
      return next - (ptr - buf);
    }
    return nullptr;
  }

  t_bool is_equal_(t_n_ lh_segs, P_char lh_begin, P_char lh_end,
                   t_n_ rh_segs, P_char rh_begin, P_char rh_end) noexcept {
    if (lh_segs == rh_segs && lh_end - lh_begin == rh_end - rh_begin) {
      P_char lh = lh_begin, rh = rh_begin;
      while (lh < lh_end) {
        t_seg_hdr_ lh_hdr{lh[0], lh[1]}, rh_hdr{rh[0], rh[1]};
        t_n_ lh_len = lh_hdr.hdr.len, rh_len = rh_hdr.hdr.len;
        t_n_ min    = lh_len < rh_len ? lh_len : rh_len;
        if (lh_len != rh_len ||
            strncmp(lh + HDR_MAX_, rh + HDR_MAX_, min))
          return false;
        lh += HDR_MAX_ + lh_len;
        rh += HDR_MAX_ + rh_len;
      }
      return true;
    }
    return false;
  }

  t_bool is_less_(P_char lh_begin, P_char lh_end,
                  P_char rh_begin, P_char rh_end) noexcept {
    if (lh_end - lh_begin == rh_end - rh_begin) {
      P_char lh = lh_begin, rh = rh_begin;
      while (lh < lh_end) {
        t_seg_hdr_ lh_hdr{lh[0], lh[1]}, rh_hdr{rh[0], rh[1]};
        t_n_ lh_len = lh_hdr.hdr.len, rh_len = rh_hdr.hdr.len;
        t_n_ min    = lh_len < rh_len ? lh_len : rh_len;
        int value = strncmp(lh + HDR_MAX_, rh + HDR_MAX_, min);
        if (value < 0)       return true;
        if (value > 0)       return false;
        if (lh_len < rh_len) return true;
        if (lh_len > rh_len) return false;
        lh += HDR_MAX_ + lh_len;
        rh += HDR_MAX_ + rh_len;
      }
    }
    return false;
  }

  t_bool is_less_equal_(P_char lh_begin, P_char lh_end,
                        P_char rh_begin, P_char rh_end) noexcept {
    if (lh_end - lh_begin == rh_end - rh_begin) {
      P_char lh = lh_begin, rh = rh_begin;
      while (lh < lh_end) {
        t_seg_hdr_ lh_hdr{lh[0], lh[1]}, rh_hdr{rh[0], rh[1]};
        t_n_ lh_len = lh_hdr.hdr.len, rh_len = rh_hdr.hdr.len;
        t_n_ min    = lh_len < rh_len ? lh_len : rh_len;
        int value = strncmp(lh + HDR_MAX_, rh + HDR_MAX_, min);
        if (value < 0)       return true;
        if (value > 0)       return false;
        if (lh_len < rh_len) return true;
        if (lh_len > rh_len) return false;
        lh += HDR_MAX_ + lh_len;
        rh += HDR_MAX_ + rh_len;
      }
      return true;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_result t_segmented_impl_base_::push_back(p_char store,
                                             t_user user) noexcept {
    t_ix_ ix = next_ - store;
    next_    = push_back_(next_, user);
    return {t_id{ix + 1}, t_seg_no{segs_++}};
  }

  t_result t_segmented_impl_base_::push_back(p_char store, t_crange range,
                                             t_user user) noexcept {
    t_ix_ ix = next_ - store;
    next_ = push_back_(next_, range, user);
    return {t_id{ix + 1}, t_seg_no{segs_++}};
  }

  t_id t_segmented_impl_base_::insert(p_char store, t_seg_no seg_no,
                                      t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      next_ = insert_(info.ptr, next_, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  t_id t_segmented_impl_base_::insert(p_char store, t_seg_no seg_no,
                                      t_crange range, t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      next_ = insert_(info.ptr, next_, range, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  t_id t_segmented_impl_base_::insert(p_char store, t_id id,
                                      t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      next_ = insert_(info.ptr, next_, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  t_id t_segmented_impl_base_::insert(p_char store, t_id id, t_crange range,
                                      t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      next_ = insert_(info.ptr, next_, range, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  t_bool t_segmented_impl_base_::change(p_char store, t_seg_no seg_no,
                                        t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      next_ = change_(info.ptr, next_, user);
      return true;
    }
    return false;
  }

  t_bool t_segmented_impl_base_::change(p_char store, p_char end,
                                        t_seg_no seg_no,
                                        t_crange range, t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      t_n_ available = (end - next_) + info.hdr.hdr.len;
      if (available >= base::get(range.n))
        next_ = change_(info.ptr, next_, range, user);
      return true;
    }
    return false;
  }

  t_bool t_segmented_impl_base_::change(p_char store, t_id id,
                                        t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      next_ = change_(info.ptr, next_, user);
      return true;
    }
    return false;
  }

  t_bool t_segmented_impl_base_::change(p_char store, p_char end, t_id id,
                                        t_crange range, t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      t_n_ available = (end - next_) + info.hdr.hdr.len;
      if (available >= base::get(range.n))
        next_ = change_(info.ptr, next_, range, user);
      return true;
    }
    return false;
  }

  t_bool t_segmented_impl_base_::remove(p_char store, t_seg_no seg_no,
                                        t_n _n) noexcept {
    t_n_ n = base::get(_n);
    if (base::get(seg_no) + n <= segs_) {
      auto info = get_seg(store, seg_no);
      next_ = remove_(info.ptr, next_, _n);
      segs_ -= n;
      return true;
    }
    return false;
  }

  t_bool t_segmented_impl_base_::remove(p_char store, t_id id,
                                        t_n n) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      p_char next = remove_(info.ptr, next_, n);
      if (next) {
        next_ = next;
        segs_ -= base::get(n);
        return true;
      }
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}
