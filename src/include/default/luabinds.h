std::vector<GPU_Rect> rect_underlays;
std::vector<SDL_Color> color_underlays;

std::vector<GPU_Rect> flashes;
std::vector<SDL_Color> flashColors;
std::vector<float> flashTimers;
std::vector<float> flashDurations;

std::vector<Uint32> shaders;
std::vector<GPU_ShaderBlock> shader_blocks;
std::vector<int> time_uniforms;
std::vector<int> res_uniforms;
std::vector<int> mask_uniforms;
std::vector<int> mask_tex_uniforms;
std::vector<bool> enabled;
std::vector<bool> isBackground;
std::vector<bool> isOverlay;

extern "C" int create_rect_underlay(lua_State *L);
extern "C" int modify_rect_color(lua_State *L);
extern "C" int modify_rect(lua_State *L);
extern "C" int delete_rect(lua_State *L);

extern "C" int createShader(lua_State *L);
extern "C" int createBackgroundShader(lua_State *L);
extern "C" int createOverlayShader(lua_State *L);

extern "C" int manageShader(lua_State *L);
extern "C" int setShaderMask(lua_State *L);
extern "C" int queueShaderProperty(lua_State *L);
extern "C" int modifyShaderProperty(lua_State *L);

extern "C" int setWindowPosition(lua_State *L);
extern "C" int setWindowSize(lua_State *L);

extern "C" int flashRect(lua_State *L);
extern "C" int setPlayfieldPosition(lua_State *L);
extern "C" int setReceptorPosition(lua_State *L);

extern "C" int MeMod(lua_State *L);
extern "C" int SetMod(lua_State *L);
extern "C" int setModifierOffset(lua_State *L);

void initLua();