#ifndef _FP_UTILS_881c3d51de77412a9d3eddfd42056f52
#define _FP_UTILS_881c3d51de77412a9d3eddfd42056f52

#include <functional>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <iterator>
#include <cassert>
#include <vector>
#include <array>
#include <iostream>
#include <string>

#include <cpp_env.hxx>

#if (CPP_STANDARD >= 201103L) && (CPP_STANDARD <=201402L)

#include <optional.hpp>
//实现std::invoke
//可以支持普通函数，成员函数，访问数据成员，lambda
namespace std {
// ========================================================================
// 第三步
// 调用普通函数的版本
struct _InvokeFunction {
    template<typename _Callable, typename... _Types>
    static auto _Call(_Callable&& obj, _Types&&... argv) {
        return obj(std::forward<_Types>(argv)...);
    }
};
// 调用成员函数版本
struct _InvokeMemFunc {
    template<typename _Callable, typename _Obj, typename... _Types>
    static auto _Call(_Callable&& fn, _Obj&& obj, _Types&&... argv) ->
    decltype((obj->*fn)(std::forward<_Types>(argv)...)) {
        return (obj->*fn)(std::forward<_Types>(argv)...);
    }
    // 这里和stl当中方法不一样，这里采用使用SFINAE技术
    // 编译器会自动选择两者当中可调用的版本
    template<typename _Callable, typename _Obj, typename... _Types>
    static auto _Call(_Callable&& fn, _Obj&& obj, _Types&&... argv) ->
    decltype((obj.*fn)(std::forward<_Types>(argv)...)) {
        return (obj.*fn)(std::forward<_Types>(argv)...);
    }
};
// 调用成员变量
struct _InvokeMemObj {
    template<typename _Callable, typename _Obj>
    static auto _Call(_Callable&& fn, _Obj&& obj) ->
    decltype((obj->*fn)) {
        return (obj->*fn);
    }
    template<typename _Callable, typename _Obj>
    static auto _Call(_Callable&& fn, _Obj&& obj) ->
    decltype((obj.*fn)) {
        return (obj.*fn);
    }
};
// =========================================================================
// 第二步
// 第二层，筛选多参数普通函数，成员函数，数据成员
// 暂时依赖标准库的萃取技术
template<typename _Callable,
         typename _FirstTy,
         typename _Decayed = typename std::decay<_Callable>::type,
         bool _Is_MemFun = std::is_member_function_pointer<_Decayed>::value,
         bool _Is_MemObj = std::is_member_object_pointer<_Decayed>::value>
struct _Invoke1;
// 成员函数，标准库当中传递
// _FirstTy的作用是用来判断 _Callable的Class是否是_FirstTy的Class或者Parent Class
// 这里为了简化不再判断
template<typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, true, false> :
    _InvokeMemFunc {
};
// 成员变量
template<typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, false, true> :
    _InvokeMemObj {
};
// 普通函数
template<typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, false, false> :
    _InvokeFunction {
};
// =========================================================================
// 第一步
// 本层先把无参数的直接筛选出来了
template<typename _Callable, typename... _Types>
struct _Invoke;
// 无参数，必定是一个普通函数
template<typename _Callable>
struct _Invoke<_Callable> :
    _InvokeFunction {
};
// 有一个或多个参数，可能是普通函数，成员函数，数据成员
template<typename _Callable, typename _FirstTy, typename... _Types>
struct _Invoke<_Callable, _FirstTy, _Types...> :
    _Invoke1<_Callable, _FirstTy> {
};

#if !defined(_MSC_VER)
// 通过Invoke函数进行一层封装，使其使用更加贴合实际
template<typename _Callable, typename... _Types>
auto invoke(_Callable&& obj, _Types&&... argv) {
    return _Invoke<_Callable, _Types...>::_Call(
               std::forward<_Callable>(obj),
               std::forward<_Types>(argv)...);
}

//实现std::apply
template <class Func, class Tuple, std::size_t... I>
constexpr auto tuple_apply_impl(Func && f, Tuple && t,
                                std::index_sequence<I...>)
{
    return std::invoke(std::forward<Func>(f),
                       std::get<I>(std::forward<Tuple>(t))...);
}
#else
// 通过Invoke函数进行一层封装，使其使用更加贴合实际
template<typename _Callable, typename... _Types>
auto __invoke(_Callable&& obj, _Types&&... argv) {
    return _Invoke<_Callable, _Types...>::_Call(
               std::forward<_Callable>(obj),
               std::forward<_Types>(argv)...);
}

//实现std::apply
template <class Func, class Tuple, std::size_t... I>
constexpr auto tuple_apply_impl(Func && f, Tuple && t,
                                std::index_sequence<I...>)
{
    return std::__invoke(std::forward<Func>(f),
                         std::get<I>(std::forward<Tuple>(t))...);
}
#endif

template <class Func, class Tuple>
constexpr auto apply(Func && f, Tuple && t)
{
    return tuple_apply_impl(std::forward<Func>(f),
                            std::forward<Tuple>(t),
                            std::make_index_sequence<std::tuple_size<std::
                            remove_reference_t<Tuple>>::value> {});
}
} // namespace std.
#else
#include <optional>
#endif // if (CPP_STANDARD >= 201103L) && (CPP_STANDARD <=201402L)

