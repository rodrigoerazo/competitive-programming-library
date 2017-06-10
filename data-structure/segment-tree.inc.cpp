template <class Monoid>
struct segment_tree {
    typedef Monoid monoid_type;
    typedef typename Monoid::type underlying_type;
    int n;
    vector<underlying_type> a;
    Monoid mon;
    segment_tree() = default;
    segment_tree(int a_n, underlying_type initial_value = Monoid().unit(), Monoid const & a_mon = Monoid()) : mon(a_mon) {
        n = 1; while (n < a_n) n *= 2;
        a.resize(2*n-1, mon.unit());
        fill(a.begin() + (n-1), a.begin() + (n-1 + a_n), initial_value); // set initial values
        repeat_reverse (i, n-1) a[i] = mon.append(a[2*i+1], a[2*i+2]); // propagate initial values
    }
    void point_set(int i, underlying_type z) { // 0-based
        a[i+n-1] = z;
        for (i = (i+n)/2; i > 0; i /= 2) { // 1-based
            a[i-1] = mon.append(a[2*i-1], a[2*i]);
        }
    }
    underlying_type range_concat(int l, int r) { // 0-based, [l, r)
        underlying_type lacc = mon.unit(), racc = mon.unit();
        for (l += n, r += n; l < r; l /= 2, r /= 2) { // 1-based loop, 2x faster than recursion
            if (l % 2 == 1) lacc = mon.append(lacc, a[(l ++) - 1]);
            if (r % 2 == 1) racc = mon.append(a[(-- r) - 1], racc);
        }
        return mon.append(lacc, racc);
    }
};

template <class Monoid, class MagmaEndomorphism>
struct lazy_propagation_segment_tree { // on monoids
    static_assert (is_same<typename Monoid::type, typename MagmaEndomorphism::underlying_type>::value, "");
    typedef Monoid monoid_type;
    typedef MagmaEndomorphism endomorphism_type;
    typedef typename Monoid::type T;
    typedef typename MagmaEndomorphism::type F;
    Monoid mon;
    MagmaEndomorphism endo;
    int n;
    vector<T> a;
    vector<F> f;
    lazy_propagation_segment_tree() = default;
    lazy_propagation_segment_tree(int a_n, T initial_value = Monoid().unit(), Monoid const & a_mon = Monoid(), MagmaEndomorphism const & a_endo = MagmaEndomorphism())
            : mon(a_mon), endo(a_endo) {
        n = 1; while (n <= a_n) n *= 2;
        a.resize(2*n-1, mon.unit());
        fill(a.begin() + (n-1), a.begin() + (n-1 + a_n), initial_value); // set initial values
        repeat_reverse (i, n-1) a[i] = mon.append(a[2*i+1], a[2*i+2]); // propagate initial values
        f.resize(max(0, 2*n-1-n), endo.identity());
    }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        repeat (i, n) {
            a[i+n-1] = first != last ? *(first ++) : mon.unit();
        }
        assert (first == last);
        repeat_reverse (i, n-1) {
            a[i] = mon.append(a[2*i+1], a[2*i+2]);
        }
        f.assign(f.size(), endo.identity());
    }
    void point_set(int i, T z) {
        assert (0 <= i and i < n);
        point_set(0, 0, n, i, z);
    }
    void point_set(int i, int il, int ir, int j, T z) {
        if (i == n+j-1) { // 0-based
            a[i] = z;
        } else if (ir <= j or j+1 <= il) {
            // nop
        } else {
            range_apply(2*i+1, il, (il+ir)/2, 0, n, f[i]);
            range_apply(2*i+2, (il+ir)/2, ir, 0, n, f[i]);
            f[i] = endo.identity();
            point_set(2*i+1, il, (il+ir)/2, j, z);
            point_set(2*i+2, (il+ir)/2, ir, j, z);
            a[i] = mon.append(a[2*i+1], a[2*i+2]);
        }
    }
    void range_apply(int l, int r, F z) {
        assert (0 <= l and l <= r and r <= n);
        range_apply(0, 0, n, l, r, z);
    }
    void range_apply(int i, int il, int ir, int l, int r, F z) {
        if (l <= il and ir <= r) { // 0-based
            a[i] = endo.apply(z, a[i]);
            if (i < f.size()) f[i] = endo.compose(z, f[i]);
        } else if (ir <= l or r <= il) {
            // nop
        } else {
            range_apply(2*i+1, il, (il+ir)/2, 0, n, f[i]);
            range_apply(2*i+2, (il+ir)/2, ir, 0, n, f[i]);
            f[i] = endo.identity();
            range_apply(2*i+1, il, (il+ir)/2, l, r, z);
            range_apply(2*i+2, (il+ir)/2, ir, l, r, z);
            a[i] = mon.append(a[2*i+1], a[2*i+2]);
        }
    }
    T range_concat(int l, int r) {
        assert (0 <= l and l <= r and r <= n);
        return range_concat(0, 0, n, l, r);
    }
    T range_concat(int i, int il, int ir, int l, int r) {
        if (l <= il and ir <= r) { // 0-based
            return a[i];
        } else if (ir <= l or r <= il) {
            return mon.unit();
        } else {
            return endo.apply(f[i], mon.append(
                    range_concat(2*i+1, il, (il+ir)/2, l, r),
                    range_concat(2*i+2, (il+ir)/2, ir, l, r)));
        }
    }
};

