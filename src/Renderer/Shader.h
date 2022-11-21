#ifndef SHADER_H
#define SHADER_H

#include "../Logger/Logger.h"
#include "types.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
public:
  u32 id;

  Shader( const char* vertexPath, const char* fragmentPath )
  {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try {
      vShaderFile.open( vertexPath );
      fShaderFile.open( fragmentPath );
      std::stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();

    } catch ( std::ifstream::failure& e ) {
      logger.error( "s", "Loading shaders failed" );
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    u32 vertex, fragment;

    // vertex shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );
    checkCompileErrors( vertex, "VERTEX" );

    // fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );
    checkCompileErrors( fragment, "FRAGMENT" );

    // shader Program
    id = glCreateProgram();
    glAttachShader( id, vertex );
    glAttachShader( id, fragment );
    glLinkProgram( id );
    checkCompileErrors( id, "PROGRAM" );

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader( vertex );
    glDeleteShader( fragment );
  };
  // activate the shader
  void use() { glUseProgram( id ); };

  // utility uniform functions
  void setBool( const std::string& name, bool value ) const
  {
    glUniform1i( glGetUniformLocation( id, name.c_str() ),
                 static_cast<int>( value ) );
  };

  void setInt( const std::string& name, s32 value ) const
  {
    glUniform1i( glGetUniformLocation( id, name.c_str() ), value );
  };

  void setFloat( const std::string& name, float value ) const
  {
    glUniform1f( glGetUniformLocation( id, name.c_str() ), value );
  };

  void set3vf( const std::string& name, Colour value ) const
  {
    std::array<f32, 3> v = { value.r, value.g, value.b };
    glUniform3fv( glGetUniformLocation( id, name.c_str() ), 1, v.data() );
  };

  void setMatrix4fv( const std::string& name, glm::mat4 value ) const
  {
    glUniformMatrix4fv( glGetUniformLocation( id, name.c_str() ),
                        1,
                        GL_FALSE,
                        glm::value_ptr( value ) );
  };

private:
  Logger logger;

  // utility function for checking shader compilation/linking errors.
  void checkCompileErrors( u32 shader, std::string type )
  {
    s32 success;
    char infoLog[1024];

    if ( type != "PROGRAM" ) {
      glGetShaderiv( shader, GL_COMPILE_STATUS, &success );

      if ( !success ) {
        glGetShaderInfoLog( shader, 1024, NULL, infoLog );
        logger.error( "ss", "Shader compilation error type: ", type.c_str() );
        logger.error( "s", infoLog );
      }
    } else {
      glGetProgramiv( shader, GL_LINK_STATUS, &success );
      if ( !success ) {
        glGetProgramInfoLog( shader, 1024, NULL, infoLog );
        logger.error( "ss", "Program linking error type: ", type.c_str() );
        logger.error( "s", infoLog );
      }
    }
  }
};

#endif
