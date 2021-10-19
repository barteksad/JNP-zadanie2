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

namespace jnp1 {

using maptel_t = unordered_map<string, string>;
using maptel_id_t = unsigned long;

namespace {
	unordered_map<maptel_id_t, maptel_t> all_maptels;
	unordered_map<maptel_id_t, bool> is_id_taken;
	maptel_id_t last_free_id = 0;
	
	bool is_tel_correct(char const *tel) {
		std::regex number("^[0-9]*$");
		return regex_match(tel, number);
	}
}


// Tworzy słownik i zwraca jego identyfikator id.
unsigned long maptel_create(void) {
	if(debug) {
		cerr << "maptel: maptel_create()\n";
	}
	
	while(is_id_taken[last_free_id]) {
		++last_free_id;
	}
	
	is_id_taken[last_free_id] = true;
	all_maptels[last_free_id] = maptel_t();
	
	if(debug) {
		cerr << "maptel: maptel_create: new map id = " << last_free_id << endl;
	}
	
	return last_free_id++;
}

// Usuwa słownik o identyfikatorze id.
void maptel_delete(unsigned long id) {
	if(debug) {
		cerr << "maptel: maptel_delete(" << id << ")\n";
	}
	
	if(is_id_taken[id]) {
		is_id_taken.erase(id);
		all_maptels.erase(id);
		
		if(debug) {
			cerr << "maptel: maptel_delete: map " << id << " deleted";
		}
	}
	else {
		is_id_taken.erase(id);
		if(debug) {
			cerr << "maptel: maptel_delete: nothing to delete\n";
		}
	}
}

// Wstawia do słownika o identyfikatorze id informację
// o zmianie numeru z tel_src na tel_dst. Nadpisuje ewentualnie
// istniejącą informację o zmianie numeru tel_src.
void maptel_insert(unsigned long id,
	char const *tel_src,
	char const *tel_dst) {
	if(debug) {
		cerr << "maptel: maptel_insert(" << id << ", " 
			<< tel_src << ", " << tel_dst << ")\n";
	}
	
	if(!is_id_taken[id]) {
		is_id_taken.erase(id);
		
		if(debug) {
			cerr << "maptel: map id = " << id << " does not exist\n";
		}
		
		return;
	}
	
	if(!is_tel_correct(tel_src)) {
		if(debug) {
			cerr << "maptel: tes_src is not a number\n";
		}
		return;
	}
	if(!is_tel_correct(tel_dst)) {
		if(debug) {
			cerr << "maptel: tes_dst is not a number\n";
		}
		return;
	}
	
	all_maptels[id][string(tel_src)] = string(tel_dst);
	
	if(debug) {
		cerr << "maptel: maptel_insert: inserted\n";
	}
}

// Usuwa ze słownika o identyfikatorze id informację o zmianie
// numeru tel_src, o ile taka istnieje.
void maptel_erase(unsigned long id, char const *tel_src) {
	if (debug) {
		cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")\n";
	}

	bool nothing_to_erase = false;
	unordered_map<maptel_id_t, maptel_t>::iterator found_maptel = all_maptels.find(id);

	if (found_maptel != all_maptels.end()) {
		string src_number(tel_src);
		size_t if_erased = (*found_maptel).second.erase(src_number);

		if (if_erased == 0)
			nothing_to_erase = true;
	}
	else
		nothing_to_erase = true;

	if (nothing_to_erase) {
		if (debug) 
			cerr << "maptel: maptel_erase: nothing to erase\n";
	}
}

// Podąża ciągiem kolejnych zmian numerów przechowywanych
// w słowniku o identyfikatorze id, począwszy od tel_src. Ostatni
// numer znaleziony w tym ciągu zapisuje w tel_dst.
// Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to w tel_dst
// zapisywany jest numer tel_src.
void maptel_transform(unsigned long id,
	char const *tel_src,
	char *tel_dst,
	size_t len) {

	if (debug) {
		cerr << "maptel: maptel_transform(" 
			<< id << ", "
			<< tel_src << ", " 
			<< (void *) tel_dst << ", "
			<< len << ")\n";
	}

	string src_number(tel_src);
	string dst_number(tel_dst);

	unordered_map<maptel_id_t, maptel_t>::iterator found_maptel = all_maptels.find(id);
	if(found_maptel == all_maptels.end())
		return;

	maptel_t id_maptel = (*found_maptel).second;
	maptel_t::iterator walk1 = id_maptel.find(src_number), walk2;
	bool write_src_to_dst = walk1 == id_maptel.end();
	set<string> seen_numbers;

	if (!write_src_to_dst)
		while(true) {
			walk2 = id_maptel.find((*walk1).second);

			if (walk2 == id_maptel.end())
				break;
			else {
				if (!seen_numbers.insert((*walk2).second).second) {
					write_src_to_dst = true;
					break;
				}
				else
					walk1 = walk2;
			}
		}

	string number_to_copy = write_src_to_dst ? src_number : (*walk1).second;

	// if (len < number_to_copy.length() + 1)
	// 	realloc(tel_dst, number_to_copy.length() + 1);
		
	strcpy(tel_dst, number_to_copy.c_str());

	if (debug) {
		cerr << "maptel: maptel_transform: " << src_number << " -> " << number_to_copy <<"\n";
	}
}

}
