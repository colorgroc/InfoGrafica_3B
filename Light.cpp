#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 color, vec3 diffuse, vec3 specular, LType lType, int number) {
	this->LightType = lType;

	this->Lambient = ambient;
	this->Ldiffuse = diffuse;
	this->Lspecular = specular;
	this->Ldirection = dir;
	switch (lType) {
	case DIRECTIONAL:
		this->Color = color;
		this->Lpos = pos;

		this->lightNumber = number;
		break;
	case POINT:
		this->Color = color;
		this->Lpos = pos;
		this->lightNumber = number;
		break;
	case SPOT:
		this->Color = color;
		this->Lpos = pos;
	//	this->Ldirection = dir;
		this->lightNumber = number;
		break;
	default:
		break;
	}
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	if (LightType == POINT) {
		this->c1 = constant;
		this->c2 = lineal;
		this->c3 = quadratic;
	}
	else if (LightType == SPOT) {
		this->c1 = constant;
		this->c2 = lineal;
		this->c3 = quadratic;
	}
}

void Light::SetAperture(float min, float max) {
	if (LightType == DIRECTIONAL) {
		this->MaxAperture = max;
		this->MinAperture = min;
	}
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;
	//vec3 color = this->GetColor();
	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType) {
	case DIRECTIONAL:	
		//glUniform3f(glGetUniformLocation(shad->Program, "directionLight.viewPos"), CamPos.x, CamPos.y, CamPos.z);
		glUniform3f(glGetUniformLocation(shad->Program, "directionLight.lightColor"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shad->Program, "directionLight.lightdir"), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->	Program,"directionLight.objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shad->Program, "directionLight.dirAmbient"), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, "directionLight.dirSpec"), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, "directionLight.dirDiffuse"), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		break;
	case POINT:
		variable = "pointLight[" + std::to_string(lightNumber) + "]";
		//glUniform3f(glGetUniformLocation(shad->Program, (variable + "viewPos").c_str()), CamPos.x, CamPos.y, CamPos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightColor").c_str()), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".objectColor").c_str()), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c1").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c2").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c3").c_str()), c3);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pointAmbient").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pointSpec").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pointDiffuse").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		break;
	case SPOT:
		variable = "focalLight[" + std::to_string(lightNumber) + "]";
	//	glUniform3f(glGetUniformLocation(shad->Program, (variable + "viewPos").c_str()), CamPos.x, CamPos.y, CamPos.z);	
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightColor").c_str()), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightdir").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".objectColor").c_str()), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c1").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c2").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c3").c_str()), c3);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMx").c_str()), MaxAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMn").c_str()), MinAperture);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".focAmbient").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".focSpec").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".focDiffuse").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);

		break;
	default:
		break;
	}
}

void Light::Rotate(vec3 rotation) {
	//opcional
}

void Light::SetDirection(vec3 dir) {
	this->Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	this->Lpos = pos;
}

vec3 Light::GetColor() {
	vec3 color(1.0f);
	return color;
}

Light::~Light() {

}