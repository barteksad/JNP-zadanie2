#include "maptel.h"
#include <string>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <regex>
#include <cstring>
#include <set>

#ifdef NDEBUG
const bool debug = false;
#else
const bool debug = true;
#endif


using std::string;
using std::unordered_map;
using std::cerr;
using std::endl;
using std::string;
using std::set;
using jnp1::TEL_NUM_MAX_LEN;

using maptel_t = unordered_map<string, string>;
using maptel_id_t = unsigned long;

namespace {
	unordered_map<maptel_id_t, maptel_t> all_maptels;
	unordered_map<maptel_id_t, bool> is_id_taken;
	maptel_id_t last_free_id = 0;
	
	bool is_tel_correct(char const *tel) {
		if (tel == NULL)
			return false;

		std::regex number("^[0-9]*$");
		if (!regex_match(tel, number))
			return false;

		size_t last_digit = 0;
		while (tel[last_digit] != '\0' && last_digit < TEL_NUM_MAX_LEN)
			last_digit++;
		
		return (tel[last_digit] == '\0');
	}
}

namespace jnp1 {

	// Tworzy slownik i zwraca jego identyfikator id.
	unsigned long maptel_create(void) {
		if (debug) {
			cerr << "maptel: maptel_create()\n";
		}
		
		while (is_id_taken[last_free_id]) {
			++last_free_id;
		}
		
		is_id_taken[last_free_id] = true;
		all_maptels[last_free_id] = maptel_t();
		
		if (debug) {
			cerr << "maptel: maptel_create: new map id = " << last_free_id << endl;
		}
		
		return last_free_id++;
	}

	// Usuwa slownik o identyfikatorze id.
	void maptel_delete(unsigned long id) {
		if (debug) {
			assert(is_id_taken[id]);

			cerr << "maptel: maptel_delete(" << id << ")\n";
		}
		

		is_id_taken.erase(id);
		all_maptels.erase(id);
		
		if (debug) {
			cerr << "maptel: maptel_delete: map " << id << " deleted\n";
		}
	}

	// Wstawia do slownika o identyfikatorze id informacje
	// o zmianie numeru z tel_src na tel_dst. Nadpisuje ewentualnie
	// istniejace informacje o zmianie numeru tel_src.
	void maptel_insert(unsigned long id,
		char const *tel_src,
		char const *tel_dst) {
		if (debug) {
			assert(is_id_taken[id]);
			assert(is_tel_correct(tel_src));
			assert(is_tel_correct(tel_dst));

			cerr << "maptel: maptel_insert(" << id << ", " 
				<< tel_src << ", " << tel_dst << ")\n";
		}
		
		all_maptels[id][string(tel_src)] = string(tel_dst);
		
		if (debug) {
			cerr << "maptel: maptel_insert: inserted\n";
		}
	}

	// Usuwa ze slownika o identyfikatorze id informacje o zmianie
	// numeru tel_src, o ile taka istnieje.
	void maptel_erase(unsigned long id, char const *tel_src) {
		if (debug) {
			assert(is_id_taken[id]);
			assert(is_tel_correct(tel_src));
			
			cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")\n";
		}

		unordered_map<maptel_id_t, maptel_t>::iterator found_maptel = 
			all_maptels.find(id);

		if (found_maptel != all_maptels.end()) {
			string src_number(tel_src);
			size_t if_erased = (*found_maptel).second.erase(src_number);

			if (debug) {
				if (if_erased == 0)
					cerr << "maptel: maptel_erase: nothing to erase\n";
				else
					cerr << "maptel: maptel_erase: erased\n";
			}
		}
	}

	// Podaza ciagiem kolejnych zmian numerow przechowywanych
	// w slowniku o identyfikatorze id, poczawszy od tel_src. Ostatni
	// numer znaleziony w tym ciagu zapisuje w tel_dst.
	// Jesli nie ma zmiany numeru lub zmiany tworzy cykl, to w tel_dst
	// zapisywany jest numer tel_src.
	void maptel_transform(unsigned long id,
		char const *tel_src,
		char *tel_dst,
		size_t len) {

		if (debug) {
			assert(is_id_taken[id]);
			assert(is_tel_correct(tel_src));
			//assert(is_tel_correct(tel_dst));

			cerr << "maptel: maptel_transform(" 
				<< id << ", "
				<< tel_src << ", " 
				<< (void *) tel_dst << ", "
				<< len << ")\n";
		}

		string src_number(tel_src);
		string dst_number(tel_dst);

		unordered_map<maptel_id_t, maptel_t>::iterator found_maptel = 
			all_maptels.find(id);
		if (found_maptel == all_maptels.end())
			return;

		maptel_t id_maptel = (*found_maptel).second;
		maptel_t::iterator walk1 = id_maptel.find(src_number), walk2;
		bool write_src_to_dst = walk1 == id_maptel.end();
		set<string> seen_numbers;

		if (!write_src_to_dst)
			while (true) {
				walk2 = id_maptel.find((*walk1).second);

				if (walk2 == id_maptel.end())
					break;
				else {
					if (!seen_numbers.insert((*walk2).second).second) {
						write_src_to_dst = true;

						if (debug) {
							cerr << "maptel: maptel_transform: cycle detected\n";
						}
						break;
					}
					else
						walk1 = walk2;
				}
			}

		string number_to_copy = write_src_to_dst ? src_number : (*walk1).second;

		if (debug) {
			assert(number_to_copy.size() + 1 <= len);
		}
			
		strcpy(tel_dst, number_to_copy.c_str());

		if (debug) {
			cerr << "maptel: maptel_transform: " << src_number << " -> " 
				<< number_to_copy <<"\n";
		}
	}

}
