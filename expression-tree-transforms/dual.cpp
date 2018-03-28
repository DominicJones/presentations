// mp_reversed_map_from_list
template<class L, class S> struct mp_reversed_map_from_list_impl;

template<template<class...> class L, class... T, std::size_t... J>
struct mp_reversed_map_from_list_impl<L<T...>, std::integer_sequence<std::size_t, J...> >
{
  using type = mp_list<mp_list<T, mp_size_t<J> >...>;
};

template<class L> using mp_reversed_map_from_list =
  typename mp_reversed_map_from_list_impl<L, std::make_index_sequence<mp_size<L>::value> >::type;




// mp_dual
template<class L, class S, std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl;

template<template<class...> class L,
         template<class SX, SX...> class S, class ST, ST... SVs,
         std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl<L<>, S<ST, SVs...>, I, NI, NJ>
{
  using type = mp_list<>;
};

template<template<class...> class L,
         template<class SX, SX...> class S, class ST, ST SV, ST... SVs,
         std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl<L<>, S<ST, SV, SVs...>, I, NI, NJ>
{
  using _head = std::integral_constant<std::size_t, NJ>;
  using _tail = typename mp_dual_impl<L<>, S<ST, SVs...>, I + 1, NI, NJ>::type;
  using type = mp_append<mp_list<_head>, _tail>;
};


template<class L, class S, std::size_t I, std::size_t NI, std::size_t NJ, std::size_t J>
struct mp_dual_impl_valid;

template<template<class...> class L, class T, class... Ts,
         template<class _SX, _SX...> class S, class ST, ST SV, ST... SVs,
         std::size_t I, std::size_t NI, std::size_t NJ, std::size_t J>
struct mp_dual_impl_valid<L<T, Ts...>, S<ST, SV, SVs...>, I, NI, NJ, J>
{
  auto static constexpr JD = (I == J)? mp_second<T>::value: NJ;
  using _head = std::integral_constant<std::size_t, JD>;

  using _tail = typename mp_if_c<bool(I == J),
                                 mp_dual_impl<L<Ts...>, S<ST, SVs...>, I + 1, NI, NJ>,
                                 mp_dual_impl<L<T, Ts...>, S<ST, SVs...>, I + 1, NI, NJ>
                                 >::type;

  using type = mp_append<mp_list<_head>, _tail>;
};


template<class L, class S, std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl_invalid;

template<template<class...> class L, class T, class... Ts,
         template<class _SX, _SX...> class S, class ST, ST SV, ST... SVs,
         std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl_invalid<L<T, Ts...>, S<ST, SV, SVs...>, I, NI, NJ>
{
  using _head = std::integral_constant<std::size_t, NJ>;
  using _tail = typename mp_dual_impl<L<Ts...>, S<ST, SVs...>, I + 1, NI, NJ>::type;
  using type = mp_append<mp_list<_head>, _tail>;
};


template<template<class...> class L, class T, class... Ts,
         template<class _SX, _SX...> class S, class ST, ST SV, ST... SVs,
         std::size_t I, std::size_t NI, std::size_t NJ>
struct mp_dual_impl<L<T, Ts...>, S<ST, SV, SVs...>, I, NI, NJ>
{
  auto static constexpr J = mp_first<T>::value;

  using type = typename mp_if_c<bool(J < NI),
    mp_dual_impl_valid<L<T, Ts...>, S<ST, SV, SVs...>, I, NI, NJ, J>,
    mp_dual_impl_invalid<L<T, Ts...>, S<ST, SV, SVs...>, I, NI, NJ>
    >::type;
};

template<typename L, std::size_t NI>
struct mp_dual_entry
{
  using IS = std::make_index_sequence<NI>;
  auto static constexpr NJ = mp_size<L>::value;
  using M = mp_reversed_map_from_list<L>;
  using MS = brigand::sort<M, brigand::bind<mp_map_incr, brigand::_1, brigand::_2> >;
  using D = mp_dual_impl<MS, IS, 0, NI, NJ>;
  using type = typename D::type;
};

template<class L, std::size_t NI> using mp_dual = typename mp_dual_entry<L, NI>::type;