namespace FP_Utils {
template <class T,
          class charT = char,
          class traits = std::char_traits<charT> >
class infix_ostream_iterator :
    public std::iterator<std::output_iterator_tag, void, void, void, void>
{
    std::basic_ostream<charT, traits> *os;
    charT const* delimiter;
    bool first_elem;
public:
    typedef charT char_type;
    typedef traits traits_type;
    typedef std::basic_ostream<charT, traits> ostream_type;
    infix_ostream_iterator(ostream_type& s)
        : os(&s), delimiter(0), first_elem(true)
    {}
    infix_ostream_iterator(ostream_type& s, charT const *d)
        : os(&s), delimiter(d), first_elem(true)
    {}
    infix_ostream_iterator<T, charT, traits>& operator=(T const &item)
    {
        // Here's the only real change from ostream_iterator:
        // Normally, the '*os << item;' would come before the 'if'.
        if (!first_elem && delimiter != 0)
            *os << delimiter;
        *os << item;
        first_elem = false;
        return *this;
    }
    infix_ostream_iterator<T, charT, traits> &operator*() {
        return *this;
    }
    infix_ostream_iterator<T, charT, traits> &operator++() {
        return *this;
    }
    infix_ostream_iterator<T, charT, traits> &operator++(int) {
        return *this;
    }
};

template <size_t I, typename T>
struct tuple_n {
    template< typename...Args>
    using type = typename tuple_n<I - 1, T>::template type<T, Args...>;
};

template <typename T>
struct tuple_n<0, T> {
    template<typename...Args> using type = std::tuple<Args...>;
};

template <size_t I, typename T>
using tuple_of = typename tuple_n<I, T>::template type<>;

template<typename T>
struct is_random_container
{
private:
    template<typename U>
    static auto Check(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::declval<U>().operator[](0),
        std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    constexpr static bool value = std::is_same<decltype(Check<T>(0)),
                          std::true_type>::value;
};

template<typename T>
struct is_not_random_container
{
private:
    template<typename U>
    static auto Check(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::declval<U>().operator[](0),
        std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    constexpr static bool value = std::is_same<decltype(Check<T>(0)),
                          std::false_type>::value;
};

template<typename T>
struct has_size_member_container
{
private:
    template<typename U>
    static auto Check(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::declval<U>().size(),
        std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    constexpr static bool value = std::is_same<decltype(Check<T>(0)),
                          std::false_type>::value;
};

template<typename T>
struct __is_container
{
private:
    template<typename U>
    static auto Check(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    constexpr static bool value =
        (std::is_same<decltype(Check<T>(0)),std::true_type>::value) &&
        (std::is_same<T, std::string>::value == false);
};

//__c51de4f9a78a4f16b2484190083e85f6
template<typename T>
struct __is_functor_base
{
private:
    template<typename U>
    static auto Check(int) -> decltype(
        std::declval<U>().__c51de4f9a78a4f16b2484190083e85f6(),
        std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    constexpr static bool value =
        (std::is_same<decltype(Check<T>(0)), std::true_type>::value) &&
        (std::is_same<T, std::string>::value == false);
};

template <typename F, typename... Args>
struct __is_invocable__ : std::is_constructible<
std::function<void(Args ...)>,
std::reference_wrapper<typename std::remove_reference<F>::type>>
{
};

template <typename R, typename F, typename... Args>
struct __is_invocable_r__ : std::is_constructible<
std::function<R(Args ...)>,
std::reference_wrapper<typename std::remove_reference<F>::type>>
{
};

//反转std::tuple
//使用举例：auto rt = RTuple<decltype(t), std::tuple_size<decltype(t)>::value>::reverse(t);
template<typename T, size_t N>
struct __RTuple
{
    static auto reverse(T& tup)
    {
        return std::move(std::tuple_cat(std::make_tuple(std::get<N - 1>(tup)),
                                        __RTuple<T, N - 1>::reverse(tup)));
    }
};

template<typename T >
struct __RTuple<T, 1>
{
    static auto reverse(T& tup)
    {
        return std::move(std::make_tuple(std::get<0>(tup)));
    }
};

template<typename T>
struct __RTuple<T, 0>
{
    static auto reverse(T& tup)
    {
        return std::move(tup);
    }
};

/*
  template<typename Tuple, typename F, int N>
  struct TupleHelp
  {
  static void map_invoke(Tuple &t, F func)
  {
  TupleHelp<Tuple, F, N - 1>::map_invoke(t, func);
  get<N - 1>(t) = func(get<N - 1>(t));
  }
  };

  template<typename Tuple, typename F>
  struct TupleHelp<Tuple, F, 0>
  {
  static void map_invoke(Tuple &t , F func)

  //nothing.
  }
  };
*/

/*
  template<typename ...Args, typename F>
  std::tuple<Args...> map(const std::tuple<Args...> &t_, F func)
  {
  std::tuple<Args...> t = t_;
  TupleHelp<std::tuple<Args...>, F, sizeof...(Args)>::map_invoke(t, func);
  return t;
  }
*/

template<size_t ...Ns, typename ...Ts, typename F>
auto __for_each_implc4e53be189c54bbebc11a5b0842b6e6c(
    std::index_sequence<Ns...>,
    const std::tuple<Ts...> &t,
    F func)
{
    int a[] = { (func(std::get<Ns>(t)), 0)... };
    return t;
}

template<typename ...Ts, typename F>
auto __for_each(const std::tuple<Ts...> &t, F func)
{
    return __for_each_implc4e53be189c54bbebc11a5b0842b6e6c(std::make_index_sequence<
            sizeof...(Ts)> {},
            t,
            func);
}

template<typename C, typename F>
auto __for_each(const C &c, F func)
{
    std::for_each(c.begin(), c.end(), func);
    return c;
}

template<typename T1, typename T2, typename F>
auto __for_each(const std::pair<T1, T2> &p, F func)
{
    func(p.first);
    func(p.second);

    return p;
}

template<size_t ...Ns, typename ...Args, typename F>
auto __map_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        const std::tuple<Args...> &t,
        F func)
{
    return std::make_tuple(func(std::get<Ns>(t))...);
}
template<typename ...Args, typename F>
auto __map(const std::tuple<Args...> &t, F func)
{
    return __map_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::make_index_sequence<
            sizeof...(Args)> {},
            t,
            func);
}

template<typename C, typename F>
C __map(const C &c, F func)
{
    C ret;
    std::transform(c.begin(), c.end(), std::inserter(ret, ret.begin()), func);
    return ret;
}

template<typename T1, typename T2, typename F>
auto __map(const std::pair<T1, T2> &p, F func)
{
    return std::make_pair(func(p.first), func(p.second));
}

template<typename C>
auto __sort(const C &c)
{
    C ret = c;
    std::sort(ret.begin(), ret.end());
    return ret;
}

template<typename C, typename Compare>
auto __sort(const C &c, Compare compare)
{
    C ret = c;
    std::sort(ret.begin(), ret.end(), compare);
    return ret;
}

template<typename C, typename F, typename I>
auto __reduce(const C &c, F func, I init)
{
    return std::accumulate(c.begin(), c.end(), init, func);
}

template<typename T1, typename T2, typename F, typename I>
auto __reduce(const std::pair<T1, T2> &p, F func, I init)
{
    return func(func(init, p.first), p.second);
}

template<size_t ...Ns, typename ...Args, typename F, typename I>
auto __reduce_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        const std::tuple<Args...> &t,
        F func,
        I init)
{
    int a[] = { (init = func(init, std::get<Ns>(t)),0)... };
    return init;
}

template<typename ...Args, typename F, typename I>
auto __reduce(const std::tuple<Args...> &t, F func, I init)
{
    return __reduce_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::
            make_index_sequence<
            sizeof...(Args)> {},
            t,
            func,
            init);
}

template<typename C, typename F>
bool __exist(const C &c, F func)
{
    return std::find_if(c.begin(), c.end(), func) != c.end();
}

template<size_t ...Ns, typename ...Args, typename F>
bool __exist_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        const std::tuple<Args...> &t,
        F func)
{
    bool ret = false;
    int a[] = { (ret = (ret || func(std::get<Ns>(t))),0)... };
    return ret;
}

template<typename ...Args, typename F>
bool __exist(const std::tuple<Args...> &t, F func)
{
    return __exist_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::
            make_index_sequence<
            sizeof...(Args)> {},
            t,
            func);
}

template<typename T>
bool __is_equal(T _1, T _2) {
    return _1 == _2;
}

template<typename T1, typename T2>
bool __is_equal(T1 _1, T2 _2) {
    return false;
}

template<typename T1, typename T2, typename F>
bool __exist(const std::pair<T1, T2> &p, F func)
{
    return func(p.first) || func(p.second);
}

template<typename C, typename F>
std::size_t __count(const C &c, F func)
{
    return std::count_if(c.begin(), c.end(), func);
}

template<typename C>
std::size_t __count(const C &c)
{
    return c.size();
}

template<size_t ...Ns, typename ...Args, typename F>
std::size_t __count_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        const std::tuple<Args...> &t,
        F func)
{
    std::size_t ret = 0;
    int a[] = { (func(std::get<Ns>(t)) ? ret++ : ret, 0)... };
    return ret;
}

template<typename ...Args, typename F>
std::size_t __count(const std::tuple<Args...> &t, F func)
{
    return __count_tuple_implc4e53be189c54bbebc11a5b0842b6e6c(std::
            make_index_sequence<
            sizeof...(Args)> {},
            t,
            func);
}

template<typename ...Args>
std::size_t __count(const std::tuple<Args...> &t)
{
    return sizeof...(Args);
}

template<typename T1, typename T2, typename F>
std::size_t __count(const std::pair<T1, T2> &p, F func)
{
    std::size_t ret = 0;

    if (func(p.first))  ret++;
    if (func(p.second)) ret++;

    return ret;
}

template<typename T1, typename T2>
std::size_t __count(const std::pair<T1, T2> &p)
{
    return 2;
}

template <typename Collection, typename Predicate>
Collection __filterNot(Collection col, Predicate predicate) {
    auto returnIterator = std::remove_if(col.begin(), col.end(), predicate);
    col.erase(returnIterator, std::end(col));
    return col;
}

template <typename Collection, typename Predicate>
Collection __filter(Collection col, Predicate predicate) {
    //capture the predicate in order to be used inside function
    auto fnCol = __filterNot(col, [predicate](typename Collection::value_type i)
    {
        return !predicate(i);
    });
    return fnCol;
}

template<typename ...Args, typename F>
auto __filter(const std::tuple<Args...> &t, F func)
{
    assert(false);
}

template<typename ...Args, typename F>
auto __filterNot(const std::tuple<Args...> &t, F func)
{
    auto res = __filter(t, [func](auto item)
    {
        return !func(item);
    });

    return res;
}

template<size_t ...Ns, typename ...Ts>
auto __filter(const std::tuple<Ts...> &t)
{
    return std::make_tuple(std::get<Ns>(t)...);
}

template<size_t ...Ns, typename T>
auto __filter(const std::vector<T> &vec)
{
    std::vector<T> ret = { vec[Ns]... };
    return ret;
}

template<size_t ...Ns, typename T, size_t N>
auto __filter(const std::array<T, N> &array)
{
    std::array<T, sizeof...(Ns)> ret = { array[Ns]... };
    return ret;
}

