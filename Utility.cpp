#include "IllustratorSDK.h"
#include "Utility.h"
#include "Ai2VltSuites.h"

bool isShape(AIArtHandle art, const char * shapeName)
{
	// TODO(rgriege): be more thorough
	AIBoolean isDefaultName;
	ai::UnicodeString name;
	sAIArt->GetArtName(art, name, &isDefaultName);
	return name.as_Platform().find(shapeName) != std::string::npos;
}