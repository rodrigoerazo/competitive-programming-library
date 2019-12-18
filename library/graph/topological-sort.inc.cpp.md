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


# :warning: graph/topological-sort.inc.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#f8b0b924ebd7046dbfa85a856e4682c8">graph</a>
* <a href="{{ site.github.repository_url }}/blob/master/graph/topological-sort.inc.cpp">View this file on GitHub</a>
    - Last commit date: 2019-05-24 16:32:35 +0900




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
/**
 * @return a list of vertices which sorted topologically
 * @note the empty list is returned if cycles exist
 * @note O(V + E)
 */
vector<int> topological_sort(const vector<vector<int> > & g_rev) {
    int n = g_rev.size();
    vector<int> order;
    vector<char> used(n);
    function<bool (int)> go = [&](int i) {
        used[i] = 1;  // in stack
        for (int j : g_rev[i]) {
            if (used[j] == 1) return true;
            if (not used[j]) {
                if (go(j)) return true;
            }
        }
        used[i] = 2;  // completely used
        order.push_back(i);
        return false;
    };
    REP (i, n) if (not used[i]) {
        if (go(i)) return vector<int>();
    }
    return order;
}

/**
 * @note you can use std::reverse instead of this
 */
vector<vector<int> > opposite_graph(const vector<vector<int> > & g) {
    vector<vector<int> > g_rev(g.size());
    REP (i, g.size()) {
        for (int j : g[i]) {
            g_rev[j].push_back(i);
        }
    }
    return g_rev;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "graph/topological-sort.inc.cpp"
/**
 * @return a list of vertices which sorted topologically
 * @note the empty list is returned if cycles exist
 * @note O(V + E)
 */
vector<int> topological_sort(const vector<vector<int> > & g_rev) {
    int n = g_rev.size();
    vector<int> order;
    vector<char> used(n);
    function<bool (int)> go = [&](int i) {
        used[i] = 1;  // in stack
        for (int j : g_rev[i]) {
            if (used[j] == 1) return true;
            if (not used[j]) {
                if (go(j)) return true;
            }
        }
        used[i] = 2;  // completely used
        order.push_back(i);
        return false;
    };
    REP (i, n) if (not used[i]) {
        if (go(i)) return vector<int>();
    }
    return order;
}

/**
 * @note you can use std::reverse instead of this
 */
vector<vector<int> > opposite_graph(const vector<vector<int> > & g) {
    vector<vector<int> > g_rev(g.size());
    REP (i, g.size()) {
        for (int j : g[i]) {

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