template<typename Collection, typename Predict,
         typename = typename std::enable_if<
             __is_invocable__<Predict, typename Collection::value_type>::value>::type>
auto __find(const Collection &c, Predict func)
{
    typename Collection::const_iterator iter = c.begin();
    typename Collection::const_iterator ret = c.end();

    __exist(c, [&iter, &ret, func](auto _1) {
        bool flag = func(_1);
        if (flag) ret = iter;
        iter++;
        return flag;
    });

    return ret;
}

template<typename Collection, typename ValueType,
         typename = typename std::enable_if<
             std::is_same<ValueType, typename Collection::value_type>::value>::type,
         size_t = 0>
auto __find(const Collection &c, ValueType val)
{
    return std::find(c.begin(), c.end(), val);
}

template<typename Collection,
         typename = typename std::enable_if<
             is_not_random_container<Collection>::value>::type>
auto __index(const Collection &c, int index)
{
    typename Collection::const_iterator iter = c.begin();
    typename Collection::const_iterator ret = c.end();

    int cnt = 0;
    __exist(c, [&iter, &ret, index, &cnt](auto _1) {
        bool flag = (cnt == index);
        if (flag) ret = iter;
        cnt++, iter++;
        return flag;
    });

    return ret;
}

template<typename Collection,
         typename = typename std::enable_if<is_random_container<Collection>::value>::type,
         size_t = 0>
auto __index(const Collection &c, int index)
{
    return index == -1 ? c.end() : c.begin() + index;
}


template<typename T, typename ...Ts>
T __head(std::tuple<T, Ts...> t)
{
    return std::get<0>(t);
}

template<typename ...Ts>
auto __last(std::tuple<Ts...> t)
{
    return std::get<std::tuple_size<decltype(t)>::value - 1u>(t);
}

template<size_t ...Ns, typename ...Ts>
auto __tail_c4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        std::tuple<Ts...> t)
{
    return make_tuple(std::get<Ns + 1u>(t)...);
}

template<typename ...Ts>
auto __tail(std::tuple<Ts...> t)
{
    return _tail_c4e53be189c54bbebc11a5b0842b6e6c(std::make_index_sequence<
            sizeof...(Ts) - 1u> {},
            t);
}

template<size_t ...Ns, typename ...Ts>
auto __init_c4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        std::tuple<Ts...> t)
{
    return std::make_tuple(std::get<Ns>(t)...);
}

template<typename ...Ts>
auto __init(std::tuple<Ts...> t)
{
    return __init_c4e53be189c54bbebc11a5b0842b6e6c(std::make_index_sequence<
            sizeof...(Ts) - 1u>(),
            t);
}

template<typename ...Arg>
constexpr std::size_t __size(const std::tuple<Arg...> &t)
{
    return std::tuple_size<decltype(t)>::value;
}

template<typename T>
class __Task;

template<typename R, typename...Args>
class __Task<R(Args...)>
{
public:
    __Task(std::function<R(Args...)>&& f) : m_fn(std::move(f)) {}
    __Task(std::function<R(Args...)>& f) : m_fn(f) {}

    template<typename... InnerArgs>
    R Run(InnerArgs&&... args)
    {
        return m_fn(std::forward<InnerArgs>(args)...);
    }

    template<typename F>
    auto Then(F&& f)->__Task<typename std::result_of<F(R)>::type(Args...)>
    {
        return __Task<typename std::result_of<F(R)>::type(Args...)>
        ([this, &f](Args&&... args) {
            return f(m_fn(std::forward<Args>(args)...));
        });
    }

private:
    std::function<R(Args...)> m_fn;
};

/*  Y-组合子
 *  使用限制：显式指定lambda函数的返回值。代码举例如下：
 *  auto gcd = fix(
 *                [](auto g, int a, int b)->int{return b == 0 ? a : g(b, a%b);}
 *                );
 */

template <typename F>
struct __Y
{
    __Y(F f) : _f(f) {}
    template <typename... Args>
    auto operator()(Args&&... t) const
    {
        return _f(*this, std::forward<Args>(t)...);
    }
    F _f;
};

template <typename F>
__Y<F> __fix(F&& f)
{
    return __Y<F>(std::forward<F>(f));
}

/*
 * 1）function_traits: 对函数基本信息进行了解构和处理
 *    可以支持普通函数，成员函数，访问数据成员，lambda
 * 2）辅助函数-make_func()： 把函数、lambda 表达式、仿函数等可调用对象转换成一个 std::function 对象
 * 3）辅助函数-arity() 会返回函数的所需的参数数量。
 * 4）辅助函数-argument_type，下面两种表达方式等价：
 *    argument_type<decltype(bar), 0>::type
 *    function_traits<decltype(bar)>::argument<0>::type
 */

template<typename F>
struct __is_lambda {
    constexpr static bool value = false;
};

template<typename Ret, typename ...Args>
struct __is_lambda<Ret(*)(Args...)>
: public __is_lambda<Ret(Args...)>
{
    constexpr static bool value = true;
};

template <class F> struct __function_traits;

// normal function, static member function
template <class Return, class...Args>
struct __function_traits<Return(Args...)>
{
    using func_type = std::function<Return(Args...)>;
    using return_type = Return;
    static constexpr std::size_t arity = sizeof...(Args);
    template <std::size_t I> struct argument
    {
        static_assert(I < arity,
                      "error: invalid index of this function's parameter.");
        using type = typename
                     std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

// function pointer, &f
template <class Return, class...Args>
struct __function_traits<Return(*)(Args...)>
: public __function_traits<Return(Args...)> {};

// std::function
template <class Return, class...Args>
struct __function_traits<std::function<Return(Args...)>>
: public __function_traits<Return(Args...)> {};

// functor, callable object, struct with overloaded operator()
template <typename Functor>
struct __function_traits
: public __function_traits<decltype(&Functor::operator())> {};

// lambda expression, const member function pointer
template <class Class, class Return, class...Args>
struct __function_traits<Return(Class::*)(Args...) const>
    : public __function_traits<Return(Args...)> {};

// member function pointer
template <class Class, class Return, class...Args>
struct __function_traits<Return(Class::*)(Args...)>
: public __function_traits<Return(Args...)> {};

// member object pointer
template <class Class, class Return>
struct __function_traits<Return(Class::*)> : public __function_traits<Return()> {};

// clear reference F&, F&&
template <class F> struct __function_traits<F&> : public __function_traits<F> {};
template <class F> struct __function_traits<F&&> : public __function_traits<F> {};

// make_func(), return a std::function<> object from function-like object
template <class Func> auto __make_func(Func && f)
{
    typename __function_traits<Func>::func_type _f = std::forward<decltype(f)>(f);
    return std::forward<decltype(_f)>(_f);
}

template <class Func> auto __F(Func && f)
{
    typename __function_traits<Func>::func_type _f = std::forward<decltype(f)>(f);
    return std::forward<decltype(_f)>(_f);
}

// arity(), return numbers of parameters
template <class Func> inline constexpr std::size_t __arity(Func &&)
{
    return __function_traits<std::remove_reference_t<Func>>::arity;
}

// argument_type<Func, I>::type, return I-th parameter's type, I start from 0
template <class Func, std::size_t I> struct __argument_type
{
    using type = typename __function_traits<Func>::template argument<I>::type;
};

template<class Func>
struct __Ret
{
    using type = typename __function_traits<Func>::return_type;
};

/*
 * curry-基于模板特化的柯里化实现
 * 注意：所有的参数和函数对象本身都是用右值引用来进行转发，保证了效率和值的不变性
 *      使用的时候，我们就可以保持原函数的写法不做改变，只需要外部对其进行一次 curry() 修饰即可。
 * 从函数式程序设计的思维来说：
 *      (1)严格执行了柯里化的定义，即函数永远只接受一个参数;
 *      (2)采用柯里化部分应用的主要目标其实就是减少不必要的代码量，让代码更精简，让逻辑更清晰。
 *         举一个简单的例子，一个 MVC 三层架构的系统，完全可以把一个在 M 层部分应用的函数传到 C 层，
 *         然后再部分应用后传到 V 层做调用，这样就大大简化了跨逻辑层的参数传递问题。
 */

template <class Return>
auto __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::function<Return()> && f)
{
    return std::forward<decltype(f)>(f);
}

template <class Return, class Arg>
auto __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::function<Return(Arg)> && f)
{
    return std::forward<decltype(f)>(f);
}

