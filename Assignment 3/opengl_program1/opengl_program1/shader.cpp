//
//  shader.cpp
//  Assignment2 - SXD160930
//
//  Created by Sukriti Dixit on 3/16/17.
//  Copyright © 2017 Sukriti Dixit. All rights reserved.
//

#include "shader.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>


GLuint LoadShaders(){
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
	std::string VertexShaderCode = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 normal;\n"
		"in vec4 gxl3d_Color;"
    "out vec3 Normal;\n"
    "out vec3 FragPos;\n"
		"flat out vec4 VertexColor"
    
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 MVP;\n"
    
    
    "void main()\n"
    "{\n"
        "gl_Position = MVP * vec4(position, 1.0f);\n"
        "FragPos = vec3(model * vec4(position, 1.0f));\n"
        "Normal = mat3(transpose(inverse(model))) * normal;\n"
		"vertexColor = gxl3d_Color"
    "}\0";
                                    
    
    
    // Read the Fragment Shader code from the file
		std::string FragmentShaderCode = "#version 330 core\n"
			"out vec4 color;\n"

			"in vec3 FragPos;\n"
			"in vec3 Normal;\n"
			"flat in vec4 VertexColor;"
			"uniform vec3 lightPos;\n"
			"uniform vec3 viewPos;\n"
			"uniform vec3 ambientColor;\n"
			"uniform vec3 diffuseColor;\n"
			"uniform vec3 specularColor;\n"
			"uniform vec3 lightColor;\n"
			"uniform vec3 objectColor;\n"

			"void main()\n"
			"{\n"
			// Ambient
			"float ambientStrength = 0.1f;\n"
			"vec3 ambient = ambientStrength * ambientColor * lightColor;\n"

			// Diffuse
			"vec3 norm = normalize(Normal);\n"
			"vec3 lightDir = normalize(lightPos - FragPos);\n"
			"float diff = max(dot(norm, lightDir), 0.0);\n"
			"vec3 diffuse = diff * diffuseColor * lightColor;\n"

			// Specular
			"float specularStrength = 0.5f;\n"
			"vec3 viewDir = normalize(viewPos - FragPos);\n"
			"vec3 reflectDir = reflect(-lightDir, norm);\n"
			"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
			"vec3 specular = specularStrength * spec * specularColor * lightColor;\n"

			"vec3 result = (ambient + diffuse + specular) * objectColor;\n"
			//"Vertex_color += vec4(result, 1.0f);\n"
			"color = vec4(result, 1.0f);\n";
    "}\0";
    
    /*"#version 330 core\n"
    
    // Interpolated values from the vertex shaders
    "in vec2 UV;\n"
    "in vec3 Position_worldspace;\n"
    "in vec3 Normal_cameraspace;\n"
    "in vec3 EyeDirection_cameraspace;\n"
    "in vec3 LightDirection_cameraspace;\n"
    
    // Ouput data
    "out vec3 color;\n"
    
    // Values that stay constant for the whole mesh.
    "uniform sampler2D myTextureSampler;\n"
    "uniform mat4 MV;\n"
    "uniform vec3 LightPosition_worldspace;\n"
    
    //"uniform vec3 incolor;\n"
    //"out vec4 color1;\n"
     
    "void main(){\n"
        
        //"color1 = vec4(incolor, 1.0f);\n"
        // Light emission properties
        // You probably want to put them as uniforms
        "vec3 LightColor = vec3(1,1,1);\n"
        "float LightPower = 50.0f;\n"
        
        // Material properties
       //"vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;\n"
        "vec3 MaterialDiffuseColor = vec3(1.0f, 0.5f, 0.0f);\n"
        "vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;\n"
        "vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);\n"
        
        // Distance to the light
        "float distance = length( LightPosition_worldspace - Position_worldspace );\n"
        
        // Normal of the computed fragment, in camera space
        "vec3 n = normalize( Normal_cameraspace );\n"
        // Direction of the light (from the fragment to the light)
        "vec3 l = normalize( LightDirection_cameraspace );\n"
        // Cosine of the angle between the normal and the light direction,
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        "float cosTheta = clamp( dot( n,l ), 0,1 );\n"
        
        // Eye vector (towards the camera)
        "vec3 E = normalize(EyeDirection_cameraspace);\n"
        // Direction in which the triangle reflects the light
        "vec3 R = reflect(-l,n);\n"
        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        "float cosAlpha = clamp( dot( E,R ), 0,1 );\n"
        
        "color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);\n"
        // Ambient : simulates indirect lighting
        // Diffuse : "color" of the object
       // Specular : reflective highlight, like a mirror
    
    "}\0";*/
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    
    // Compile Vertex Shader
    //printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    
    
    // Compile Fragment Shader
    //printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    
    
    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}
