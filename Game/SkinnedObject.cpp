#include "SkinnedObject.h"
#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <iostream>
#include <GLEW\glew.h>
#include "GLM/gtx/string_cast.hpp"
#include <tinyxml2\tinyxml2.h>

SkinnedObject::SkinnedObject()
	: m_pCurrentFrame(0),
	m_pHTRAnimation(nullptr),
	m_pJointAnimation(nullptr),
	m_pPlaying(true), m_pDrawMesh(true), m_pDrawBindPose(false), m_pXrayJoints(true),
	m_pJointToBindMat(glm::mat4(1.0f))
{
}

SkinnedObject::~SkinnedObject()
{
	delete m_pHTRAnimation;
	delete m_pJointAnimation;
}

void SkinnedObject::setXRayJoints(bool xray)
{
	m_pXrayJoints = xray;
	for (int i = 0; i < m_pChildren.size(); i++)
	{
		auto sg = dynamic_cast<SkinnedObject*>(m_pChildren[i]);
		if (sg)
			sg->setXRayJoints(m_pXrayJoints);
	}
}

void SkinnedObject::initializeSkeletonFromHTR(std::string htrFilePath, std::string skinWeightFilePath, std::shared_ptr<OBJMesh> bindMesh)
{
	// Load the HTR data
	m_pHTRAnimation = new HTRLoader();
	m_pHTRAnimation->loadHTR(htrFilePath);

	// Create GameObjects from the data
	m_pHTRAnimation->createGameObjects();

	// Add the root node from the HTR file as a child to
	// this GameObject. This allows us to control the locomotion of the hierarchy
	// my changing 'this' GameObject's scale, rotation and translation
	addChild(m_pHTRAnimation->getRootGameObject());

	// The root object (this object) will update the animation playback, all children
	// will look to this object to see what the current animation frame is
	{
		auto gameObjects = m_pHTRAnimation->getGameObjects();
		for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
		{
			(*itr)->m_pCurrentFrame = m_pCurrentFrame;
		}
	}

	/// Load Skin data

	tinyxml2::XMLDocument skinWeightFile;
	skinWeightFile.LoadFile(skinWeightFilePath.c_str());

	tinyxml2::XMLElement* xmlRoot = skinWeightFile.FirstChildElement("deformerWeight");
	tinyxml2::XMLElement* xmlJointItr = xmlRoot->FirstChildElement("weights");

	// Loop through each joint
	for (; xmlJointItr != nullptr; xmlJointItr = xmlJointItr->NextSiblingElement())
	{
		// Get reference to the Joint data
		JointDescriptor* joint = m_pHTRAnimation->getJointDescriptorByName(xmlJointItr->Attribute("source"));
		if (joint == nullptr)
		{
			std::cout << "Error parsing vertex weights" << std::endl;
			assert(0);
		}

		// Load in which vertices are influenced by this joint
		tinyxml2::XMLElement* xmlWeightItr = xmlJointItr->FirstChildElement("point");
		for (; xmlWeightItr != nullptr; xmlWeightItr = xmlWeightItr->NextSiblingElement())
		{
			int vertexIdx = atoi(xmlWeightItr->Attribute("index"));
			float jointWeight = atof(xmlWeightItr->Attribute("value"));
			joint->skinWeights.push_back(std::pair<int, float>(vertexIdx, jointWeight));
		}
		//std::cout << xmlJointItr->Attribute("source") << std::endl;
	}

	m_pBindMesh = bindMesh;

	// Create an intermediate mesh for skinning
	// We're essentially going to generate a new OBJ mesh every frame
	m_pSkinnedMesh = std::make_shared<OBJMesh>();
	m_pSkinnedMesh->objVertices.resize(m_pBindMesh->objVertices.size());
	m_pSkinnedMesh->objNormals.resize(m_pBindMesh->objNormals.size());
	m_pSkinnedMesh->objFaces.resize(m_pBindMesh->objFaces.size());
	m_pSkinnedMesh->textureCoordinates.resize(m_pBindMesh->textureCoordinates.size());

	// Texture coordinates do not need to be morphed, so we can just initialize them here once
	memcpy(&m_pSkinnedMesh->textureCoordinates[0], &m_pBindMesh->textureCoordinates[0], m_pSkinnedMesh->textureCoordinates.size() * sizeof(glm::vec2));

	// Copy the face layout into the new mesh
	memcpy(&m_pSkinnedMesh->objFaces[0], &m_pBindMesh->objFaces[0], m_pSkinnedMesh->objFaces.size() * sizeof(Face3));

	// Tell all the children to use these meshes
	auto gameObjects = m_pHTRAnimation->getGameObjects();
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
	{
		(*itr)->m_pSkinnedMesh = m_pSkinnedMesh;
		(*itr)->m_pBindMesh = m_pBindMesh;
	}
}

glm::mat4 SkinnedObject::getJointToBindMatrix()
{
	return m_pJointToBindMat;
}

