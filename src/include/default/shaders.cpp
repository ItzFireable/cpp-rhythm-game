#include <default/shaders.h>

Uint32 load_shader(GPU_ShaderEnum shader_type, const char *filename)
{
	SDL_RWops *rwops;
	Uint32 shader;
	char *source;
	int header_size, file_size;
	const char *header = "";
	GPU_Renderer *renderer = GPU_GetCurrentRenderer();

	rwops = SDL_RWFromFile(filename, "rb");
	if (rwops == NULL)
	{
		GPU_PushErrorCode("load_shader", GPU_ERROR_FILE_NOT_FOUND, "Shader file \"%s\" not found", filename);
		return 0;
	}

	file_size = SDL_RWseek(rwops, 0, SEEK_END);
	SDL_RWseek(rwops, 0, SEEK_SET);

	if (renderer->shader_language == GPU_LANGUAGE_GLSL)
	{
		if (renderer->max_shader_version >= 120)
			header = "#version 120\n";
		else
			header = "#version 110\n";
	}
	else if (renderer->shader_language == GPU_LANGUAGE_GLSLES)
		header = "#version 100\nprecision mediump int;\nprecision mediump float;\n";

	header_size = strlen(header);

	source = (char *)malloc(sizeof(char) * (header_size + file_size + 1));
	strcpy(source, header);
	SDL_RWread(rwops, source + strlen(source), 1, file_size);
	source[header_size + file_size] = '\0';
	shader = GPU_CompileShader(shader_type, source);
	free(source);
	SDL_RWclose(rwops);

	return shader;
}

GPU_ShaderBlock load_shader_program(Uint32 *p, const char *vertex_shader_file, const char *fragment_shader_file)
{
	Uint32 v, f;
	v = load_shader(GPU_VERTEX_SHADER, vertex_shader_file);
	f = load_shader(GPU_FRAGMENT_SHADER, fragment_shader_file);
	*p = GPU_LinkShaders(v, f);

	if (!*p)
	{
		GPU_ShaderBlock b = {-1, -1, -1, -1};
		return b;
	}

	GPU_ShaderBlock block = GPU_LoadShaderBlock(*p, "gpu_Vertex", "gpu_TexCoord", "gpu_Color", "gpu_ModelViewProjectionMatrix");

	return block;
}

void free_shader(Uint32 p)
{
	GPU_FreeShaderProgram(p);
}