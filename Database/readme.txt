bptree.hpp:
1. include data structure 'B+ Tree';
2. easily complete 'iterator' and 'const_iterator';
3. for iterator:
    One iterator represents one data stored in the 'B+ Tree'.
    Using the operator '*' to get the value and the function 'get_key()' to get the key.
    You can use the operator '++' or '--' to traverse the data list(either prefix or suffix form, similar to integer).
    You can also use the operator '==' or '!=' to check whether 2 iterator are equal.
4. operations below:
   (1) initialize: sjtu::bptree<Key, value_t, K, Compare>
        (a) 'Key' is the key type;
        (b) 'value_t' is the value type;
        (c) 'K' is a integer, meaning the base(usually 4096)
        (d) 'Compare' is the function for comparing 2 different keys, default 'std::less<Key>'
   (2) void insert(Key k, value_t v) : to insert an element whose key is 'k' and value is 'v'.
   (3) void erase(Key k): to erase the element whose key is 'k'. Make sure that the element exists.
   (4) int count(Key k): to check whether the element whose key is 'k' has been inserted.
        '1' is yes, '0' is no.
   (5) bool modify(Key k, value_t v): similar to insert. Change the value of the element whose key is k.
        '1' is changed, '0' is no.
   (6) (const) value_t at(Key k): to return the value included in the key 'k'.
   (7) iterator lower_bound(Key k): to find the element whose key is not less than 'k'.
   (8) (const_)iterator begin(): to return the first element.
   (9) (const_)iterator end(): to return the lastest element(an null element)
   (10) bool empty(): to check whether the tree is empty.
          '1' is yes, '0' is no.

memory_pool.hpp:
This 'hpp' file is for the memory pool. I suggest that you should use this memory pool to store your data.
1. need a boolean variable 'cleaned' to tell whether the whole system has been cleaned.(!!! very important)
2. functions below:
    (1) buffer_read(void *v, size_t p, size_t, s, char *f): to read the value in file whose name is 'f', from position 'p', the length readed being 's', stored in 'v'.
    (2) buffer_write(void *v, size_t p, size_t s, char *f): to write the value to file whose name is 'f', from position 'p', the length written being 's', the value stored in 'v'.