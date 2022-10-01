#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_UV_x2;
layout(location=2) in vec4 in_Normal;
layout(location=3) in vec4 in_BoneWeights;
layout(location=4) in vec4 in_BoneIDs;

out vec4 ex_Position;			
out vec4 ex_PositionWorld;
out vec4 ex_Normal;
out vec4 ex_UV_x2;

uniform mat4 ModelMatrix;	
uniform mat4 RotationMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform int NumberOfBones;
uniform mat4 BoneMatrices[48];
uniform mat4 BoneRotationMatrices[48];

void main(void)
{
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	// Positions
	vec4 position = vec4(0.0f);
	position += BoneMatrices[int(in_BoneIDs[0])] * in_Position * in_BoneWeights[0];
	position += BoneMatrices[int(in_BoneIDs[1])] * in_Position * in_BoneWeights[1];
	position += BoneMatrices[int(in_BoneIDs[2])] * in_Position * in_BoneWeights[2];
	position += BoneMatrices[int(in_BoneIDs[3])] * in_Position * in_BoneWeights[3];

	
	vec4 normal = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	normal += BoneRotationMatrices[int(in_BoneIDs[0])] * in_Normal * in_BoneWeights[0];
	normal += BoneRotationMatrices[int(in_BoneIDs[1])] * in_Normal * in_BoneWeights[1];
	normal += BoneRotationMatrices[int(in_BoneIDs[2])] * in_Normal * in_BoneWeights[2];
	normal += BoneRotationMatrices[int(in_BoneIDs[3])] * in_Normal * in_BoneWeights[3];

	// Normals
	
	gl_Position = MVP * position;

	ex_Position = position;
	ex_PositionWorld = ModelMatrix * position;
	ex_Normal = RotationMatrix * normalize(normal);
	ex_UV_x2 = in_UV_x2;

	return;
}