template <class Return,
          class Arg,
          typename = typename std::enable_if<(std::is_reference<Arg>::value == false)>::type,
          size_t = 0,
          class... Args>
auto __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::function<
        Return(Arg, Args...)> && f)
{
    auto _f = [f = std::forward<decltype(f)>(f)](Arg arg)
    {
        std::function<Return(Args...)> rest
            = [f, arg]
        (Args... args)->Return {
            return f(arg, args...);
        };
        return __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::forward<
                decltype(rest)>(rest));
    };

    return __make_func(_f);
}

template <class Return,
          class Arg,
          typename = typename std::enable_if<(std::is_reference<Arg>::value == true)>::type,
          class... Args>
auto __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::function<
        Return(Arg, Args...)> && f)
{
    auto _f = [f = std::forward<decltype(f)>(f)](Arg arg)
    {
        std::function<Return(Args...)> rest
            = [f, &arg]
        (Args... args)->Return {
            return f(arg, args...);
        };
        return __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::forward<
                decltype(rest)>(rest));
    };

    return __make_func(_f);
}

template <class Func> auto __curry(Func && f)
{
    auto _f = __make_func(f);
    return __curry_implc4e53be189c54bbebc11a5b0842b6e6c(std::forward<
            decltype(_f)>(_f));
}

