#include <default/luabinds.h>

extern "C" int create_rect_underlay(lua_State *L)
{
	GPU_Rect rect;
	rect.x = luaL_checknumber(L, 1);
	rect.y = luaL_checknumber(L, 2);
	rect.w = luaL_checknumber(L, 3);
	rect.h = luaL_checknumber(L, 4);

	SDL_Color color;
	color.r = luaL_checknumber(L, 5);
	color.g = luaL_checknumber(L, 6);
	color.b = luaL_checknumber(L, 7);
	color.a = luaL_checknumber(L, 8);

	rect_underlays.push_back(rect);
	color_underlays.push_back(color);

	int id = rect_underlays.size() - 1;
	lua_pushinteger(L, id);

	return 1;
}

extern "C" int modify_rect_color(lua_State *L)
{
	int id = luaL_checknumber(L, 1);

	SDL_Color color;
	color.r = luaL_checknumber(L, 2);
	color.g = luaL_checknumber(L, 3);
	color.b = luaL_checknumber(L, 4);
	color.a = luaL_checknumber(L, 5);

	color_underlays[id] = color;

	return 0;
}
extern "C" int modify_rect(lua_State *L)
{
	int id = luaL_checknumber(L, 1);

	GPU_Rect rect;
	rect.x = luaL_checknumber(L, 2);
	rect.y = luaL_checknumber(L, 3);
	rect.w = luaL_checknumber(L, 4);
	rect.h = luaL_checknumber(L, 5);

	rect_underlays[id] = rect;

	return 0;
}
extern "C" int delete_rect(lua_State *L)
{
	int id = luaL_checknumber(L, 1);

	rect_underlays.erase(rect_underlays.begin() + id);
	color_underlays.erase(color_underlays.begin() + id);

	return 0;
}

extern "C" int createShader(lua_State *L)
{
	Uint32 shader;
	const char *frag = luaL_checkstring(L, 1);
	const char *vert = luaL_checkstring(L, 2);
	int enable = luaL_checknumber(L, 3);

	GPU_ShaderBlock shader_block = load_shader_program(&shader, con(SHADER_PATH, vert), con(SHADER_PATH, frag));
	shaders.push_back(shader);
	enabled.push_back(enable == 0 ? false : true);

	frags.push_back(frag);
	verts.push_back(vert);

	isBackground.push_back(false);
	isOverlay.push_back(false);
	shader_blocks.push_back(shader_block);

	time_uniforms.push_back(GPU_GetUniformLocation(shader, "time"));
	res_uniforms.push_back(GPU_GetUniformLocation(shader, "resolution"));
	mask_uniforms.push_back(GPU_GetUniformLocation(shader, "mask"));
	mask_tex_uniforms.push_back(GPU_GetUniformLocation(shader, "mask_tex"));

	int id = shaders.size() - 1;
	lua_pushinteger(L, id);

	return 1;
}
extern "C" int createBackgroundShader(lua_State *L)
{
	Uint32 shader_;
	const char *frag = luaL_checkstring(L, 1);
	const char *vert = luaL_checkstring(L, 2);
	int enable = luaL_checknumber(L, 3);

	GPU_ShaderBlock shader_block = load_shader_program(&shader_, con(SHADER_PATH, vert), con(SHADER_PATH, frag));
	shaders.push_back(shader_);
	enabled.push_back(enable == 0 ? false : true);

	frags.push_back(frag);
	verts.push_back(vert);

	isBackground.push_back(true);
	isOverlay.push_back(false);
	shader_blocks.push_back(shader_block);

	time_uniforms.push_back(GPU_GetUniformLocation(shader_, "time"));
	res_uniforms.push_back(GPU_GetUniformLocation(shader_, "resolution"));
	mask_uniforms.push_back(GPU_GetUniformLocation(shader_, "mask"));
	mask_tex_uniforms.push_back(GPU_GetUniformLocation(shader_, "mask_tex"));

	int id = shaders.size() - 1;
	lua_pushinteger(L, id);

	return 1;
}

extern "C" int createOverlayShader(lua_State *L)
{
	Uint32 shader_;
	const char *frag = luaL_checkstring(L, 1);
	const char *vert = luaL_checkstring(L, 2);
	int enable = luaL_checknumber(L, 3);

	GPU_ShaderBlock shader_block = load_shader_program(&shader_, con(SHADER_PATH, vert), con(SHADER_PATH, frag));
	shaders.push_back(shader_);
	enabled.push_back(enable == 0 ? false : true);

	frags.push_back(frag);
	verts.push_back(vert);

	isBackground.push_back(false);
	isOverlay.push_back(true);
	shader_blocks.push_back(shader_block);

	time_uniforms.push_back(GPU_GetUniformLocation(shader_, "time"));
	res_uniforms.push_back(GPU_GetUniformLocation(shader_, "resolution"));
	mask_uniforms.push_back(GPU_GetUniformLocation(shader_, "mask"));
	mask_tex_uniforms.push_back(GPU_GetUniformLocation(shader_, "mask_tex"));

	int id = shaders.size() - 1;
	lua_pushinteger(L, id);

	return 1;
}

