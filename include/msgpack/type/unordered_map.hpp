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

/// Had to define a macro to suppport unordered_map with custom hashers. 
/// Not all C++ compiler allow default template arguments for functions
#define MSGPACK_UNORDERED_MAP_WITH_STD_HASH(K, V, hasher_type) \
template <typename K, typename V> \
inline std::unordered_map<K, V, std::hash<hasher_type>> operator>> (object o, std::unordered_map<K, V, std::hash<hasher_type>>& v) \
{ \
	if(o.type != type::MAP) { throw type_error(); } \
	object_kv* p(o.via.map.ptr); \
	object_kv* const pend(o.via.map.ptr + o.via.map.size); \
	for(; p != pend; ++p) { \
		K key; \
		p->key.convert(&key); \
		p->val.convert(&v[key]); \
	} \
	return v; \
} \
 \
template <typename Stream, typename K, typename V> \
inline packer<Stream>& operator<< (packer<Stream>& o, const std::unordered_map<K,V,std::hash<hasher_type>>& v) \
{ \
	o.pack_map(v.size()); \
	for(typename std::unordered_map<K,V,std::hash<hasher_type>>::const_iterator it(v.begin()), it_end(v.end()); \
			it != it_end; ++it) { \
		o.pack(it->first); \
		o.pack(it->second); \
	} \
	return o; \
} \
 \
template <typename K, typename V> \
inline void operator<< (object::with_zone& o, const std::unordered_map<K,V,std::hash<hasher_type>>& v) \
{ \
	o.type = type::MAP; \
	if(v.empty()) { \
		o.via.map.ptr  = NULL; \
		o.via.map.size = 0; \
	} else { \
		object_kv* p = (object_kv*)o.zone->malloc(sizeof(object_kv)*v.size()); \
		object_kv* const pend = p + v.size(); \
		o.via.map.ptr  = p; \
		o.via.map.size = v.size(); \
		typename std::unordered_map<K,V,std::hash<hasher_type>>::const_iterator it(v.begin()); \
		do { \
			p->key = object(it->first, o.zone); \
			p->val = object(it->second, o.zone); \
			++p; \
			++it; \
		} while(p < pend); \
	} \
} \
 \
 \
template <typename K, typename V> \
inline std::unordered_multimap<K, V, std::hash<hasher_type>> operator>> (object o, std::unordered_multimap<K, V, std::hash<hasher_type>>& v) \
{ \
	if(o.type != type::MAP) { throw type_error(); } \
	object_kv* p(o.via.map.ptr); \
	object_kv* const pend(o.via.map.ptr + o.via.map.size); \
	for(; p != pend; ++p) { \
		std::pair<K, V> value; \
		p->key.convert(&value.first); \
		p->val.convert(&value.second); \
		v.insert(value); \
	} \
	return v; \
} \
 \
template <typename Stream, typename K, typename V> \
inline packer<Stream>& operator<< (packer<Stream>& o, const std::unordered_multimap<K,V>& v) \
{ \
	o.pack_map(v.size()); \
	for(typename std::unordered_multimap<K,V>::const_iterator it(v.begin()), it_end(v.end()); \
			it != it_end; ++it) { \
		o.pack(it->first); \
		o.pack(it->second); \
	} \
	return o; \
} \
 \
template <typename K, typename V> \
inline void operator<< (object::with_zone& o, const std::unordered_multimap<K,V>& v) \
{ \
	o.type = type::MAP; \
	if(v.empty()) { \
		o.via.map.ptr  = NULL; \
		o.via.map.size = 0; \
	} else { \
		object_kv* p = (object_kv*)o.zone->malloc(sizeof(object_kv)*v.size()); \
		object_kv* const pend = p + v.size(); \
		o.via.map.ptr  = p; \
		o.via.map.size = v.size(); \
		typename std::unordered_multimap<K,V>::const_iterator it(v.begin()); \
		do { \
			p->key = object(it->first, o.zone); \
			p->val = object(it->second, o.zone); \
			++p; \
			++it; \
		} while(p < pend); \
	} \
}

MSGPACK_UNORDERED_MAP_WITH_STD_HASH(K, V, K)

}  // namespace msgpack

#endif /* msgpack/type/map.hpp */

