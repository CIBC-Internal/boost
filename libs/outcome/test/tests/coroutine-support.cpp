/* Unit testing for outcomes
(C) 2013-2019 Niall Douglas <http://www.nedproductions.biz/> (6 commits)


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#if defined(__cpp_coroutines)

#include <boost/outcome/coroutine_support.hpp>
#include <boost/outcome/outcome.hpp>
#include <boost/outcome/try.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>

namespace coroutines
{
  template <class T> using eager = BOOST_OUTCOME_V2_NAMESPACE::awaitables::eager<T>;
  template <class T> using lazy = BOOST_OUTCOME_V2_NAMESPACE::awaitables::lazy<T>;
  template <class T, class E = boost::system::error_code> using result = BOOST_OUTCOME_V2_NAMESPACE::result<T, E>;

  inline eager<result<int>> eager_int(int x) { co_return x + 1; }
  inline lazy<result<int>> lazy_int(int x) { co_return x + 1; }
  inline eager<result<int>> eager_error() { co_return boost::system::errc::not_enough_memory; }
  inline lazy<result<int>> lazy_error() { co_return boost::system::errc::not_enough_memory; }
  inline eager<result<void>> eager_void() { co_return boost::system::errc::not_enough_memory; }
  inline lazy<result<void>> lazy_void() { co_return boost::system::errc::not_enough_memory; }

  template <class U, class... Args> inline eager<result<std::string>> eager_coawait(U &&f, Args... args)
  {
    BOOST_OUTCOME_CO_TRY(co_await f(args...));
    co_return "hi";
  }
  template <class U, class... Args> inline lazy<result<std::string>> lazy_coawait(U &&f, Args... args)
  {
    BOOST_OUTCOME_CO_TRY(co_await f(args...));
    co_return "hi";
  }

#ifndef BOOST_NO_EXCEPTIONS
  struct custom_exception_type
  {
  };
  inline lazy<result<int, boost::exception_ptr>> result_exception(boost::exception_ptr e)
  {
    boost::rethrow_exception(e);
    co_return 5;
  }

  inline lazy<BOOST_OUTCOME_V2_NAMESPACE::outcome<int>> outcome_exception(boost::exception_ptr e)
  {
    boost::rethrow_exception(e);
    co_return 5;
  }
#endif

  inline eager<int> eager_int2(int x) { co_return x + 1; }
  inline lazy<int> lazy_int2(int x) { co_return x + 1; }
  inline eager<void> eager_void2() { co_return; }
  inline lazy<void> lazy_void2() { co_return; }
}  // namespace coroutines

BOOST_OUTCOME_AUTO_TEST_CASE(works_result_coroutine, "Tests that results are eager and lazy awaitable")
{
  using namespace coroutines;
  auto eager_await = [](auto t) { return t.await_resume(); };
  auto lazy_await = [](auto t) {
    t.await_suspend({});
    return t.await_resume();
  };

  BOOST_CHECK(eager_await(eager_int(5)).value() == 6);
  BOOST_CHECK(lazy_await(lazy_int(5)).value() == 6);
  BOOST_CHECK(eager_await(eager_error()).error() == boost::system::errc::not_enough_memory);
  BOOST_CHECK(lazy_await(lazy_error()).error() == boost::system::errc::not_enough_memory);
  BOOST_CHECK(eager_await(eager_void()).error() == boost::system::errc::not_enough_memory);
  BOOST_CHECK(lazy_await(lazy_void()).error() == boost::system::errc::not_enough_memory);

  BOOST_CHECK(eager_await(eager_coawait(eager_int, 5)).value() == "hi");
  BOOST_CHECK(lazy_await(lazy_coawait(lazy_int, 5)).value() == "hi");

#ifndef BOOST_NO_EXCEPTIONS
  auto e = boost::copy_exception(custom_exception_type());
  BOOST_CHECK_THROW(lazy_await(result_exception(e)).value(), custom_exception_type);
  BOOST_CHECK_THROW(lazy_await(outcome_exception(e)).value(), custom_exception_type);
#endif

  BOOST_CHECK(eager_await(eager_int2(5)) == 6);
  BOOST_CHECK(lazy_await(lazy_int2(5)) == 6);
  eager_await(eager_void2());
  lazy_await(lazy_void2());
}
#else
int main(void)
{
  return 0;
}
#endif