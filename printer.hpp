#include <type_traits>

/**
 * @brief Metatemplate function to check is type the container
 * 
 * @tparam T - input type
 */
template<typename T>
struct is_stl_container_like
{
    typedef typename std::remove_const<T>::type test_type;

    template<typename A>
    static constexpr bool test(A* pt,  
        decltype(pt->begin()) * = nullptr,
        decltype(pt->end()) * = nullptr,
        typename A::iterator * pi = nullptr,
        typename A::value_type * pv = nullptr)
    {
        typedef typename A::iterator iterator;
        return std::is_same<decltype(pt->begin()), iterator>::value &&
                std::is_same<decltype(pt->end()),iterator>::value;
    }

    template<typename A>
    static constexpr bool test(...)
    {
        return false;
    }

    static const bool value = test<test_type>(nullptr);
};

template<typename T>
constexpr bool is_stl_container_like_v = is_stl_container_like<T>::value;

template<bool B, typename T>
struct enable_if
{
    using type = T;
};

template<typename T>
struct enable_if<false,T>{};

template<bool B, typename T>
using enable_if_t = typename enable_if<B,T>::type;

template<typename T>
enable_if_t<is_stl_container_like_v<T>,bool> print_ip(const T& val)
{
    auto iter = val.begin();
    auto iter_end = val.end();
    std::cout << *(iter++);
    for(;iter != iter_end;++iter)
    {
        std::cout << '.' << *iter; 
    }
    std::cout << std::endl;

    return true;
}

template<typename T>
enable_if_t<std::is_integral_v<T>,bool> print_ip(T&& val)
{
    int sz = sizeof(T); //Size of type bytes
    std::cout << ((val >> 8*(--sz)) & 0xFF); //print first byte
    for(;--sz >= 0; )
    {
        std::cout << '.'  << ((val >> 8*sz) & 0xFF); //print all bytes
    }
    std::cout << std::endl;
    return true;
}

bool print_ip(const std::string& val)
{
    std::cout << val << std::endl;
    return true;
}