template <class Return, class Arg>
auto operator<=(const std::function<Return(Arg)> &f, const Arg &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return, class Arg>
auto operator<=(const std::function<Return(const Arg&)> &f, const Arg &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return>
auto operator<=(const std::function<Return(std::ostream&)> &f, std::ostream &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class F, class Arg,
          typename = typename std::enable_if<__is_invocable__<F, Arg>::value>::type>
auto operator<=(F func, const Arg &arg)
{
    return __make_func(func) <<= arg;
}

template <class Return, class Arg>
auto operator/(const std::function<Return(Arg)> &f, const Arg &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return, class Arg>
auto operator/(const std::function<Return(const Arg&)> &f, const Arg &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return>
auto operator/(const std::function<Return(std::ostream&)> &f, std::ostream &arg)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class F, class Arg,
          typename = typename std::enable_if<__is_invocable__<F, Arg>::value>::type>
auto operator/(F func, const Arg &arg)
{
    return __make_func(func) <<= arg;
}

template <class Return, class Arg>
auto operator|(const Arg &arg, const std::function<Return(Arg)> &f)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return, class Arg>
auto operator|(const Arg &arg, const std::function<Return(const Arg&)> &f)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class Return>
auto operator|(std::ostream &arg, const std::function<Return(std::ostream&)> &f)
{
    return f(/*std::forward<Arg>*/(arg));
}

template <class F, class Arg,
          typename = typename std::enable_if<__is_invocable__<F, Arg>::value>::type>
auto operator|(const Arg &arg, F func)
{
    return arg | __make_func(func);
}

/*
//函数组合-compose
template <class... Fs>
class composited_functor
{
  template<typename ...Fs1>
  friend auto reverse(const composited_functor<Fs1...> &func);
private:
  const std::tuple<Fs...> fs_;
  const static std::size_t _size_ = sizeof...(Fs);

  template <class Arg, std::size_t I>
  auto call_impl(Arg && arg, const std::index_sequence<I>&) const
	  -> decltype(std::get<I>(fs_)(std::forward<Arg>(arg)))
  {
	  return std::get<I>(fs_)(std::forward<Arg>(arg));
  }

  template <class Arg, std::size_t I, std::size_t... Is>
  auto call_impl(Arg && arg, const std::index_sequence<I, Is...>&) const
	  -> decltype(call_impl(std::get<I>(fs_)(
                          std::forward<Arg>(arg)), std::index_sequence<Is...>{}))
  {
	  return call_impl(std::get<I>(fs_)(std::forward<Arg>(arg)),
                                                std::index_sequence<Is...>{});
  }

  template <class Arg> auto call(Arg && arg) const
	  -> decltype(call_impl(std::forward<Arg>(arg),
                                    std::make_index_sequence<_size_>{}))
  {
	  return call_impl(std::forward<Arg>(arg), std::make_index_sequence<_size_>{});
  }
public:
  composited_functor() : fs_(std::tuple<>()) {}
  composited_functor(std::tuple<Fs...> && fs) : fs_(std::forward<decltype(fs)>(fs)) {}

  template <class F>
  inline auto composite(F && f) const
  {
	return composited_functor<Fs..., F>(std::tuple_cat(fs_,
                                                std::make_tuple(std::forward<F>(f))));
  }

  template <class Arg>
  inline auto operator()(Arg && arg) const
  {
	  return call(std::forward<Arg>(arg));
  }
};

template<typename ...Fs>
auto compose(Fs... fs)
{
  auto fs_tuple = make_tuple(fs...);
  auto fs_reverse = RTuple<decltype(fs_tuple), sizeof...(fs)>::reverse(fs_tuple);

  return composited_functor<Fs...>(std::move(fs_reverse));
}


template<typename ...Fs>
auto reverse(const composited_functor<Fs...> &func)
  {
  auto fs_tuple = func.fs_;
  auto fs_reverse = RTuple<decltype(fs_tuple), sizeof...(Fs)>::reverse(fs_tuple);

  return composited_functor<Fs...>(std::move(fs_reverse));
}
*/

/*
template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(MaybeB)> & f,
                   const std::function<MaybeB(MaybeA)> & g)
  -> std::function<MaybeC(MaybeA)>
{
  std::function<MaybeC(MaybeA)> composited
	= [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
	(MaybeA arg)->MaybeC { return f(g(arg)); };
  return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(const MaybeB&)> & f,
	   const std::function<MaybeB(const MaybeA&)> & g)
  -> std::function<MaybeC(const MaybeA&)>
{
  std::function<MaybeC(MaybeA)> composited
	= [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
	(MaybeA arg)->MaybeC { return f(g(arg)); };
  return __make_func(composited);
}
*/

//compose 函数组合
template <class MaybeA, class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(MaybeB)> & f,
               const std::function<MaybeB(MaybeA)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(const MaybeB&)> & f,
               const std::function<MaybeB(const MaybeA&)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(MaybeB)> & f,
               const std::function<MaybeB(const MaybeA&)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(const MaybeB&)> & f,
               const std::function<MaybeB(MaybeA)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

//__compose 最内层函数void参数版本
template <class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(MaybeB)> & f,
               const std::function<MaybeB(void)> & g)
-> std::function<MaybeC(void)>
{
    std::function<MaybeC(void)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (void)->MaybeC { return f(g()); };
    return __make_func(composited);
}

template <class MaybeB, class MaybeC>
auto __compose(const std::function<MaybeC(const MaybeB&)> & f,
               const std::function<MaybeB(void)> & g)
-> std::function<MaybeC(void)>
{
    std::function<MaybeC(void)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (void)->MaybeC { return f(g()); };
    return __make_func(composited);
}

template<class F1, class F2>
auto __compose(F1 f, F2 g)
{
    return __compose(__make_func(f), __make_func(g));
}

template<typename F1, typename F2, typename ...Fs>
auto __compose(F1 f1, F2 f2, Fs... fs)
{
    return __compose(__compose(f1, f2), fs...);
}

//__pipe 管道
template <class MaybeA, class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(MaybeA)> & f,
            const std::function<MaybeC(MaybeB)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return g(f(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(const MaybeA&)> & f,
            const std::function<MaybeC(const MaybeB&)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return g(f(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(const MaybeA&)> & f,
            const std::function<MaybeC(MaybeB)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return g(f(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(MaybeA)> & f,
            const std::function<MaybeC(const MaybeB&)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return g(f(arg)); };
    return __make_func(composited);
}

//__compose 最内层函数void参数版本
template <class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(void)> & f,
            const std::function<MaybeC(MaybeB)> & g)
-> std::function<MaybeC(void)>
{
    std::function<MaybeC(void)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (void)->MaybeC { return g(f()); };
    return __make_func(composited);
}

template <class MaybeB, class MaybeC>
auto __pipe(const std::function<MaybeB(void)> & f,
            const std::function<MaybeC(const MaybeB&)> & g)
-> std::function<MaybeC(void)>
{
    std::function<MaybeC(void)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (void)->MaybeC { return g(f()); };
    return __make_func(composited);
}

template<class F1, class F2>
auto __pipe(F1 f, F2 g)
{
    return __pipe(__make_func(f), __make_func(g));
}

template<typename F1, typename F2, typename ...Fs>
auto __pipe(F1 f1, F2 f2, Fs... fs)
{
    return __pipe(__pipe(f1, f2), fs...);
}

//operator+
template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(MaybeB)> & f,
               const std::function<MaybeB(MaybeA)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(const MaybeB&)> & f,
               const std::function<MaybeB(const MaybeA&)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(MaybeB)> & f,
               const std::function<MaybeB(const MaybeA&)> & g)
-> std::function<MaybeC(const MaybeA&)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return f(g(arg)); };
    return __make_func(composited);
}

template <class MaybeA, class MaybeB, class MaybeC>
auto operator+(const std::function<MaybeC(const MaybeB&)> & f,
               const std::function<MaybeB(MaybeA)> & g)
-> std::function<MaybeC(MaybeA)>
{
    std::function<MaybeC(MaybeA)> composited
        = [f = std::forward<decltype(f)>(f), g = std::forward<decltype(g)>(g)]
          (MaybeA arg)->MaybeC { return g(f(arg)); };
    return __make_func(composited);
}

template<typename Ret, typename Arg, typename F2>
auto operator+(std::function<Ret(Arg)> f1, F2 f2)
{
    return f1 + __make_func(f2);
}

template<typename Ret, typename Arg, typename F2>
auto operator+(std::function<Ret(const Arg &)> f1, F2 f2)
{
    return f1 + __make_func(f2);
}

template<typename Ret, typename Arg, typename F1>
auto operator+(F1 f1, std::function<Ret(Arg)> f2)
{
    return __make_func(f1) + f2;
}

template<typename Ret, typename Arg, typename F1>
auto operator+(F1 f1, std::function<Ret(const Arg &)> f2)
{
    return __make_func(f1) + f2;
}

//gather 收缩
template<typename ReturnType, typename ...Args>
auto __gather(const std::function<ReturnType(Args...)> &func)
{
    return __make_func([func](const std::tuple<Args...> &tup)->ReturnType {
        return std::apply(func, tup);
    });
}

template<typename Func>
auto __gather(const Func &func)
{
    return __gather(__make_func(func));
}

//spread 展开
template<typename ReturnType, typename ...Args>
auto __spread(const std::function<ReturnType(const std::tuple<Args...>&)> &func)
{
    return __make_func([func](Args... args)->ReturnType {
        return func(std::make_tuple(args...));
    });
}

template<typename Func>
auto __spread(const Func &func)
{
    return __spread(__make_func(func));
}

//tuple reverse
template<typename>
struct __templated_base_casec4e53be189c54bbebc11a5b0842b6e6c;

template <template<typename...> class T, typename... TArgs>
struct __templated_base_casec4e53be189c54bbebc11a5b0842b6e6c<T<TArgs...>>
{
    using type = T<>;
};

template<
    typename T, // Input
    typename = typename __templated_base_casec4e53be189c54bbebc11a5b0842b6e6c<T>::type>
// Reversed output
struct __reverse_implc4e53be189c54bbebc11a5b0842b6e6c;

template<
    template <typename...> class T,
    typename... TArgs>
struct __reverse_implc4e53be189c54bbebc11a5b0842b6e6c<
    typename __templated_base_casec4e53be189c54bbebc11a5b0842b6e6c<T<TArgs...>>::type,
            T<TArgs...>>
{
    using type = T<TArgs...>;
};

template<
    template<typename...> class T,
    typename x,
    typename... xs,
    typename... done>
struct __reverse_implc4e53be189c54bbebc11a5b0842b6e6c<
    T<x, xs...>,
    T<done...>>
{
    using type = typename __reverse_implc4e53be189c54bbebc11a5b0842b6e6c<
                 T<xs...>, T<x, done...>>::type;
};

template<typename ReturnType, typename ...Args>
auto __reverse(const std::function<ReturnType(const std::tuple<Args...> &)> &func)
{
    using TupleType = std::tuple<Args...>;
    using RevertTupleType = typename __reverse_implc4e53be189c54bbebc11a5b0842b6e6c<
                            TupleType>::type;
    auto ret = [func](const RevertTupleType &tup)->ReturnType {
        auto rt = __RTuple<decltype(tup), sizeof...(Args)>::reverse(tup);

        return func(rt);
    };

    return __make_func(ret);
}

//reverse 颠倒

//大于5时的通用版本, 缺陷：参数无法为引用形式，必须为原始的值传递
template<typename ReturnType, typename ...Args,
         typename = typename std::enable_if<(sizeof...(Args) > 3)>::type>
auto __reverse(const std::function<ReturnType(Args...)> &func)
{
    return __spread(__reverse(__gather(func)));
}

//2个参数版本
template<typename ReturnType, typename Arg1, typename Arg2>
auto __reverse(const std::function<ReturnType(Arg1, Arg2)> &func)
{
    return __make_func([func](Arg2 arg2, Arg1 arg1)
    {
        return func(arg1, arg2);
    });
}

//3个参数版本
template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3>
auto __reverse(const std::function<ReturnType(Arg1, Arg2, Arg3)> &func)
{
    return __make_func([func](Arg3 arg3, Arg2 arg2, Arg1 arg1)
    {
        return func(arg1, arg2, arg3);
    });
}

//4个参数版本
template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
auto __reverse(const std::function<ReturnType(Arg1, Arg2, Arg3, Arg4)> &func)
{
    return __make_func([func](Arg4 arg4, Arg3 arg3, Arg2 arg2, Arg1 arg1)
    {
        return func(arg1, arg2, arg3, arg4);
    });
}

//5个参数版本
template<typename ReturnType,
         typename Arg1,
         typename Arg2,
         typename Arg3,
         typename Arg4,
         typename Arg5>
auto __reverse(const std::function<ReturnType(Arg1, Arg2, Arg3, Arg4, Arg5)> &func)
{
    return __make_func([func](Arg5 arg5, Arg4 arg4, Arg3 arg3, Arg2 arg2, Arg1 arg1)
    {
        return func(arg1, arg2, arg3, arg4, arg5);
    });
}

template<typename Func>
auto __reverse(const Func &func)
{
    return __reverse(__make_func(func));
}

//partial 左偏
template<typename ReturnType, typename Arg, typename ...Args>
auto __partial(std::function<ReturnType(Arg, Args...)> func, Args... args)
{
    return __make_func([func, args...](Arg arg)->ReturnType{
        return func(arg, args...);
    });
}

template<typename Func, typename ...Args>
auto __partial(const Func &func, Args... args)
{
    return __partial(__make_func(func), args...);
}

//partialRight 右偏
template<typename Func, size_t ...Ns, typename ...Args>
auto __partial_right_implc4e53be189c54bbebc11a5b0842b6e6c(std::index_sequence<Ns...>,
        const Func &func,
        const std::tuple<Args...> &tup)
{
    return __partial(func, std::get<Ns>(tup)...);
}
template<typename Func, typename ...Args>
auto __partialRight(const Func &func, Args... args)
{
    auto t = std::make_tuple(args...);
    auto rt = __RTuple<decltype(t), sizeof...(args)>::reverse(t);

    auto r_func = __reverse(__make_func(func));

    return __partial_right_implc4e53be189c54bbebc11a5b0842b6e6c(std::make_index_sequence<
            sizeof...(Args)> {},
            r_func,
            rt);
}

//curryLeft 柯里化左侧第一个参数;
//用于克服curry函数实现策略的缺陷，实现多个关联紧密的参数同时调用。
template<typename ReturnType, typename Arg, typename ...Args>
auto __curryLeft(const std::function<ReturnType(Arg, Args...)> &func)
{
    return __make_func([func](Arg arg) {
        return __make_func([func, arg](Args... args) {
            return func(arg, args...);
        });
    });
}

template<typename Fn>
auto __curryLeft(const Fn &func)
{
    return __curryLeft(__make_func(func));
}

//curryRight 柯里化右侧最后一个参数
//用于克服curry函数实现策略的缺陷，实现多个关联紧密的参数同时调用。
template<typename ReturnType, typename Arg, typename ...Args>
auto __curryRightImpl(const std::function<ReturnType(Arg, Args...)> &func)
{
    return __make_func([func](Arg arg) {
        return __reverse(__make_func([func, arg](Args... args) {
            return func(arg, args...);
        }));
    });
}
template<typename Fn>
auto __curryRight(const Fn &func)
{
    return __curryRightImpl(__reverse(func));
}

//tap 弃离
template<typename ReturnType, typename Arg>
auto __tap(const std::function<ReturnType(Arg)> &func)
{
    typedef typename std::remove_reference<Arg>::type ArgType;
    return __make_func([func](ArgType arg)->ArgType {
        return (func(arg), arg);
    });
}

template<typename Func>
auto __tap(const Func &func)
{
    return __tap(__make_func(func));
}

//alt 交替
template<typename ReturnType, typename Arg>
auto __alt(const std::function<ReturnType(Arg)> &func1,
           const std::function<ReturnType(Arg)> &func2,
           const std::function<bool(ReturnType)> &predict)
{
    return __make_func([func1, func2, predict](Arg arg) {
        ReturnType ret = func1(arg);
        if (predict(ret))
        {
            return ret;
        }
        else
        {
            return func2(arg);
        }
    });
}

template<typename F1, typename F2, typename Predict>
auto __alt(const F1 &func1, const F2 &func2, const Predict &predict)
{
    return __alt(__make_func(func1), __make_func(func2), __make_func(predict));
}

//defaultTo
template<typename T>
auto __defaultTo(const T &def, const std::function<bool(T)> &predict)
{
    return __make_func([def, predict](const T &val) {
        if (predict(val))
        {
            return val;
        }
        else
        {
            return def;
        }
    });
}

template<typename T, typename Predict>
auto __defaultTo(const T &def, const Predict &predict)
{
    return __defaultTo(def, __make_func(predict));
}

//identity 照旧
template<typename Arg>
struct __identity
{
    static auto func(Arg arg)
    {
        return arg;
    }
};

//nothing 无为
template<typename Arg>
struct __nothing
{
    static auto func(Arg)
    {
        //nothing.
    }
};

//always 恒定
template<typename T>
auto __always(T &&constant)
{
    return __make_func([constant]() {
        return constant;
    });
}

//tryCatch 补救
template<typename ReturnType, typename Arg>
auto __tryCatch(const std::function<ReturnType(Arg)> &tryer,
                const std::function<ReturnType(Arg)> &catcher)
{
    return __make_func([tryer, catcher](Arg arg) {
        try
        {
            return tryer(arg);
        }
        catch (...)
        {
            return catcher(arg);
        }
    });
}

template<typename Tryer, typename Catcher>
auto __tryCatch(const Tryer &tryer, const Catcher &catcher)
{
    return __tryCatch(__make_func(tryer), __make_func(catcher));
}

//seq 同时
template<typename ReturnType, typename Arg, typename ...Fs>
auto __seq(std::function<ReturnType(Arg)> fn, Fs... fs)
{
    return __make_func([fn, fs...](Arg arg)
    {
        fn(arg);
        int a[] = { (fs(arg), 0)... };
    });
}

template<typename F, typename ...Fs>
auto __seq(F fn, Fs... fs)
{
    return __seq(__make_func(fn), fs...);
}

//converge 聚集
template<typename ...Fs,
         typename ...Args, typename ReturnType,
         typename Arg, typename ReturnType1>
auto __converge(std::function<ReturnType(Args...)> fn,
                std::function<ReturnType1(Arg)> fs1, Fs ...fs)
{
    return __make_func([fn, fs1, fs...](Arg arg) {
        return fn(fs1(arg), fs(arg)...);
    });
}

template<typename Fn, typename Fs1, typename ...Fs>
auto __converge(Fn fn, Fs1 fs1, Fs... fs)
{
    return __converge(__make_func(fn), __make_func(fs1), fs...);
}

//useWith 适配特例，大于一个参数且类型相同
template<typename ReturnType,
         typename ...Args,
         typename ReturnTypeAdapt,
         typename ArgAdapt>
auto __adapt_impl(std::function<ReturnType(Args...)> fn,
                  std::function<ReturnTypeAdapt(ArgAdapt)> adaptor)
{
    return  __make_func([fn, adaptor](const tuple_of<sizeof...(Args), ArgAdapt> &args)
    {
        return __gather(fn)(__map(args, adaptor));
    });
}

template<typename Ret, typename ...Args, typename Adapt,
         typename = typename std::enable_if<(sizeof...(Args) > 1)>::type>
auto __useWith(std::function<Ret(Args...)> fn,
               Adapt adaptor)
{
    return __spread(__adapt_impl(__make_func(fn), __make_func(adaptor)));
}

//useWith 分拣
//1个参数
template<typename Ret,
         typename RetAdaptor1, typename Arg1>
auto __useWith_impl(std::function<Ret(RetAdaptor1)> fn,
                    std::function<RetAdaptor1(Arg1)> adaptor1)
{
    return  __make_func([fn, adaptor1](const Arg1 &arg1)
    {
        return fn(adaptor1(arg1));
    });
}

template<typename Fn, typename Adapt1>
auto __useWith(Fn fn, Adapt1 adaptor1)
{
    return __useWith_impl(__make_func(fn), __make_func(adaptor1));
}

//2个参数
template<typename Ret,
         typename RetAdaptor1, typename Arg1,
         typename RetAdaptor2, typename Arg2>
auto __useWith_impl(std::function<Ret(RetAdaptor1,
                                      RetAdaptor2)> fn,
                    std::function<RetAdaptor1(Arg1)> adaptor1,
                    std::function<RetAdaptor2(Arg2)> adaptor2)
{
    return  __make_func([fn,
                         adaptor1,
                         adaptor2](
                            const Arg1 &arg1,
                            const Arg2 &arg2)
    {
        return fn(adaptor1(arg1),
                  adaptor2(arg2));
    });
}

template<typename Fn,
         typename Adapt1,
         typename Adapt2>
auto __useWith(Fn fn,
               Adapt1 adaptor1,
               Adapt2 adaptor2)
{
    return __useWith_impl(__make_func(fn),
                          __make_func(adaptor1),
                          __make_func(adaptor2));
}

//3个参数
template<typename Ret,
         typename RetAdaptor1, typename Arg1,
         typename RetAdaptor2, typename Arg2,
         typename RetAdaptor3, typename Arg3>
auto __useWith_impl(std::function<Ret(RetAdaptor1,
                                      RetAdaptor2,
                                      RetAdaptor3)> fn,
                    std::function<RetAdaptor1(Arg1)> adaptor1,
                    std::function<RetAdaptor2(Arg2)> adaptor2,
                    std::function<RetAdaptor3(Arg3)> adaptor3)
{
    return  __make_func([fn,
                         adaptor1,
                         adaptor2,
                         adaptor3](
                            const Arg1 &arg1,
                            const Arg2 &arg2,
                            const Arg3 &arg3)
    {
        return fn(adaptor1(arg1),
                  adaptor2(arg2),
                  adaptor3(arg3));
    });
}

template<typename Fn,
         typename Adapt1,
         typename Adapt2,
         typename Adapt3>
auto __useWith(Fn fn,
               Adapt1 adaptor1,
               Adapt2 adaptor2,
               Adapt3 adaptor3)
{
    return __useWith_impl(__make_func(fn),
                          __make_func(adaptor1),
                          __make_func(adaptor2),
                          __make_func(adaptor3));
}

//4个参数
template<typename Ret,
         typename RetAdaptor1, typename Arg1,
         typename RetAdaptor2, typename Arg2,
         typename RetAdaptor3, typename Arg3,
         typename RetAdaptor4, typename Arg4>
auto __useWith_impl(std::function<Ret(RetAdaptor1,
                                      RetAdaptor2,
                                      RetAdaptor3,
                                      RetAdaptor4)> fn,
                    std::function<RetAdaptor1(Arg1)> adaptor1,
                    std::function<RetAdaptor2(Arg2)> adaptor2,
                    std::function<RetAdaptor3(Arg3)> adaptor3,
                    std::function<RetAdaptor4(Arg4)> adaptor4)
{
    return  __make_func([fn,
                         adaptor1,
                         adaptor2,
                         adaptor3,
                         adaptor4](
                            const Arg1 &arg1,
                            const Arg2 &arg2,
                            const Arg3 &arg3,
                            const Arg4 &arg4)
    {
        return fn(adaptor1(arg1),
                  adaptor2(arg2),
                  adaptor3(arg3),
                  adaptor4(arg4));
    });
}

template<typename Fn,
         typename Adapt1,
         typename Adapt2,
         typename Adapt3,
         typename Adapt4>
auto __useWith(Fn fn,
               Adapt1 adaptor1,
               Adapt2 adaptor2,
               Adapt3 adaptor3,
               Adapt4 adaptor4)
{
    return __useWith_impl(__make_func(fn),
                          __make_func(adaptor1),
                          __make_func(adaptor2),
                          __make_func(adaptor3),
                          __make_func(adaptor4));
}

//5个参数
template<typename Ret,
         typename RetAdaptor1, typename Arg1,
         typename RetAdaptor2, typename Arg2,
         typename RetAdaptor3, typename Arg3,
         typename RetAdaptor4, typename Arg4,
         typename RetAdaptor5, typename Arg5>
auto __useWith_impl(std::function<Ret(RetAdaptor1,
                                      RetAdaptor2,
                                      RetAdaptor3,
                                      RetAdaptor4,
                                      RetAdaptor5)> fn,
                    std::function<RetAdaptor1(Arg1)> adaptor1,
                    std::function<RetAdaptor2(Arg2)> adaptor2,
                    std::function<RetAdaptor3(Arg3)> adaptor3,
                    std::function<RetAdaptor4(Arg4)> adaptor4,
                    std::function<RetAdaptor5(Arg5)> adaptor5)
{
    return  __make_func([fn,
                         adaptor1,
                         adaptor2,
                         adaptor3,
                         adaptor4,
                         adaptor5](
                            const Arg1 &arg1,
                            const Arg2 &arg2,
                            const Arg3 &arg3,
                            const Arg4 &arg4,
                            const Arg5 &arg5)
    {
        return fn(adaptor1(arg1),
                  adaptor2(arg2),
                  adaptor3(arg3),
                  adaptor4(arg4),
                  adaptor5(arg5));
    });
}

template<typename Fn,
         typename Adapt1,
         typename Adapt2,
         typename Adapt3,
         typename Adapt4,
         typename Adapt5>
auto __useWith(Fn fn,
               Adapt1 adaptor1,
               Adapt2 adaptor2,
               Adapt3 adaptor3,
               Adapt4 adaptor4,
               Adapt4 adaptor5)
{
    return __useWith_impl(__make_func(fn),
                          __make_func(adaptor1),
                          __make_func(adaptor2),
                          __make_func(adaptor3),
                          __make_func(adaptor4),
                          __make_func(adaptor5));
}

template<typename T>
auto __stream(std::string head = "", std::string tail = "\n")
{
    return __make_func([head, tail](std::ostream &output, std::string msg, T val) {
        std::cout << msg << head << val << tail;
        return val;
    });
}

//__C
template<typename C>
struct __C { /*Container*/
    template<typename Func>
    static auto for_each(Func func)
    {
        return __make_func([func](const C &c)
        {
            return FP_Utils::__for_each(c, func);
        });
    }

    template<typename Func>
    static auto map(Func func)
    {
        return __make_func([func](const C &c)
        {
            return FP_Utils::__map(c, func);
        });
    }

    static auto sort()
    {
        return __make_func([](const C &c) {
            return FP_Utils::__sort(c);
        });
    }

    template<typename Compare>
    static auto sort(Compare compare) {
        return __make_func([compare](const C &c) {
            return FP_Utils::__sort(c, compare);
        });
    }

    template<typename Func, typename I>
    static auto reduce(Func func, I init)
    {
        return __make_func([func, init](const C &c)
        {
            return FP_Utils::__reduce(c, func, init);
        });
    }

    static auto reduce()
    {
        return reduce([](auto init, auto item) {
            return init + item;
        }, 0.0);
    }

    template<typename Func>
    static auto filter(Func func)
    {
        return __make_func([func](const C &c)
        {
            auto ret = FP_Utils::__filter(c, func);
            return ret;
        });
    }

    template<typename Func>
    static auto exist(Func func)
    {
        return __make_func([func](const C &c)
        {
            return FP_Utils::__exist(c, func);
        });
    }

    template<typename Func>
    static auto count(Func func)
    {
        return __make_func([func](const C &c)
        {
            return FP_Utils::__count(c, func);
        });
    }

    static auto count()
    {
        return __make_func([](const C &c)
        {
            return FP_Utils::__count(c);
        });
    }

    template<typename T>
    static auto find(T t)
    {
        return __make_func([t](const C &c)
        {
            return FP_Utils::__find(c, t);
        });
    }

    static auto index()
    {
        return __make_func([](const C &c, int index) {
            if (index == 0) return c.begin();
            if (index == -1) return c.end();
            return FP_Utils::__index(c, index);
        });
    }

    static auto isEndIterator()
    {
        return __make_func([](const C &c, typename C::const_iterator iter)
        {
            return iter == c.end();
        });
    }

    template<typename CollectionFrom = C>
    static auto section()
    {
        return __make_func([](typename CollectionFrom::const_iterator begin,
                              typename CollectionFrom::const_iterator end)
        {
            C ret;
            std::copy(begin, end, std::inserter(ret, ret.end()));
            return ret;
        });
    }

    static auto range()
    {
        return __make_func([](const C &c, int begin, int end) {
            assert(begin >= 0 && begin < static_cast<int>(c.size()));
            assert((end >= 0 && end < static_cast<int>(c.size())) || end == -1);

            auto iter_begin = FP_Utils::__index(c, begin);
            auto iter_end = FP_Utils::__index(c, end);

            C ret;
            std::copy(iter_begin, iter_end, std::inserter(ret, ret.end()));
            return ret;
        });
    }

    static auto stream(std::string separator, std::string head = "", std::string tail = "\n")
    {
        return __make_func([separator, head, tail](const C &c,
                           std::string msg,
                           std::ostream &output)
        {
            output << msg;
            output << head;
            copy(c.begin(),
                 c.end(),
                 infix_ostream_iterator<
                 typename C::value_type>(output, separator.c_str()));
            output << tail;
            return c;
        });
    }
};

template <bool Flag, class MaybeA, class MaybeB> class __IfElse;

template <class MaybeA, class MaybeB>
class __IfElse<true, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeA;
};

template <class MaybeA, class MaybeB>
class __IfElse<false, MaybeA, MaybeB>
{
public:
    using ResultType = MaybeB;
};

//惰性求值工具
template <typename T> class __lazy
{
private:
    T value_;
    std::function<T()> policy_;
    bool initialized_;
    static T default_policy()
    {
        throw std::runtime_error("No lazy initiation policy given.");
    }
public:
    __lazy() : policy_(default_policy), initialized_(false) {}
    __lazy(std::function<T()> _) : policy_(_), initialized_(false) {}
    __lazy(const __lazy<T>& _) : policy_(_.policy_), initialized_(false) {}

    __lazy<T>& operator=(const __lazy<T>& _)
    {
        policy_ = _.policy_;
        initialized_ = false;
        return *this;
    }

    T& get_value()
    {
        if (!initialized_)
        {
            value_ = policy_();
            initialized_ = true;
        }
        return value_;
    }

    T& operator()() {
        return get_value();
    }

    operator T() {
        return get_value();
    }
};

//FunctorBase函子 基础实现
template<typename T>
auto __undefined() {
    return std::optional<T>(std::nullopt);
}

template<typename Ret, typename T>
auto __pack(const T &val) {
    return std::optional<Ret>(val);
}

template<typename T>
class FunctorBase
{
public:
    void __c51de4f9a78a4f16b2484190083e85f6() {}
private:
    std::optional<T> _val;
protected:
    std::string _err_msg;
    bool _is_left;
public:
    template<typename ...Args>
    FunctorBase(Args... args) : _val(T(args...)), _is_left(false) { }
    FunctorBase(const T &val) : _val(T(val)), _is_left(false) { }
    FunctorBase(const std::optional<T> &val) : _is_left(false) {
        if (val) _val = val;
    }
    FunctorBase() : _val(std::nullopt), _err_msg(""), _is_left(false) {}
public:
    template<typename ...Args>
    static FunctorBase<T> of(Args... args)
    {
        return FunctorBase<T>(args...);
    }

    static FunctorBase<T> of(const T &val)
    {
        return FunctorBase<T>(val);
    }

    static FunctorBase<T> of(const std::optional<T> &val)
    {
        return FunctorBase<T>(val);
    }
public:
    static FunctorBase<T> of()
    {
        return FunctorBase<T>();
    }
    void setErrorMesssage(const std::string msg)
    {
        _is_left = true;
        _err_msg = msg;
    }
private:
    bool isNothing() {
        if (_val)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
public:
    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == true)>::type>
    auto map(const std::function<Ret(const std::optional<T1> &)> &func)
    {
        isNothing() ? false : (func(_val), true);
        return *this;
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<Ret(const std::optional<T1> &)> &func)
    {
        if (_is_left)
        {
            FunctorBase<Ret> ret = FunctorBase<Ret>::of();
            ret.setErrorMesssage(_err_msg);

            return ret;
        }
        else
        {
            return isNothing() ?
                   FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt)) :
                   FunctorBase<Ret>::of(func(_val));
        }
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<std::optional<Ret>(const std::optional<T1> &)> &func)
    {
        if (_is_left)
        {
            FunctorBase<Ret> ret = FunctorBase<Ret>::of();
            ret.setErrorMesssage(_err_msg);

            return ret;
        }

        if (isNothing())
        {
            return FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt));
        }
        else
        {
            auto ret = func(_val);
            if (ret)
            {
                return FunctorBase<Ret>::of(*ret);
            }
            else
            {
                return FunctorBase<Ret>::of(ret);
            }
        }
    }
