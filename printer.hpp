#include <type_traits>
#include <tuple>
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
        [[maybe_unused]] typename A::iterator * pi = nullptr,
        [[maybe_unused]] typename A::value_type * pv = nullptr)
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

/**
 * @brief enable_if<expression, type>::type 
 *          get the type if expression true 
 *          else nothing(used in SFINAE)
 * 
 * @tparam B - exprission
 * @tparam T - returned type in ::type
 */
template<bool B, typename T>
struct enable_if
{
    using type = T;
};

template<typename T>
struct enable_if<false,T>{};

template<bool B, typename T>
using enable_if_t = typename enable_if<B,T>::type;

/**
 * @brief Instantiate function print_ip for container like types 
 *      vector, list (have begin() and end() iterators) 
 * 
 * @tparam T - container type
 * @param val - container ref
 * @return enable_if_t<is_stl_container_like_v<T>,bool> 
 */
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

/**
 * @brief Instantiate print_ip function for integral types(char short int uint16 etc)
 * 
 * @tparam T - integral type
 * @param val - value reference
 * @return enable_if_t<std::is_integral_v<T>,bool> 
 */
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

/**
 * @brief Specialization for string input
 * 
 * @param val - std::string ref
 * @return true - ok
 * @return false - not posible
 */
bool print_ip(const std::string& val)
{
    std::cout << val << std::endl;
    return true;
}

/**
 * @brief Special case print_ip helper for tuple
 *        unpack tuple in vector and call print_ip
 * 
 * @tparam T - tuple
 * @tparam N - list count of elements 0,1..... to get<N>
 * @param val 
 */
template<typename T, size_t... N>
void print_tuple(T&& val)
{
    std::vector<std::decay_t<decltype(std::get<0>(std::forward<T>(val)))>> res = {(std::get<N>(std::forward<T>(val)))...};
    print_ip(std::move(res));
};

/**
 * @brief Implement sequence 0...N where N is cout of tuple
 *         helper to call std::get<N> for tuple
 * 
 * @tparam T - tuple
 * @tparam enough - expression(special case used this to stop)
 * @tparam TotalArgs - sizeof tuple
 * @tparam N - list of count(if std::tuple_size<T>::value == 2, then N == 0,1)
 */
template<typename T,bool enough, int TotalArgs, size_t... N>
struct print_imp
{
    auto static call(T&& val)
    {
           return print_imp<T, TotalArgs == 1 + sizeof...(N),
                      TotalArgs, N..., sizeof...(N)>::call(std::forward<T>(val));
    }
};

/**
 * @brief Ended case for print_imp
 * 
 * @tparam T - tuple
 * @tparam TotalArgs - sizeof tuple
 * @tparam N - Sequence of tuple count to use it in get<N>
 */
template<typename T, int TotalArgs, size_t... N>
struct print_imp<T,true, TotalArgs, N...>
{
    auto static call(T&& val)
    {
        return print_tuple<T, N...>(std::forward<T>(val));
    }
};


/**
 * @brief Implementation for tuple type
 * 
 * @tparam T - tuple
 * @param val - tuple ref
 * @return enable_if_t<std::__is_tuple_like<T>::value,bool> 
 */
template<typename T>
enable_if_t<std::__is_tuple_like<T>::value,bool> print_ip(T&& val)
{
    print_imp<T, 0 == std::tuple_size<T>::value,
                      std::tuple_size<T>::value>::call(std::forward<T>(val));
    return true;
}






