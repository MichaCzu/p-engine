#define PGE_SHADER_HPP
//
#ifndef PGE_SHADER_HPP
#define PGE_SHADER_HPP
//#define SHADERS_ON

#include <SDL2/SDL_gpu.h>
#include <string>
#include <vector>

namespace pge {

namespace shader {
    void init();
    void free();
    void stop();
}

class Shader {
    bool loaded = false;
    ShaderBlock shaderblock;

protected:
    friend void shader::init();
    friend void shader::free();
    friend void shader::stop();
    virtual void load() = 0;
    virtual void free() = 0;
    void stop();

public:
    bool is_loaded() { return loaded; }
    virtual void start() = 0;
};

class ShaderA : public Shader {
protected:
    void load();
    void free();
};

class ShaderB : public Shader {
protected:
    void load();
    void free();
};

class ShaderC : public Shader {
protected:
    void load();
    void free();
};

extern ShaderA a;
extern ShaderB b;
extern ShaderC c;

namespace shader {

    void init()
    {
#ifdef SHADER_ON
        a->load();
        b->load();
        c->load();
#endif
    }

    void free()
    {
#ifdef SHADER_ON
        a->free();
        b->free();
        c->free();
#endif
    }
    void stop()
    {
        GPU_DisableShader();
    }
}
}

#endif //PGE_THREAD_HPP
