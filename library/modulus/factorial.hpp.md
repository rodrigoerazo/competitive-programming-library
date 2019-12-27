---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :heavy_check_mark: modulus/factorial.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#06efba23b1f3a9b846a25c6b49f30348">modulus</a>
* <a href="{{ site.github.repository_url }}/blob/master/modulus/factorial.hpp">View this file on GitHub</a>
    - Last commit date: 2019-12-26 23:56:34+09:00




## Depends on

* :heavy_check_mark: <a href="mint.hpp.html">modulus/mint.hpp</a>
* :heavy_check_mark: <a href="modinv.hpp.html">modulus/modinv.hpp</a>
* :heavy_check_mark: <a href="modpow.hpp.html">modulus/modpow.hpp</a>


## Required by

* :heavy_check_mark: <a href="bell_number.hpp.html">the Bell number <small>(modulus/bell_number.hpp)</small></a>
* :heavy_check_mark: <a href="choose.hpp.html">modulus/choose.hpp</a>
* :heavy_check_mark: <a href="stirling_number_of_the_second_kind.hpp.html">the Stirling number of the second kind <small>(modulus/stirling_number_of_the_second_kind.hpp)</small></a>
* :heavy_check_mark: <a href="twelvefold_way.hpp.html">twelvefold way / 写像12相 <small>(modulus/twelvefold_way.hpp)</small></a>


## Verified with

* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_1.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_1.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_11.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_11.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_2.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_2.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_3.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_3.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_4.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_4.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_5.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_5.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_6.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_6.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_7.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_7.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_8.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_8.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/modulus/twelvefold_way.balls_and_boxes_9.test.cpp.html">modulus/twelvefold_way.balls_and_boxes_9.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#pragma once
#include <vector>
#include "modulus/mint.hpp"

template <int32_t MOD>
mint<MOD> fact(int n) {
    static std::vector<mint<MOD> > memo(1, 1);
    while (n >= memo.size()) {
        memo.push_back(memo.back() * mint<MOD>(memo.size()));
    }
    return memo[n];
}
template <int32_t MOD>
mint<MOD> inv_fact(int n) {
    static std::vector<mint<MOD> > memo;
    if (memo.size() <= n) {
        int l = memo.size();
        int r = n * 1.3 + 100;
        memo.resize(r);
        memo[r - 1] = fact<MOD>(r - 1).inv();
        for (int i = r - 2; i >= l; -- i) {
            memo[i] = memo[i + 1] * (i + 1);
        }
    }
    return memo[n];
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 2 "modulus/factorial.hpp"
#include <vector>
#line 2 "modulus/mint.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#line 2 "modulus/modpow.hpp"
#include <cassert>

inline constexpr int32_t modpow(uint_fast64_t x, uint64_t k, int32_t MOD) {
    assert (0 <= x and x < MOD);
    uint_fast64_t y = 1;
    for (; k; k >>= 1) {
        if (k & 1) (y *= x) %= MOD;
        (x *= x) %= MOD;
    }
    assert (0 <= y and y < MOD);
    return y;
}
#line 2 "modulus/modinv.hpp"
#include <algorithm>
#include <cassert>

inline int32_t modinv(int32_t value, int32_t MOD) {
    assert (0 <= value and value < MOD);
    assert (value != 0);
    int64_t a = value, b = MOD;
    int64_t x = 0, y = 1;
    for (int64_t u = 1, v = 0; a; ) {
        int64_t q = b / a;
        x -= q * u; std::swap(x, u);
        y -= q * v; std::swap(y, v);
        b -= q * a; std::swap(b, a);
    }
    assert (value * x + MOD * y == b and b == 1);
    if (x < 0) x += MOD;
    assert (0 <= x and x < MOD);
    return x;
}
#line 7 "modulus/mint.hpp"


template <int32_t MOD>
struct mint {
    int32_t value;
    mint() : value() {}
    mint(int64_t value_) : value(value_ < 0 ? value_ % MOD + MOD : value_ >= MOD ? value_ % MOD : value_) {}
    mint(int32_t value_, std::nullptr_t) : value(value_) {}
    explicit operator bool() const { return value; }
    inline constexpr mint<MOD> operator + (mint<MOD> other) const { return mint<MOD>(*this) += other; }
    inline constexpr mint<MOD> operator - (mint<MOD> other) const { return mint<MOD>(*this) -= other; }
    inline constexpr mint<MOD> operator * (mint<MOD> other) const { return mint<MOD>(*this) *= other; }
    inline constexpr mint<MOD> & operator += (mint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
    inline constexpr mint<MOD> & operator -= (mint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
    inline constexpr mint<MOD> & operator *= (mint<MOD> other) { this->value = (uint_fast64_t)this->value * other.value % MOD; return *this; }
    inline constexpr mint<MOD> operator - () const { return mint<MOD>(this->value ? MOD - this->value : 0, nullptr); }
    inline constexpr mint<MOD> pow(uint64_t k) const { return mint<MOD>(modpow(value, k, MOD), nullptr); }
    inline mint<MOD> inv() const { return mint<MOD>(modinv(value, MOD), nullptr); }
    inline constexpr mint<MOD> operator /  (mint<MOD> other) const { return *this *  other.inv(); }
    inline constexpr mint<MOD> operator /= (mint<MOD> other)       { return *this *= other.inv(); }
    inline constexpr bool operator == (mint<MOD> other) const { return value == other.value; }
    inline constexpr bool operator != (mint<MOD> other) const { return value != other.value; }
};
template <int32_t MOD> mint<MOD> operator * (int64_t value, mint<MOD> n) { return mint<MOD>(value) * n; }
template <int32_t MOD> std::istream & operator >> (std::istream & in, mint<MOD> & n) { int64_t value; in >> value; n = value; return in; }
template <int32_t MOD> std::ostream & operator << (std::ostream & out, mint<MOD> n) { return out << n.value; }
#line 4 "modulus/factorial.hpp"

template <int32_t MOD>
mint<MOD> fact(int n) {
    static std::vector<mint<MOD> > memo(1, 1);
    while (n >= memo.size()) {
        memo.push_back(memo.back() * mint<MOD>(memo.size()));
    }
    return memo[n];
}
template <int32_t MOD>
mint<MOD> inv_fact(int n) {
    static std::vector<mint<MOD> > memo;
    if (memo.size() <= n) {
        int l = memo.size();
        int r = n * 1.3 + 100;
        memo.resize(r);
        memo[r - 1] = fact<MOD>(r - 1).inv();
        for (int i = r - 2; i >= l; -- i) {
            memo[i] = memo[i + 1] * (i + 1);
        }
    }
    return memo[n];
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
