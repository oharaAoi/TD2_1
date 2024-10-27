#include "JumpAssessor.h"

JumpAssessor::JumpAssessor(){}
JumpAssessor::~JumpAssessor(){}

void JumpAssessor::Update(){
	if(!isUIvisible_){ return; }
}

void JumpAssessor::Draw(){
	if(!isUIvisible_){ return; }
}

void JumpAssessor::SetJumpData(float speed, int32_t bodyCount,WorldTransform* parentWT){

	if(speed >= 150.0f && bodyCount == 8){// prefect
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationPERFECT/", "evaluationPERFECT.gltf",
			parentWT,
			false, 
			Vector3(0.5f, 0.5f, 0.5f),Quaternion(), Vector3()
		);

	} else if(speed >= 120.0f or bodyCount >= 6){// good
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationGood/", "evaluationGood.gltf",
			parentWT,
			false,
			Vector3(0.5f, 0.5f, 0.5f), Quaternion(), Vector3()
		);
	
	} else if(speed >= 70.0f or bodyCount >= 4){// nice
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationNice/", "evaluationNice.gltf",
			parentWT,
			false,
			Vector3(1, 1, 1), Quaternion(), Vector3()
		);
	
	} else{

	}

}