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