public:
    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == true)>::type>
    auto map(const std::function<Ret(std::optional<T1>)> &func)
    {
        isNothing() ? false : (func(_val), true);
        return *this;
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<Ret(std::optional<T1>)> &func)
    {
        if (_is_left)
        {
            return FunctorBase<Ret>::of(_err_msg);
        }

        return isNothing() ?
               FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt)) :
               FunctorBase<Ret>::of(func(_val));
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<std::optional<Ret>(std::optional<T1>)> &func)
    {
        if (_is_left)
        {
            return FunctorBase<Ret>::of(_err_msg);
        }

        if (isNothing())
        {
            return FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt));
        }
        else
        {
            auto ret = func(_val);
            if (ret)
            {
                return FunctorBase<Ret>::of(*ret);
            }
            else
            {
                return FunctorBase<Ret>::of(ret);
            }
        }
    }
public:
    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == true)>::type>
    auto map(const std::function<Ret(T1)> &func)
    {
        isNothing() ? false : (func(*_val), true);
        return *this;
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<Ret(T1)> &func)
    {
        if (_is_left)
        {
            FunctorBase<Ret> ret = FunctorBase<Ret>::of();
            ret.setErrorMesssage(_err_msg);

            return ret;
        }

        return isNothing() ?
               FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt)) :
               FunctorBase<Ret>::of(func(*_val));
    }

    template<typename Ret, typename T1,
             typename = typename std::enable_if<
                 (std::is_same<Ret, void>::value == false)>::type,
             size_t = 0>
    auto map(const std::function<std::optional<Ret>(T1)> &func)
    {
        if (_is_left)
        {
            FunctorBase<Ret> ret = FunctorBase<Ret>::of();
            ret.setErrorMesssage(_err_msg);

            return ret;
        }

        if (isNothing())
        {
            return FunctorBase<Ret>::of(std::optional<Ret>(std::nullopt));
        }
        else
        {
            auto ret = func(*_val);
            if (ret)
            {
                return FunctorBase<Ret>::of(*ret);
            }
            else
            {
                return FunctorBase<Ret>::of(ret);
            }
        }
    }
