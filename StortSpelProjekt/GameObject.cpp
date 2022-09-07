#include "GameObject.h"

GameObject::GameObject(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), rotation(rot), mesh(useMesh), objectID(id)
{
	// set position
	mesh->position = pos;

	// set rotation
	mesh->rotation = rot;
}

GameObject::GameObject(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), rotation(rot), objectID(id)
{
	// load obj file
	OBJ testObj(objectPath);
	this->mesh = new Mesh(testObj);


	// load all materials for Obj
	int nrOfMat = testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}
	
	// set position
	mesh->position = pos;

	// set rotation
	mesh->rotation = rot;
}



/*


	

*/