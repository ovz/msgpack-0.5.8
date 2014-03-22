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
#ifndef MSGPACK_TYPE_UNORDERED_MAP_HPP__
#define MSGPACK_TYPE_UNORDERED_MAP_HPP__

#include "msgpack/object.hpp"
#include <unordered_map>

namespace msgpack {

/// Had to rework unordered map support code into macros to support custom hashers.  
/// Not all C++ compiler allow default template arguments for functions

#define MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_UNPACK(K, V, H) \
inline std::unordered_map<K, V, H> operator>> (msgpack::object o, std::unordered_map<K, V, H>& v) \
{ \
	if(o.type != msgpack::type::MAP) { throw msgpack::type_error(); } \
	msgpack::object_kv* p(o.via.map.ptr); \
	msgpack::object_kv* const pend(o.via.map.ptr + o.via.map.size); \
	for(; p != pend; ++p) { \
		K key; \
		p->key.convert(&key); \
		p->val.convert(&v[key]); \
	} \
	return v; \
}

template <typename K, typename V> 
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_UNPACK(K, V, std::hash<K>)

#define MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK(K, V, H, typename_keyword) \
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::unordered_map<K, V, H>& v) \
{ \
	o.pack_map(v.size()); \
	for(typename_keyword std::unordered_map<K, V, H>::const_iterator it(v.begin()), it_end(v.end()); \
			it != it_end; ++it) { \
		o.pack(it->first); \
		o.pack(it->second); \
	} \
	return o; \
} 

template <typename Stream, typename K, typename V> 
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK(K, V, std::hash<K>, typename)

#define MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK_OBJECT(K, V, H, typename_keyword) \
inline void operator<< (msgpack::object::with_zone& o, const std::unordered_map<K, V, H>& v) \
{ \
	o.type = msgpack::type::MAP; \
	if(v.empty()) { \
		o.via.map.ptr  = NULL; \
		o.via.map.size = 0; \
	} else { \
		msgpack::object_kv* p = (msgpack::object_kv*)o.zone->malloc(sizeof(msgpack::object_kv)*v.size()); \
		msgpack::object_kv* const pend = p + v.size(); \
		o.via.map.ptr  = p; \
		o.via.map.size = v.size(); \
		typename_keyword std::unordered_map<K, V, H>::const_iterator it(v.begin()); \
		do { \
			p->key = msgpack::object(it->first, o.zone); \
			p->val = msgpack::object(it->second, o.zone); \
			++p; \
			++it; \
		} while(p < pend); \
	} \
} 

template <typename K, typename V> 
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK_OBJECT(K, V, std::hash<K>, typename)

#define MSGPACK_UNORDERED_MAP_WITH_HASH_NO_TYPENAME
#define MSGPACK_UNORDERED_MAP_WITH_HASH(K, V, H) \
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_UNPACK(K, V, H) \
template <typename Stream> \
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK(K, V, H, MSGPACK_UNORDERED_MAP_WITH_HASH_NO_TYPENAME) \
MSGPACK_UNORDERED_MAP_CONCRETE_OPERATOR_PACK_OBJECT(K, V, H, MSGPACK_UNORDERED_MAP_WITH_HASH_NO_TYPENAME) 


// Original implementation of multimap

template <typename K, typename V>
inline std::unordered_multimap<K, V> operator>> (msgpack::object o, std::unordered_multimap<K, V>& v)
{
	if(o.type != msgpack::type::MAP) { throw msgpack::type_error(); }
	msgpack::object_kv* p(o.via.map.ptr);
	msgpack::object_kv* const pend(o.via.map.ptr + o.via.map.size);
	for(; p != pend; ++p) {
		std::pair<K, V> value;
		p->key.convert(&value.first);
		p->val.convert(&value.second);
		v.insert(value);
	}
	return v;
}

template <typename Stream, typename K, typename V>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::unordered_multimap<K,V>& v)
{
	o.pack_map(v.size());
	for(typename std::unordered_multimap<K,V>::const_iterator it(v.begin()), it_end(v.end());
			it != it_end; ++it) {
		o.pack(it->first);
		o.pack(it->second);
	}
	return o;
}

template <typename K, typename V>
inline void operator<< (msgpack::object::with_zone& o, const std::unordered_multimap<K,V>& v)
{
	o.type = msgpack::type::MAP;
	if(v.empty()) {
		o.via.map.ptr  = NULL;
		o.via.map.size = 0;
	} else {
		msgpack::object_kv* p = (msgpack::object_kv*)o.zone->malloc(sizeof(msgpack::object_kv)*v.size());
		msgpack::object_kv* const pend = p + v.size();
		o.via.map.ptr  = p;
		o.via.map.size = v.size();
		typename std::unordered_multimap<K,V>::const_iterator it(v.begin());
		do {
			p->key = msgpack::object(it->first, o.zone);
			p->val = msgpack::object(it->second, o.zone);
			++p;
			++it;
		} while(p < pend);
	}
}

}  // namespace msgpack

#endif /* msgpack/type/map.hpp */

