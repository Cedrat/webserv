#ifndef FIELDGET_HPP
#define FIELDGET_HPP

#include "AField.hpp"
#include "RequestInProgress.hpp"
class AMethod;
class FieldGet : public AField
{
    private :
        AMethod *createGetMethod();
        AMethod *createErrorMethod(Config config);
        AMethod *createAiMethod();

    public : 
        FieldGet(std::string str_request, RequestInProgress data_request);
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