public:
    template<typename Func>
    auto map(const Func &func)
    {
        return map(__make_func(func));
    }
public:
    template<typename Func>
    const FunctorBase<T> &check(Func func)
    {
        func(_val, _val ? true : false, _err_msg);
        return *this;
    }
public:
    const T &value() const {
        return *_val;
    }
    bool is_left()const {
        return _is_left;
    }
    const std::string &err_msg() const {
        return _err_msg;
    }
};

template<typename T> using Functor = FunctorBase<T>;
template<typename T> using MayBe = FunctorBase<T>;

template<typename T>
auto __Functor_of(const T &t)
{
    return Functor<T>::of(t);
}

template<typename T>
auto __MayBe_of(const T &t)
{
    return MayBe<T>::of(t);
}

//Either函子
namespace Either
{
template<typename T> using Right = FunctorBase<T>;

template<typename T>
auto __Right_of(const T &t)
{
    return Right<T>::of(t);
}

template<typename T>
class Left : public Right<T>
{
public:
    auto static of(const std::string &err_msg)
    {
        Right<T> ret = Right<T>::of();
        ret.setErrorMesssage(err_msg);

        return ret;
    }
};
}// namespace Either

template<typename T>
class Ap : public FunctorBase<T>
{
public:
    Ap(const T &t) : FunctorBase<T>(t) {}
private:
    template<typename T1>
    auto static of_(const T1 &t)
    {
        return Ap<T1>(t);
    }
public:
    auto static of(const T &t)
    {
        return Ap<T>(t);
    }
public:
    template<typename T1,
             typename = typename std::enable_if<(__is_invocable__<T, T1>::value ||
                     __is_invocable__<T, const T1&>::value ||
                     __is_invocable__<T, T1&&>::value)>::type>
    auto ap(const FunctorBase<T1> &functor)
    {
        return of_(this->value()(functor.value()));
    }
};

