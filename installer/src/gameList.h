
#ifndef _GAMELIST_H_
#define _GAMELIST_H_

#include "zipList.h"

typedef struct _gList_t {
	uint32_t tid;
	char name[64];
	u8 *romPtr;
	u32 *romSizePtr;
} gList_t;

gList_t GameList[] = {
	{ 0x10179A00, "Brain Age [JPN]", brainage_zip, &brainage_zip_size },
	{ 0x10179B00, "Brain Age [USA]", brainage_zip, &brainage_zip_size },
	{ 0x10179C00, "Brain Training [PAL]", brainage_zip, &brainage_zip_size },

	{ 0x10179D00, "Yoshi Touch & Go [JPN]", yoshitouchandgo_zip, &yoshitouchandgo_zip_size },
	{ 0x10179E00, "Yoshi Touch & Go [USA]", yoshitouchandgo_zip, &yoshitouchandgo_zip_size },
	{ 0x10179F00, "Yoshi Touch & Go [PAL]", yoshitouchandgo_zip, &yoshitouchandgo_zip_size },

	{ 0x10195600, "Mario Kart DS [JPN]", mariokartds_zip, &mariokartds_zip_size },
	{ 0x10195700, "Mario Kart DS [USA]", mariokartds_zip, &mariokartds_zip_size },
	{ 0x10195800, "Mario Kart DS [PAL]", mariokartds_zip, &mariokartds_zip_size },

	{ 0x10195900, "New Super Mario Bros. [JPN]", newsmb_zip, &newsmb_zip_size },
	{ 0x10195A00, "New Super Mario Bros. [USA]", newsmb_zip, &newsmb_zip_size },
	{ 0x10195B00, "New Super Mario Bros. [PAL]", newsmb_eur_zip, &newsmb_eur_zip_size },

	{ 0x10198800, "Yoshi's Island DS [JPN]", yoshids_zip, &yoshids_zip_size },
	{ 0x10198900, "Yoshi's Island DS [USA]", yoshids_zip, &yoshids_zip_size },
	{ 0x10198A00, "Yoshi's Island DS [PAL]", yoshids_zip, &yoshids_zip_size },

	{ 0x10198B00, "Big Brain Academy [JPN]", bigbrainacademy_zip, &bigbrainacademy_zip_size },
	{ 0x10198C00, "Big Brain Academy [USA]", bigbrainacademy_zip, &bigbrainacademy_zip_size },
	{ 0x10198D00, "Big Brain Academy [PAL]", bigbrainacademy_zip, &bigbrainacademy_zip_size },

	{ 0x101A1E00, "WarioWare: Touched [JPN]", wwtouched_zip, &wwtouched_zip_size },
	{ 0x101A1F00, "WarioWare: Touched [USA]", wwtouched_zip, &wwtouched_zip_size },
	{ 0x101A2000, "WarioWare: Touched [PAL]", wwtouched_zip, &wwtouched_zip_size },

	{ 0x101A2100, "Mario and Luigi: Partners in Time [JPN]", partnersintime_zip, &partnersintime_zip_size },
	{ 0x101A2200, "Mario and Luigi: Partners in Time [USA]", partnersintime_zip, &partnersintime_zip_size },
	{ 0x101A2300, "Mario and Luigi: Partners in Time [PAL]", partnersintime_zip, &partnersintime_zip_size },

	{ 0x101A5200, "DK Jungle Climber [JPN]", dkjclimber_zip, &dkjclimber_zip_size },
	{ 0x101A5300, "DK Jungle Climber [USA]", dkjclimber_zip, &dkjclimber_zip_size },
	{ 0x101A5400, "DK Jungle Climber [PAL]", dkjclimber_zip, &dkjclimber_zip_size },

	{ 0x101A5500, "Kirby Squeak Squad [JPN]", kirby_zip, &kirby_zip_size },
	{ 0x101A5600, "Kirby Squeak Squad [USA]", kirby_zip, &kirby_zip_size },
	{ 0x101A5700, "Kirby Mouse Attack [PAL]", kirby_zip, &kirby_zip_size },

	{ 0x101ABD00, "Wario Master of Disguise [JPN]", masterofdisguise_zip, &masterofdisguise_zip_size },
	{ 0x101ABE00, "Wario Master of Disguise [USA]", masterofdisguise_zip, &masterofdisguise_zip_size },
	{ 0x101ABF00, "Wario Master of Disguise [PAL]", masterofdisguise_zip, &masterofdisguise_zip_size },

	{ 0x101AC000, "Star Fox Command [JPN]", sfcommand_zip, &sfcommand_zip_size },
	{ 0x101AC100, "Star Fox Command [USA]", sfcommand_zip, &sfcommand_zip_size },
	{ 0x101AC200, "Star Fox Command [PAL]", sfcommand_zip, &sfcommand_zip_size },

	{ 0x101B8800, "Kirby Canvas Curse [JPN]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },
	{ 0x101B8900, "Kirby Canvas Curse [USA]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },
	{ 0x101B8A00, "Kirby Power Paintbrush [PAL]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },

	{ 0x101B8B00, "Zelda Spirit Tracks [JPN]", zeldast_zip, &zeldast_zip_size },
	{ 0x101B8C00, "Zelda Spirit Tracks [USA]", zeldast_zip, &zeldast_zip_size },
	{ 0x101B8D00, "Zelda Spirit Tracks [PAL]", zeldast_zip, &zeldast_zip_size },

	{ 0x101C3300, "Super Mario 64 DS [JPN]", sm64ds_zip, &sm64ds_zip_size },
	{ 0x101C3400, "Super Mario 64 DS [USA]", sm64ds_zip, &sm64ds_zip_size },
	{ 0x101C3500, "Super Mario 64 DS [PAL]", sm64ds_zip, &sm64ds_zip_size },

	{ 0x101C3600, "Zelda Phantom Hourglass [JPN]", zeldaph_zip, &zeldaph_zip_size },
	{ 0x101C3700, "Zelda Phantom Hourglass [USA]", zeldaph_zip, &zeldaph_zip_size },
	{ 0x101C3800, "Zelda Phantom Hourglass [PAL]", zeldaph_zip, &zeldaph_zip_size },

	{ 0x101C8600, "Kirby Mass Attack [JPN]", kirbymassattack_zip, &kirbymassattack_zip_size },
	{ 0x101C8700, "Kirby Mass Attack [USA]", kirbymassattack_zip, &kirbymassattack_zip_size },
	{ 0x101C8800, "Kirby Mass Attack [PAL]", kirbymassattack_zip, &kirbymassattack_zip_size },

	{ 0x101E0F00, "Pokemon Ranger: Shadows of Almia [JPN]", shadowsofalmia_zip, &shadowsofalmia_zip_size },
	{ 0x101E1000, "Pokemon Ranger: Shadows of Almia [USA]", shadowsofalmia_zip, &shadowsofalmia_zip_size },
	{ 0x101E1100, "Pokemon Ranger: Shadows of Almia [PAL]", shadowsofalmia_zip, &shadowsofalmia_zip_size },
};

static const int GameListSize = sizeof(GameList) / sizeof(gList_t);

#endif
