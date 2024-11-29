	#version 330 core

    in vec3 position;         
    in vec3 color;            
    in vec2 texCoord;         
    in vec3 normal;           

    out vec3 Color;             
    out vec2 TexCoord;          
    out vec3 Normal_cameraspace;
    out vec3 vertexPosition_cameraspace; 
    out vec3 LightDirection_cameraspace; 

    uniform mat4 model;        
    uniform mat4 view;     
	uniform mat4 normalMatrix;    
    uniform mat4 projection;   
    uniform vec3 LightPosition_worldspace; 

    void main()
    {
        Color = color; 
        TexCoord = texCoord; 

        vec4 Position_worldspace = model * vec4(position, 1.0);
        vertexPosition_cameraspace = (view * Position_worldspace).xyz;

		Normal_cameraspace = normalize((normalMatrix * vec4(normal, 0.0)).xyz);

        vec3 LightPosition_cameraspace = (view * vec4(LightPosition_worldspace, 1.0)).xyz;
        LightDirection_cameraspace = normalize(LightPosition_cameraspace - vertexPosition_cameraspace);

        gl_Position = projection * view * model * vec4(position, 1.0);
    }
	