template<typename T>
auto __Ap_of(const T &t)
{
    return Ap<T>::of(t);
}

template<typename T>
class Monad : public FunctorBase<T>
{
public:
    Monad(const T &t) : FunctorBase<T>(t) {}
public:
    auto static of(const T &t)
    {
        return Monad<T>(t);
    }
public:
    template<typename Func>
    auto map(const Func &func)
    {
        auto ret = FunctorBase<T>::map(func).value();
        return Monad<decltype(ret)>::of(ret);
    }
public:
    template<typename Func,
             typename = typename std::enable_if<
                 __is_functor_base<typename __Ret<Func>::type>::value>::type>
    auto chain(const Func &func)
    {
        return this->map(func).value();
    }

    template<typename Func,
             typename = typename std::enable_if<
                 (__is_functor_base<typename __Ret<Func>::type>::value == false)>::type,
             size_t = 0>
    auto chain(const Func &func)
    {
        return this->map(func);
    }

    template<typename Func>
    auto flatMap(const Func &func)
    {
        return chain(func);
    }
};

template<typename T>
auto __Monad_of(const T& t)
{
    return Monad<T>::of(t);
}

template<typename T>
class IO : public Monad<T>
{
public:
    template<typename T1,
             typename = typename std::enable_if<__is_invocable__<T1>::value>::type>
    IO(const T1 &t) : Monad<T1>(t) {}
public:
    auto static of(const T &value)
    {
        auto wrap_func = [value]() {
            return value;
        };
        return IO<decltype(wrap_func)>(wrap_func);
    }
    auto static of(const std::function<T()> &func)
    {
        return IO<std::function<T()>>(func);
    }
public:
    template<typename Func>
    auto map(const Func &func)
    {
        auto ret = __compose(func, this->__parentValue());
        return IO<decltype(ret)>(ret);
    }
private:
    auto __parentValue()
    {
        Monad<T>* pParent = (Monad<T>*)this;
        return pParent->value();
    }
public:
    auto value()
    {
        return this->__parentValue()();
    }
};


template<typename T>
auto __IO_of(const std::function<T()>& func)
{
    return IO<T>::of(func);
}

template<typename T,
         typename = typename std::enable_if<(__is_invocable__<T>::value == false)>::type >
auto __IO_of(const T& t)
{
    return IO<T>::of(t);
}

template<typename T,
         typename = typename std::enable_if<__is_invocable__<T>::value>::type,
         size_t = 0>
auto __IO_of(const T& t)
{
    return __IO_of(__make_func(t));
}

#define DISPLAY(expr) do { cout << #expr" = " << (expr) << endl; } while (0);

} // namespace FP_Utils

#endif // _FP_UTILS_881c3d51de77412a9d3eddfd42056f52
