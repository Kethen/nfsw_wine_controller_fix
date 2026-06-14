#include <stdlib.h>
#include <stdbool.h>

#include <dlfcn.h>

#include <SDL2/SDL.h>

#include <unordered_map>

extern "C" {
#include "libdetour/src/libdetour.h"
}

#include "log.h"

DETOUR_DECL_TYPE(int, SDL_WaitEventTimeout_orig, SDL_Event *event, int timeout);
detour_ctx_t SDL_WaitEventTimeout_detour_ctx;

struct controller_state_entry{
	Sint16 ltrigger_real;
	Sint16 rtrigger_real;
};
std::unordered_map<SDL_JoystickID, controller_state_entry> controller_state;

int SDL_WaitEventTimeout_hooked(SDL_Event *event, int timeout){
	//LOG("%s: begin\n", __func__);

	int orig_result;
	DETOUR_ORIG_GET(&SDL_WaitEventTimeout_detour_ctx, orig_result, SDL_WaitEventTimeout_orig, event, timeout);
	if (orig_result == 0){
		return orig_result;
	}
	if (event->type != SDL_CONTROLLERAXISMOTION){
		return orig_result;
	}
	if (event->caxis.axis != SDL_CONTROLLER_AXIS_TRIGGERLEFT && event->caxis.axis != SDL_CONTROLLER_AXIS_TRIGGERRIGHT){
		return orig_result;
	}

	auto entry = controller_state.find(event->caxis.which);
	if (entry == controller_state.end()){
		controller_state.insert_or_assign(event->caxis.which, controller_state_entry{0, 0});
		entry = controller_state.find(event->caxis.which);
	}

	if (event->caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT){
		entry->second.ltrigger_real = event->caxis.value;
	}
	if (event->caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT){
		entry->second.rtrigger_real = event->caxis.value;
	}

	event->caxis.axis = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
	static const Sint32 neutral = SDL_JOYSTICK_AXIS_MAX / 2;
	Sint32 combined_trigger = entry->second.ltrigger_real - entry->second.rtrigger_real;
	event->caxis.value = neutral + combined_trigger / 2;

	//LOG("%s: mixed triggers into %d\n", __func__, event->caxis.value);

	return orig_result;
}

__attribute__((constructor))
int init(){
	init_log();

	void *lib_handle = dlopen("libSDL2-2.0.so.0", RTLD_NOW);
	if (lib_handle == NULL){
		LOG("%s: failed loading libSDL2-2.0.so.0\n", __func__);
		exit(1);
	}

	void *func = dlsym(lib_handle, "SDL_WaitEventTimeout");
	if (func == NULL){
		LOG("%s: failed fetching SDL_WaitEventTimeout from libSDL2-2.0.so.0", __func__);
		exit(1);
	}

	detour_init(&SDL_WaitEventTimeout_detour_ctx, func, (void *)SDL_WaitEventTimeout_hooked);
	bool enable_status = detour_enable(&SDL_WaitEventTimeout_detour_ctx);
	if (!enable_status){
		LOG("%s: failed enabling XCloseDevice detour\n", __func__);
		exit(1);
	}

	LOG("%s: ready\n", __func__);
	return 0;
}
