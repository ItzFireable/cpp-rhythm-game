Uint32 load_shader(GPU_ShaderEnum shader_type, const char *filename);
GPU_ShaderBlock load_shader_program(Uint32 *p, const char *vertex_shader_file, const char *fragment_shader_file);
void free_shader(Uint32 p);