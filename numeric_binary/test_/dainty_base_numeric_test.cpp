#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "dainty_base_numeric.h"

using namespace dainty::base::numeric;

static_assert(sizeof(impl_::t_pvalue_) == 1);

TEST_CASE("numeric::impl_::calc_size_", "impl_") {
  REQUIRE(impl_::calc_size_(t_bits{0})  == impl_::t_n{0});
  REQUIRE(impl_::calc_size_(t_bits{1})  == impl_::t_n{1});
  REQUIRE(impl_::calc_size_(t_bits{7})  == impl_::t_n{1});
  REQUIRE(impl_::calc_size_(t_bits{8})  == impl_::t_n{2});
  REQUIRE(impl_::calc_size_(t_bits{14}) == impl_::t_n{2});
  REQUIRE(impl_::calc_size_(t_bits{15}) == impl_::t_n{3});
  REQUIRE(impl_::calc_size_(t_bits{33}) == impl_::t_n{5});
}

TEST_CASE("numeric::impl_::calc_bits_", "impl_") {
}

TEST_CASE("numeric::impl_::digits_bits_", "impl_") {
}

/*
// test t_bpos
  t_bool operator==(R_bpos_ lh, R_bpos_ rh) noexcept {
  t_bool operator<(R_bpos_ lh, R_bpos_ rh) noexcept {
  t_bool operator<=(R_bpos_ lh, R_bpos_ rh) noexcept {
  t_bool operator!=(R_bpos_ lh, R_bpos_ rh) noexcept {
  t_bool operator>(R_bpos_ lh, R_bpos_ rh) noexcept {
  t_bool operator>=(R_bpos_ lh, R_bpos_ rh) noexcept {

// test t_info_
  t_n calc_size_(t_n bits) noexcept {
  t_n bits_(R_bpos_ pos) noexcept {
  t_ix last_ix_(R_store_ store) noexcept {
  r_pvalue_ last_(r_store_ store) noexcept {
  R_pvalue_ last_(R_store_ store) noexcept {
  t_bool lsb_(R_store_ store) noexcept {
  t_bool msb_(R_store_ store) noexcept {
  t_bool is_neg_(R_store_ store) noexcept {
  t_n max_bits_(R_store_ value) noexcept {
  t_bool abs_(r_store_ value) noexcept {

  t_void assign_(r_store_ store, t_pvalue_ value) noexcept {
  t_void assign_(r_store_ store, t_nvalue_ value) noexcept {

  t_void   display_bits_  (R_store_, t_bool)        noexcept; // IMPL_FUNC_1_1_
  t_info_  bits_info_     (R_store_)                noexcept; // IMPL_FUNC_1_2_
  t_n      free_bits_     (R_store_)                noexcept; // IMPL_FUNC_1_3_
  t_n      on_bits_       (R_store_)                noexcept; // IMPL_FUNC_1_4_
  t_bpos_  lsb_on_bit_    (R_store_)                noexcept; // IMPL_FUNC_1_5_
  t_bpos_  msb_on_bit_    (R_store_)                noexcept; // IMPL_FUNC_1_6_
  t_bpos_  msb_off_bit_   (R_store_)                noexcept; // IMPL_FUNC_1_7_
  t_void   ones_compl_    (r_store_)                noexcept; // IMPL_FUNC_1_8_
  t_void   twos_compl_    (r_store_)                noexcept; // IMPL_FUNC_1_9_
  t_bool   assign_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_10_
  t_bool   add_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_11_
  t_bool   minus_         (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_12_
  t_bool   multiply_      (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_13_
  t_bool   divide_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_14_
  t_bool   and_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_15_
  t_bool   or_            (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_16_
  t_bool   xor_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_17_
  t_bool   shift_left_    (r_store_, t_n = 1_n)     noexcept; // IMPL_FUNC_1_18_
  t_bool   shift_right_   (r_store_, t_n = 1_n)     noexcept; // IMPL_FUNC_1_19_
  t_n      shift_next_    (r_store_)                noexcept; // IMPL_FUNC_1_20_
  t_bool   set_bit_       (r_store_, t_ix, t_bool)  noexcept; // IMPL_FUNC_1_21_
  t_bool   get_bit_       (R_store_, t_ix)          noexcept; // IMPL_FUNC_1_22_
  t_bool   is_zero_       (R_store_)                noexcept; // IMPL_FUNC_1_23_
  t_bool   reset_         (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_24_
  t_bool   reset_         (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_25_
  t_bool   is_equal_      (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_26_
  t_bool   is_less_       (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_27_
  t_bool   is_less_equal_ (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_28_
  t_n      calc_bits_     (t_n)                     noexcept; // IMPL_FUNC_1_29_
  t_n      calc_digits_   (t_n)                     noexcept; // IMPL_FUNC_1_30_
  t_bool   ensure_        (r_store_, t_n)           noexcept; // IMPL_FUNC_1_31_
  t_bool   ensure_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_32_
  t_bool   ensure_        (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_33_
*/
