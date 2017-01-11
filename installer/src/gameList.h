
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
	{ 0x10179A00, "Kawashima: Motto Nou wo Kitaeru Otona no DS Training [JPN]", brainage_zip, &brainage_zip_size },
	{ 0x10179B00, "Brain Age: Train Your Brain in Minutes a Day! [USA]", brainage_zip, &brainage_zip_size },
	{ 0x10179C00, "Dr. Kawashima's Brain Training [PAL]", brainage_zip, &brainage_zip_size },

	{ 0x10179D00, "Catch! Touch! Yoshi! [JPN]", yoshitouchandgo_zip, &yoshitouchandgo_zip_size },
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

	{ 0x10198B00, "Yawaraka Atama Juku [JPN]", bigbrainacademy_zip, &bigbrainacademy_zip_size },
	{ 0x10198C00, "Big Brain Academy [USA]", bigbrainacademy_zip, &bigbrainacademy_zip_size },
	{ 0x10198D00, "Big Brain Academy [PAL]", bigbrainacademy_zip, &bigbrainacademy_zip_size },

	{ 0x101A1E00, "Sawaru: Made in Wario [JPN]", wwtouched_zip, &wwtouched_zip_size },
	{ 0x101A1F00, "WarioWare: Touched! [USA]", wwtouched_zip, &wwtouched_zip_size },
	{ 0x101A2000, "WarioWare: Touched! [PAL]", wwtouched_zip, &wwtouched_zip_size },

	{ 0x101A2100, "Mario & Luigi RPG 2x2 [JPN]", partnersintime_zip, &partnersintime_zip_size },
	{ 0x101A2200, "Mario & Luigi: Partners in Time [USA]", partnersintime_zip, &partnersintime_zip_size },
	{ 0x101A2300, "Mario & Luigi: Partners in Time [PAL]", partnersintime_zip, &partnersintime_zip_size },

	{ 0x101A5200, "Donkey Kong: Jungle Climber [JPN]", dkjclimber_zip, &dkjclimber_zip_size },
	{ 0x101A5300, "DK: Jungle Climber [USA]", dkjclimber_zip, &dkjclimber_zip_size },
	{ 0x101A5400, "Donkey Kong: Jungle Climber [PAL]", dkjclimber_zip, &dkjclimber_zip_size },

	{ 0x101A5500, "Hoshi no Kirby: Sanjou! Dorocche Dan [JPN]", kirby_zip, &kirby_zip_size },
	{ 0x101A5600, "Kirby: Squeak Squad [USA]", kirby_zip, &kirby_zip_size },
	{ 0x101A5700, "Kirby: Mouse Attack [PAL]", kirby_zip, &kirby_zip_size },

	{ 0x101ABD00, "Kaitou Wario the Seven [JPN]", masterofdisguise_zip, &masterofdisguise_zip_size },
	{ 0x101ABE00, "Wario: Master of Disguise [USA]", masterofdisguise_zip, &masterofdisguise_zip_size },
	{ 0x101ABF00, "Wario: Master of Disguise [PAL]", masterofdisguise_zip, &masterofdisguise_zip_size },

	{ 0x101AC000, "Star Fox Command [JPN]", sfcommand_zip, &sfcommand_zip_size },
	{ 0x101AC100, "Star Fox Command [USA]", sfcommand_zip, &sfcommand_zip_size },
	{ 0x101AC200, "Star Fox Command [PAL]", sfcommand_zip, &sfcommand_zip_size },

	{ 0x101B8800, "Touch! Kirby's Magic Paintbrush [JPN]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },
	{ 0x101B8900, "Kirby: Canvas Curse [USA]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },
	{ 0x101B8A00, "Kirby: Power Paintbrush [PAL]", kirbycanvascurse_zip, &kirbycanvascurse_zip_size },

	{ 0x101B8B00, "Zelda no Densetsu: Daichi no Kiteki [JPN]", zeldast_zip, &zeldast_zip_size },
	{ 0x101B8C00, "The Legend of Zelda: Spirit Tracks [USA]", zeldast_zip, &zeldast_zip_size },
	{ 0x101B8D00, "The Legend of Zelda: Spirit Tracks [PAL]", zeldast_zip, &zeldast_zip_size },

	{ 0x101C3300, "Super Mario 64 DS [JPN]", sm64ds_zip, &sm64ds_zip_size },
	{ 0x101C3400, "Super Mario 64 DS [USA]", sm64ds_zip, &sm64ds_zip_size },
	{ 0x101C3500, "Super Mario 64 DS [PAL]", sm64ds_zip, &sm64ds_zip_size },

	{ 0x101C3600, "Zelda no Densetsu: Mugen no Sunadokei [JPN]", zeldaph_zip, &zeldaph_zip_size },
	{ 0x101C3700, "The Legend of Zelda: Phantom Hourglass [USA]", zeldaph_zip, &zeldaph_zip_size },
	{ 0x101C3800, "The Legend of Zelda: Phantom Hourglass [PAL]", zeldaph_zip, &zeldaph_zip_size },

	{ 0x101C8600, "Atsumete! Kirby [JPN]", kirbymassattack_zip, &kirbymassattack_zip_size },
	{ 0x101C8700, "Kirby Mass Attack [USA]", kirbymassattack_zip, &kirbymassattack_zip_size },
	{ 0x101C8800, "Kirby Mass Attack [PAL]", kirbymassattack_zip, &kirbymassattack_zip_size },

	{ 0x101CC200, "Pokemon Ranger [JPN]", pokemonranger_zip, &pokemonranger_zip_size },
	{ 0x101CC300, "Pokemon Ranger [USA]", pokemonranger_zip, &pokemonranger_zip_size },
	{ 0x101CC400, "Pokemon Ranger [PAL]", pokemonranger_zip, &pokemonranger_zip_size },

	{ 0x101D1F00, "Oideyo Doubutsu no Mori [JPN]", animalcrossing_zip, &animalcrossing_zip_size },
	{ 0x101D2000, "Animal Crossing: Wild World [USA]", animalcrossing_zip, &animalcrossing_zip_size },
	{ 0x101D2100, "Animal Crossing: Wild World [PAL]", animalcrossing_zip, &animalcrossing_zip_size },

	{ 0x101E0C00, "Pokemon Fushigi no Dungeon: Sora no Tankentai [JPN]", explorersofsky_zip, &explorersofsky_zip_size },
	{ 0x101E0D00, "Pokemon Mystery Dungeon: Explorers of Sky [USA]", explorersofsky_zip, &explorersofsky_zip_size },
	{ 0x101E0E00, "Pokemon Mystery Dungeon: Explorers of Sky [PAL]", explorersofsky_zip, &explorersofsky_zip_size },

	{ 0x101E0F00, "Pokemon Ranger: Batonnage [JPN]", shadowsofalmia_zip, &shadowsofalmia_zip_size },
	{ 0x101E1000, "Pokemon Ranger: Shadows of Almia [USA]", shadowsofalmia_zip, &shadowsofalmia_zip_size },
	{ 0x101E1100, "Pokemon Ranger: Shadows of Almia [PAL]", shadowsofalmia_zip, &shadowsofalmia_zip_size },

	{ 0x101E6F00, "Pokemon Ranger: Hikari no Kiseki [JPN]", guardiansigns_zip, &guardiansigns_zip_size },
	{ 0x101E7000, "Pokemon Ranger: Guardian Signs [USA]", guardiansigns_zip, &guardiansigns_zip_size },
	{ 0x101E7100, "Pokemon Ranger: Guardian Signs [PAL]", guardiansigns_zip, &guardiansigns_zip_size },
};

static const int GameListSize = sizeof(GameList) / sizeof(gList_t);

#endif
