#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	this->LightType = lType;
	switch (lType) {
	case DIRECTIONAL:
		this->SetPosition(pos);
		this->lightNumber = number;
		this->SetDirection(dir);
		break;
	case POINT:
		this->SetPosition(pos);
		this->lightNumber = number;
		break;
	case SPOT:
		this->SetPosition(pos);
		this->lightNumber = number;
		this->SetDirection(dir);
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
	vec3 color = this->GetColor();
	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType) {
	case DIRECTIONAL:
		glUniform3f(glGetUniformLocation(shad->Program, "lightColor"), color.x, color.y, color.z);
		glUniform3f(glGetUniformLocation(shad->Program, "lightDir"), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->	Program, "objectColor"), 1.0f, 0.5f, 0.31f);
		break;
	case POINT:
		glUniform3f(glGetUniformLocation(shad->Program, "lightColor"), color.x, color.y, color.z);
		glUniform3f(glGetUniformLocation(shad->Program, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shad->Program, "lightPos"), Lpos.x, Lpos.y, Lpos.z);

		variable = "plight[" + std::to_string(lightNumber) + "]";

		break;
	case SPOT:
		glUniform3f(glGetUniformLocation(shad->Program, "lightColor"), color.x, color.y, color.z);
		glUniform3f(glGetUniformLocation(shad->Program, "lightDir"), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shad->Program, "lightPos"), Lpos.x, Lpos.y, Lpos.z);
		variable = "slight[" + std::to_string(lightNumber) + "]";

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