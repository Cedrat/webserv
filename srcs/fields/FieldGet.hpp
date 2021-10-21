#ifndef FIELDGET_HPP
#define FIELDGET_HPP

#include "AField.hpp"
#include "../parsing/RequestInProgress.hpp"
class AMethod;
class FieldGet : public AField
{
    private :
        AMethod *createGetMethod();
        AMethod *createErrorMethod(Config config, Location location);
        AMethod *createAiMethod();
        //AMethod *createRedirMethod(Config config, Location location);
        AMethod *createCgiMethod(Config config, Location location);

    public : 
        FieldGet(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd);
        ~FieldGet();

        void        fillField();
        void        verifyMissingData();
        void        verifyData();
		void		verifyRedirect(Location const &);
        
        //void        verifyAI(Location const &);
        bool        isAIPath(std::string path, Location const &location); 
        AMethod *	getAMethod();
};

#endif