// http://arc054.contest.atcoder.jp/submissions/1335245
template <class Monoid>
struct dynamic_segment_tree { // on monoid
    typedef Monoid monoid_type;
    typedef typename Monoid::type underlying_type;
    struct node_t {
        int left, right; // indices on pool
        underlying_type value;
    };
    deque<node_t> pool;
    int root; // index
    int width; // of the tree
    int size; // the number of leaves
    Monoid mon;
    dynamic_segment_tree(Monoid const & a_mon = Monoid()) : mon(a_mon) {
        node_t node = { -1, -1, mon.unit() };
        pool.push_back(node);
        root = 0;
        width = 1;
        size = 1;
    }
protected:
    int create_node(int parent, bool is_right) {
        // make a new node
        int i = pool.size();
        node_t node = { -1, -1, mon.unit() };
        pool.push_back(node);
        // link from the parent
        assert (parent != -1);
        int & ptr = is_right ? pool[parent].right : pool[parent].left;
        assert (ptr == -1);
        ptr = i;
        return i;
    }
    int get_value(int i) {
        return i == -1 ? mon.unit() : pool[i].value;
    }
public:
    void point_set(int i, underlying_type z) {
        assert (0 <= i);
        while (width <= i) {
            node_t node = { root, -1, pool[root].value };
            root = pool.size();
            pool.push_back(node);
            width *= 2;
        }
        point_set(root, -1, false, 0, width, i, z);
    }
    void point_set(int i, int parent, bool is_right, int il, int ir, int j, underlying_type z) {
        if (il == j and ir == j+1) { // 0-based
            if (i == -1) {
                i = create_node(parent, is_right);
                size += 1;
            }
            pool[i].value = z;
        } else if (ir <= j or j+1 <= il) {
            // nop
        } else {
            if (i == -1) i = create_node(parent, is_right);
            point_set(pool[i].left,  i, false, il, (il+ir)/2, j, z);
            point_set(pool[i].right, i, true,  (il+ir)/2, ir, j, z);
            pool[i].value = mon.append(get_value(pool[i].left), get_value(pool[i].right));
        }
    }
    underlying_type range_concat(int l, int r) {
        assert (0 <= l and l <= r);
        if (width <= l) return mon.unit();
        return range_concat(root, 0, width, l, min(width, r));
    }
    underlying_type range_concat(int i, int il, int ir, int l, int r) {
        if (i == -1) return mon.unit();
        if (l <= il and ir <= r) { // 0-based
            return pool[i].value;
        } else if (ir <= l or r <= il) {
            return mon.unit();
        } else {
            return mon.append(
                    range_concat(pool[i].left,  il, (il+ir)/2, l, r),
                    range_concat(pool[i].right, (il+ir)/2, ir, l, r));
        }
    }
    template <class Func>
    void traverse_leaves(Func func) {
        return traverse_leaves(root, 0, width, func);
    }
    template <class Func>
    void traverse_leaves(int i, int il, int ir, Func func) {
        if (i == -1) return;
        if (ir - il == 1) {
            func(il, pool[i].value);
        } else {
            traverse_leaves(pool[i].left,  il, (il+ir)/2, func);
            traverse_leaves(pool[i].right, (il+ir)/2, ir, func);
        }
    }
};
