#pragma once

#include "Object.h"
#include "HTRLoader.h"
#include "Mesh.h"

// Forward declare the HTRLoader class
// This is necessary because SkinnedObject references HTRLoader and HTRLoader references SkinnedObject
class HTRLoader;

class SkinnedObject : public GameObject
{
	// Let the HTRLoader class access private and protected member variables
	friend class HTRLoader;

protected:
	int m_pCurrentFrame;
	HTRLoader* m_pHTRAnimation;

	std::shared_ptr<OBJMesh> m_pBindMesh; // Mesh in t-pose
	std::shared_ptr<OBJMesh> m_pSkinnedMesh;
	JointDescriptor* m_pJointAnimation; // The animation for this specific joint

public:
	SkinnedObject();
	~SkinnedObject();

	void setXRayJoints(bool xray);

	bool m_pPlaying, m_pDrawMesh, m_pDrawBindPose, m_pXrayJoints;

	void initializeSkeletonFromHTR(std::string htrFilePath, std::string skinWeightFilePath, std::shared_ptr<OBJMesh> bindMesh);

	glm::mat4 m_pJointToBindMat;
	glm::mat4 getJointToBindMatrix();

	virtual void update(float dt);
	void draw();
};