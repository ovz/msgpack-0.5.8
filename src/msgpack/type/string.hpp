//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2009 FURUHASHI Sadayuki
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#ifndef MSGPACK_TYPE_STRING_HPP__
#define MSGPACK_TYPE_STRING_HPP__

#include "msgpack/object.hpp"
#include <string>

namespace msgpack {

template <typename Elem>
inline std::basic_string<Elem>& operator>> (object o, std::basic_string<Elem>& v)
{
	if(o.type != type::RAW) { throw type_error(); }
	v.assign(reinterpret_cast<const Elem*>(o.via.raw.ptr), o.via.raw.size/sizeof(std::basic_string<Elem>::value_type));
	return v;
}

template <typename Stream, typename Elem>
inline packer<Stream>& operator<< (packer<Stream>& o, const std::basic_string<Elem>& v)
{
  auto size = v.size()*sizeof(std::basic_string<Elem>::value_type);
	o.pack_raw(size);
	o.pack_raw_body(reinterpret_cast<const char*>(v.data()), size);
	return o;
}
template <typename Elem>
inline void operator<< (object::with_zone& o, const std::basic_string<Elem>& v)
{
	o.type = type::RAW;
  auto size = v.size()*sizeof(std::basic_string<Elem>::value_type);
	char* ptr = (char*)o.zone->malloc(size);
	o.via.raw.ptr = ptr;
	o.via.raw.size = (uint32_t)size;
	memcpy(ptr, v.data(), size);
}
template <typename Elem>
inline void operator<< (object& o, const std::basic_string<Elem>& v)
{
	o.type = type::RAW;
	o.via.raw.ptr = v.data();
	o.via.raw.size = (uint32_t)(v.size()*sizeof(std::basic_string<Elem>::value_type));
}


}  // namespace msgpack

#endif /* msgpack/type/string.hpp */