void SkinnedObject::update(float dt)
{
	// This GameObject has no jointAnimation so we will construct our
	// transformation matrices in the traditional way
	if (m_pJointAnimation == nullptr)
	{
		if (!m_pPlaying)
			dt = 0.0f;

		GameObject::update(dt);
	}
	else
	{
		//// Todo: create localRotation, scale and translation matrices using HTR data
		glm::mat4 trans = glm::translate(glm::mat4(), m_pJointAnimation->jointBasePosition);
		glm::mat4 rot = glm::mat4_cast(m_pJointAnimation->jointBaseRotation);
		//glm::mat4 sca = glm::scale(glm::mat4(), glm::vec3(m_pJointAnimation->boneLength));
		if (m_pParent)
		{
			SkinnedObject* sg = dynamic_cast<SkinnedObject*>(m_pParent);
			if (sg)
				m_pJointToBindMat = sg->m_pJointToBindMat * (trans * rot);// * sca;
		}
		else
		{
			m_pJointToBindMat = trans * rot;// * sca;
		}

		//double dArray[16] = { 0.0 };
		//const float *pSource = (const float*)glm::value_ptr(m_pJointToBindMat);
		//std::cout << name << std::endl;
		//for (int i = 0; i < 16; ++i)
		//{
		//	dArray[i] = pSource[i];
		//	if (i % 4 == 3)
		//	{
		//		std::cout << dArray[i] << std::endl;
		//	}
		//	else
		//	{
		//		std::cout << dArray[i] << ' ';
		//	}
		//}
		//std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

		m_pLocalRotation =
			glm::mat4_cast(m_pJointAnimation->jointBaseRotation *
				m_pJointAnimation->jointRotations[m_pCurrentFrame]);

		m_pLocalPosition = glm::vec3(m_pJointAnimation->jointBasePosition
			+ m_pJointAnimation->jointPositions[m_pCurrentFrame]);

		m_pLocalScale = glm::vec3(m_pJointAnimation->jointScales[m_pCurrentFrame]);

		//// Create translation matrix
		glm::mat4 tran = glm::translate(glm::mat4(), m_pLocalPosition);

		//// Create scale matrix
		glm::mat4 scal = glm::scale(glm::mat4(), m_pLocalScale);

		m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

		if (m_pParent)
		{
			m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
		}
		else
		{
			m_pLocalToWorldMatrix = m_pLocalTransformMatrix;
		}

		// Update children
		for (int i = 0; i < m_pChildren.size(); i++)
			m_pChildren[i]->update(dt);


		// Increment frame to next frame
		// Note: Right now we are just playing the animation back frame by frame
		// Since we exported the animation at 60 fps, as long as the game runs at 60 fps this works fine
		// However, to make this more robust, you should blend between frames (i.e. lerp / slerp) to get
		// frame rate independent playback. This will also allow you to control the
		// playback speed of the animation (hint: this is what you need to do for assignment 3!)
		m_pCurrentFrame++;

		// Make sure we are within our animation frame count
		if ((m_pCurrentFrame) >= (int)m_pJointAnimation->numFrames)
			(m_pCurrentFrame) = 0;
	}
}

void SkinnedObject::draw()
{
	// Root object will set up the intermediate skinning mesh
	if (m_pParent == nullptr)
	{
		// Clear existing vertices and normals in temporary mesh
		memset(&m_pSkinnedMesh->objVertices[0], 0, m_pSkinnedMesh->objVertices.size() * sizeof(glm::vec3));
		memset(&m_pSkinnedMesh->objNormals[0], 0, m_pSkinnedMesh->objNormals.size() * sizeof(glm::vec3));
	}

	if (m_pJointAnimation != nullptr)
	{
		// Tip: compute this once and store it somewhere
		// Computing the inverse of a matrix is an expensive operation!

		// TODO: calculate m_pJointToBindMat in the update function
		glm::mat4 bindToJointMat = glm::inverse(m_pJointToBindMat);

		// Loop through each vertex this joint influences
		//for (auto itr = m_pJointAnimation->skinWeights.begin(); itr != m_pJointAnimation->skinWeights.end(); itr++)
		for (int i = 0; i < m_pJointAnimation->skinWeights.size(); i++)
		{
			auto itr = &m_pJointAnimation->skinWeights[i];
			int vertexIdx = (*itr).first;
			float jointWeight = (*itr).second;

			// Vertex in the bind mesh
			glm::vec3 vBind = m_pBindMesh->objVertices[vertexIdx];

			// glm::vec4 vJointSpace = ...
			glm::vec4 vJointSpace = bindToJointMat * glm::vec4(vBind, 1.f);
			//std::cout << name << ':' << vJointSpace.x << ';' << vJointSpace.y << ';' << vJointSpace.z << ';' << std::endl;

			// glm::vec4 vWorldSpace = ...
			glm::vec4 vWorldSpace = m_pLocalToWorldMatrix * vJointSpace;

			// glm::vec3 vSkinned = ...
			glm::vec3 vSkinned = jointWeight * vWorldSpace;

			// m_pSkinnedMesh->objVertices[vertexIdx] ...
			m_pSkinnedMesh->objVertices[vertexIdx] += vSkinned;

			// TODO: Update the normals
			glm::vec3 nBind = m_pBindMesh->objNormals[vertexIdx];
			glm::vec4 nJointSpace = bindToJointMat * glm::vec4(nBind, 0.0f);
			glm::vec4 nWorldSpace = m_pLocalToWorldMatrix * nJointSpace;
			glm::vec3 nSkinned = jointWeight * nWorldSpace;
			m_pSkinnedMesh->objNormals[vertexIdx] += nSkinned;
		}
	}

	// Draw children (each joint performs skinning on the vertices they influence)
	for (int i = 0; i < m_pChildren.size(); ++i)
		m_pChildren[i]->draw();

	// Only the root object will draw the final mesh
	if (m_pParent == nullptr)
	{
		//m_pSkinnedMesh->draw(glm::mat4(1.0f));

		// UpdateMesh is a new function added to the OBJMesh class
		// Take a look at what it does.
		m_pSkinnedMesh->updateMesh(true, true, false, false);

		if (m_pXrayJoints)
			glDisable(GL_DEPTH_TEST);

		// Draw the kinematic linkage lines by calling the base class' draw function
		//drawKinematicLinkage();

		if (m_pXrayJoints)
			glEnable(GL_DEPTH_TEST);
	}
}