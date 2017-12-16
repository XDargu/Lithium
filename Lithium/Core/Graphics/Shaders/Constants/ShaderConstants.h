#ifndef _SHADERS_CONSTANTS_H
#define _SHADERS_CONSTANTS_H

#include "Core\Graphics\Shaders\Constants\ShaderPlatform.h"

cbuffer cEgConstantBuffer SHADER_REGISTER(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

#endif