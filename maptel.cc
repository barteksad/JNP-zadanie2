#include "maptel.h"
#include <string>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <regex>

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

using maptel_t = unordered_map<string, string>;
using id_t = unsigned long;

namespace {
	unordered_map<id_t, maptel_t> all_maptels;
	unordered_map<id_t, bool> is_id_taken;
	id_t last_free_id = 0;
	
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
	// TODO
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
	// TODO
}
