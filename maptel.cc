#include "maptel.h"
#include <string>

using std::string;
using std::unordered_map;

using Maptel = unordered_map<string, string>;

Maptel maptel;

// Tworzy słownik i zwraca jego identyfikator id.
unsigned long maptel_create(void) {
	// TODO
}

// Usuwa słownik o identyfikatorze id.
void maptel_delete(unsigned long id) {
	// TODO
}

// Wstawia do słownika o identyfikatorze id informację
// o zmianie numeru z tel_src na tel_dst. Nadpisuje ewentualnie
// istniejącą informację o zmianie numeru tel_src.
void maptel_insert(unsigned long id,
	char const *tel_src,
	char const *tel_dst) {
	// TODO
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