extern "C" int setShaderMask(lua_State *L)
{
	int id = luaL_checkinteger(L, 1);
	const char *mask = luaL_checkstring(L, 2);

	int loc = mask_uniforms[id];

	GPU_Image *mask_ = GPU_LoadImage(con(IMG_PATH, con(mask, ".png")));
	float res[2] = {(float)mask_->w, (float)mask_->h};

	if (mask_ != NULL)
	{
		GPU_SetUniformfv(loc, 2, 1, res);
		GPU_SetShaderImage(mask_, mask_tex_uniforms[id], 1);
	}

	return 0;
}
extern "C" int manageShader(lua_State *L)
{
	int id = luaL_checkinteger(L, 1);
	bool enable = luaL_checkinteger(L, 2);

	enabled[id] = enable == 0 ? false : true;
	return 0;
}
extern "C" int queueShaderProperty(lua_State *L)
{
	int id = luaL_checkinteger(L, 1);
	const char *property = luaL_checkstring(L, 2);

	int val = GPU_GetUniformLocation(shaders[id], property);
	lua_pushinteger(L, val);

	return 1;
}
extern "C" int modifyShaderProperty(lua_State *L)
{
	int id = luaL_checkinteger(L, 1);
	int property = luaL_checkinteger(L, 2);
	float value = luaL_checknumber(L, 3);

	GPU_ActivateShaderProgram(shaders[id], &shader_blocks[id]);
	GPU_SetUniformf(property, value);

	return 0;
}
extern "C" int setWindowPosition(lua_State *L)
{
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	float screen_pos_x = screen_width / 2 - WIDTH / 2;
	float screen_pos_y = screen_height / 2 - HEIGHT / 2;

	if (window != NULL)
	{
		SDL_SetWindowPosition(window, screen_pos_x + x, screen_pos_y + y);
	}

	return 0;
}
extern "C" int setWindowSize(lua_State *L)
{
	int w = luaL_checknumber(L, 1);
	int h = luaL_checknumber(L, 2);

	if (window != NULL)
	{
		WIDTH = w;
		HEIGHT = h;

		/*GPU_FreeImage(temp);*/
		GPU_ClearRGBA(renderer, 0, 0, 0, 255);
		GPU_ClearRGBA(framebuffer->target, 0, 0, 0, 255);

		GPU_SetWindowResolution(w, h);
		GPU_ResetProjection(renderer);
	}

	return 0;
}

extern "C" int flashRect(lua_State *L)
{
	float r = luaL_checknumber(L, 1);
	float g = luaL_checknumber(L, 2);
	float b = luaL_checknumber(L, 3);
	float duration = luaL_checknumber(L, 4);

	GPU_Rect flash;
	flash.x = 0;
	flash.y = 0;
	flash.w = WIDTH;
	flash.h = HEIGHT;

	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;

	flashes.push_back(flash);
	flashColors.push_back(color);
	flashTimers.push_back(duration);
	flashDurations.push_back(duration);

	return 0;
}

extern "C" int setPlayfieldPosition(lua_State *L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);

	for (Playfield *playfield : playfields)
	{
		playfield->SetPosition(x, y);
	}
	return 0;
}

extern "C" int setReceptorPosition(lua_State *L)
{
	int id = luaL_checkinteger(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);

	for (Playfield *playfield : playfields)
	{
		std::vector<Receptor *> receptors = playfield->GetReceptors();
		std::vector<Lane *> lanes = playfield->GetLanes();

		receptors[id]->SetPosition(x, y);
		lanes[id]->SetPosition(x, y);
	}

	return 0;
}

extern "C" int SetMod(lua_State *L)
{
	float beat = luaL_checknumber(L, 1);
	float value = luaL_checknumber(L, 2);
	const char *mod = luaL_checkstring(L, 3);
	int column = luaL_checkinteger(L, 4);

	manager->set(beat, value, (char *)mod, column - 1);
	return 0;
}

extern "C" int MeMod(lua_State *L)
{
	float beat = luaL_checknumber(L, 1);
	float time = luaL_checknumber(L, 2);
	const char *easing = luaL_checkstring(L, 3);
	float value = luaL_checknumber(L, 4);
	const char *mod = luaL_checkstring(L, 5);
	int column = luaL_checkinteger(L, 6);

	manager->me(beat, time, (char *)easing, value, (char *)mod, column - 1);
	return 0;
}

extern "C" int setModifierOffset(lua_State *L)
{
	float id = luaL_checkinteger(L, 2);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);

	manager->luaOffset[id][0] = x;
	manager->luaOffset[id][1] = y;

	return 0;
}

void initLua()
{
	logger->Log(DebugP, "Started loading lua");
	luaL_openlibs(L);

	lua_register(L, "setWindowPosition", setWindowPosition);
	lua_register(L, "setWindowSize", setWindowSize);

	lua_register(L, "createShader", createShader);
	lua_register(L, "createBackgroundShader", createBackgroundShader);
	lua_register(L, "createOverlayShader", createOverlayShader);
	lua_register(L, "setShaderMask", setShaderMask);

	lua_register(L, "queueShaderProperty", queueShaderProperty);
	lua_register(L, "modifyShaderProperty", modifyShaderProperty);
	lua_register(L, "manageShader", manageShader);

	lua_register(L, "createRectUnderlay", create_rect_underlay);
	lua_register(L, "modifyRectColor", modify_rect_color);
	lua_register(L, "modifyRect", modify_rect);
	lua_register(L, "flash", flashRect);

	lua_register(L, "setPlayfieldPosition", setPlayfieldPosition);
	lua_register(L, "setReceptorPosition", setReceptorPosition);

	lua_register(L, "setMod", SetMod);
	lua_register(L, "meMod", MeMod);
	lua_register(L, "setModifierOffset", setModifierOffset);

	logger->Log(DebugP, "Created binds");

	if (luaL_dofile(L, con(SONG_PATH, con(SONG_NAME.c_str(), "/modchart.lua"))) != LUA_OK)
	{
		logger->Log(ErrorP, lua_tostring(L, -1));
	}
	else
	{
		logger->Log(DebugP, "Loaded file");

		lua_getglobal(L, "init");
		lua_pcall(L, 0, 0, 0);

		logger->Log(DebugP, "Called init");
	}
}