#include "IllustratorSDK.h"
#include "Utility.h"
#include "Ai2VltSuites.h"

bool Ai2Vlt::isShape(AIArtHandle art, const char * shapeName)
{
	// TODO(rgriege): be more thorough
	AIBoolean isDefaultName;
	ai::UnicodeString name;
	sAIArt->GetArtName(art, name, &isDefaultName);
	return name.as_Platform().find(shapeName) != std::string::npos;
}


Ai2Vlt::Error::Error(const char * context, AIErr err) :
	std::exception(context),
	m_err(err),
	m_reported(false)
{
}

Ai2Vlt::Error::operator AIErr() const
{
	return m_err;
}

void Ai2Vlt::Error::report()
{
	if (!m_reported && sAIUser) {
		char errbuf[5];
		*(reinterpret_cast<ASErr*>(errbuf)) = static_cast<ASErr>(m_err);
		errbuf[4] = '\0';
		char buf[1024];
#ifdef WIN_ENV
		char tmpbuf[5];
		*(reinterpret_cast<ASErr*>(tmpbuf)) = static_cast<ASErr>(m_err);
		tmpbuf[4] = '\0';
		errbuf[0] = tmpbuf[3];
		errbuf[1] = tmpbuf[2];
		errbuf[2] = tmpbuf[1];
		errbuf[3] = tmpbuf[0];
		_snprintf(buf, sizeof(buf), "Unexpected internal error=%s, %s", what() ? what() : "<unknown context>", errbuf);
#endif
		sAIUser->ErrorAlert(ai::UnicodeString(buf));
	}
	
	m_reported = true;
}