//managing game controls, getting text input.
#pragma once

#include <string>

enum en_transl {
    et_msellang = 0,
    et_mnewgame,
    et_mloadgame,
    et_moptions,
    et_mcredits,
    et_mquit,
    et_mdeveloped,
    et_mlanguage,
    et_mgraphics,
    et_msound,
    et_mcontrols,
    et_mapply,
    et_mback,
    et_mmusic,
    et_mframerate,
    et_mresolution,
    et_mfullscreen,
    et_menabled,
    et_mdisabled,
    et_mquality,
    et_mhigh,
    et_mmedium,
    et_mlow,
};
std::wstring tr(en_transl _key);

namespace pge {
	namespace locale {

		bool load(std::string _path); //reads file with translations that overwrite defaults
		bool setfallback(std::string _path);
		std::wstring get(en_transl _key);
		void resort();
	}
}
