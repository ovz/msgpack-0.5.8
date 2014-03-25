#ifndef MSGPACK_TYPE_SHARED_PTR_HPP__
#define MSGPACK_TYPE_SHARED_PTR_HPP__

#include "msgpack/object.hpp"
#include <memory>

namespace msgpack {


template <typename T>
inline std::shared_ptr<T>& operator>> (object o, std::shared_ptr<T>& v)
{
	if(o.type != type::ARRAY) { throw type_error(); }
  if (o.via.array.size > 1) { throw type_error(); }
  if(o.via.array.size > 0) {
    object* p = o.via.array.ptr;
    v = std::make_shared<T>();
    p->convert(v.get());
  }
	return v;
}

template <typename Stream, typename T>
inline packer<Stream>& operator<< (packer<Stream>& o, const std::shared_ptr<T>& v)
{
  if (v != nullptr) {
    o.pack_array(1);
    o.pack(*v);
  }
  else {
    o.pack_array(0);
  }
	return o;
}

template <typename T>
inline void operator<< (object::with_zone& o, const std::shared_ptr<T>& v)
{
	o.type = type::ARRAY;
	if(nullptr == v) {
		o.via.array.ptr = NULL;
		o.via.array.size = 0;
	} else {
		object* p = (object*)o.zone->malloc(sizeof(object));
		o.via.array.ptr = p;
		o.via.array.size = 1;
    *p = object(*v, o.zone);
	}
}


}  // namespace msgpack

#endif /* msgpack/type/string.hpp */

