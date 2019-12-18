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


# :warning: number/lagrange_interpolation.random-test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#b1bc248a7ff2b2e95569f56de68615df">number</a>
* <a href="{{ site.github.repository_url }}/blob/master/number/lagrange_interpolation.random-test.cpp">View this file on GitHub</a>
    - Last commit date: 2019-12-13 18:43:42 +0900




## Depends on

* :warning: <a href="lagrange_interpolation.hpp.html">Lagrange interpolation <small>(number/lagrange_interpolation.hpp)</small></a>
* :heavy_check_mark: <a href="../utils/macros.hpp.html">utils/macros.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "number/lagrange_interpolation.hpp"

#include <cassert>
#include <random>
#include <set>
#include <tuple>
#include <utility>
#include <iostream>
using namespace std;

long double apply(std::vector<long double> & f, long double x) {
    long double y = 0;
    REP_R (i, f.size()) {
        y = y * x + f[i];
    }
    return y;
}

int main() {
    default_random_engine gen;
    REP (iteration, 20) {
        // generate
        vector<pair<long double, long double> > points;
        set<int> used;
        REP (i, iteration) {
            int z_i = uniform_int_distribution<int>(- 100, 100)(gen);
            if (used.count(z_i)) continue;
            used.insert(z_i);
            long double x_i = z_i / 10.0L;
            long double y_i = uniform_int_distribution<int>(- 100, 100)(gen) / 10.0L;
            points.emplace_back(x_i, y_i);
        }

        // run
        auto f = lagrange_interpolate(points);

        // check
        constexpr long double eps = 1e-4;
        assert (f.size() == points.size());
        for (auto point : points) {
            long double x, y; tie(x, y) = point;
            assert (abs(apply(f, x) - y) < eps);
        }
    }
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "number/lagrange_interpolation.hpp"
#include <cassert>
#include <utility>
#include <vector>
#line 1 "utils/macros.hpp"
#define REP(i, n) for (int i = 0; (i) < (int)(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < (int)(n); ++ (i))
#define REP_R(i, n) for (int i = (int)(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = (int)(n) - 1; (i) >= (int)(m); -- (i))
#define ALL(x) begin(x), end(x)
#line 5 "number/lagrange_interpolation.hpp"

/**
 * @brief Lagrange interpolation
 * @note O(n^2)
 * @note works on any field
 * @note the error is very big
 */
std::vector<long double> lagrange_interpolate(const std::vector<std::pair<long double, long double> > & points) {
    // given ((x_0, y_1), \dots, (x_{n - 1}, y_{n - 1}))
    int n = points.size();

    // functions
    auto mul_monomial = [&](std::vector<long double> & f, long double a) {  // f \gets f \cdot (x - a)
        assert (f[n] == 0);
        REP_R (i, n + 1) {
            f[i] = (i >= 1 ? f[i - 1] : 0) - a * f[i];
        }
    };
    auto div_monomial = [&](std::vector<long double> & f, long double a) {  // f \gets f / (x - a)
        long double last = 0;
        REP_R (i, n + 1) {
            std::swap(f[i], last);
            last += a * f[i];
        }
    };
    auto apply = [&](std::vector<long double> & f, long double x) {  // f(x)
        long double y = 0;
        REP_R (i, n + 1) {
            y = y * x + f[i];
        }
        return y;
    };

    // let g = \prod_i (x - x_i)
    // let g_i = g / (x - x_i)
    // assert for all j \ne i. g_i(x_j) = 0
    std::vector<long double> g(n + 1);
    g[0] = 1;
    for (auto point : points) {
        mul_monomial(g, point.first);
    }

    // let c_i = y_i / g_i(x_i)
    // let f = \sum_i c_i g_i / (x - x_i)
    // assert for all i. f(x_i) = y_i
    std::vector<long double> f(n + 1);
    for (auto point : points) {
        div_monomial(g, point.first);
        long double c_i = point.second / apply(g, point.first);
        REP (j, n + 1) {
            f[j] += c_i * g[j];
        }
        mul_monomial(g, point.first);
    }
    assert (f.back() == 0);
    f.pop_back();
    return f;
}
#line 2 "number/lagrange_interpolation.random-test.cpp"

#include <cassert>
#include <random>
#include <set>
#include <tuple>
#include <utility>
#include <iostream>
using namespace std;

long double apply(std::vector<long double> & f, long double x) {
    long double y = 0;
    REP_R (i, f.size()) {
        y = y * x + f[i];
    }
    return y;
}

int main() {
    default_random_engine gen;
    REP (iteration, 20) {
        // generate
        vector<pair<long double, long double> > points;
        set<int> used;
        REP (i, iteration) {
            int z_i = uniform_int_distribution<int>(- 100, 100)(gen);
            if (used.count(z_i)) continue;
            used.insert(z_i);
            long double x_i = z_i / 10.0L;
            long double y_i = uniform_int_distribution<int>(- 100, 100)(gen) / 10.0L;
            points.emplace_back(x_i, y_i);
        }

        // run
        auto f = lagrange_interpolate(points);

        // check
        constexpr long double eps = 1e-4;
        assert (f.size() == points.size());
        for (auto point : points) {
            long double x, y; tie(x, y) = point;
            assert (abs(apply(f, x) - y) < eps);
        }
    }
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
