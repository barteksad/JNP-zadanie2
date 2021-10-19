#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include "stddef.h"

const size_t TEL_NUM_MAX_LEN = 22;

// Tworzy słownik i zwraca jego identyfikator id.
extern unsigned long maptel_create(void);
// Usuwa słownik o identyfikatorze id.
extern void maptel_delete(unsigned long id);
// Wstawia do słownika o identyfikatorze id informację
// o zmianie numeru z tel_src na tel_dst. Nadpisuje ewentualnie
// istniejącą informację o zmianie numeru tel_src.
extern void maptel_insert(unsigned long id,
	char const *tel_src,
	char const *tel_dst);
// Usuwa ze słownika o identyfikatorze id informację o zmianie
// numeru tel_src, o ile taka istnieje.
extern void maptel_erase(unsigned long id, char const *tel_src);
// Podąża ciągiem kolejnych zmian numerów przechowywanych
// w słowniku o identyfikatorze id, począwszy od tel_src. Ostatni
// numer znaleziony w tym ciągu zapisuje w tel_dst.
// Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to w tel_dst
// zapisywany jest numer tel_src.
extern void maptel_transform(unsigned long id,
	char const *tel_src,
	char *tel_dst,
	size_t len);

#ifdef __cplusplus
}
#endif