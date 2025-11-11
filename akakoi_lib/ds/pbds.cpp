#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace __gnu_pbds;
// int order_of_key(x) 集合内でxより小さい要素の個数
// iterator find_by_order(k) k番目(0start)のイテレータ
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T, typename Hash = hash<T>>
using unordered_set_pbds = gp_hash_table<T, null_type, Hash>;
gp_hash_table<int, int> mp