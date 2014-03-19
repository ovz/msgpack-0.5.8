#include <msgpack.hpp>
#include <gtest/gtest.h>
#include <sstream>


using namespace std;

struct myclass {
	myclass() : num(0), str("default") { }

	myclass(int num, const std::string& str) :
		num(num), str(str) { }

	~myclass() { }

	int num;
	std::string str;
	MSGPACK_DEFINE(num, str);
};

TEST(sharedptr, pack_myclass)
{
	msgpack::sbuffer sbuf;
  auto ptr(make_shared<myclass>(1, "msgpack")); 

	msgpack::pack(sbuf, ptr);
}


TEST(sharedptr, unpack_myclasss)
{
	msgpack::sbuffer sbuf;
  auto m1_ptr(make_shared<myclass>(1, "phraser")); 
	msgpack::pack(sbuf, m1_ptr);

	msgpack::zone z;
	msgpack::object obj;

	msgpack::unpack_return ret =
		msgpack::unpack(sbuf.data(), sbuf.size(), NULL, &z, &obj);

	EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

	shared_ptr<myclass> m2_ptr = obj.as<decltype(m2_ptr)>();
	EXPECT_EQ(m1_ptr->num, m2_ptr->num);
	EXPECT_EQ(m1_ptr->str, m2_ptr->str);
}

